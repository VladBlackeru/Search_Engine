#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <libpq-fe.h>
#include <regex>

std::ofstream errorLog("err.log", std::ios::app);

std::string replaceBackslashes(const std::string& input) {
    std::regex backslashPattern("\\\\");
    return std::regex_replace(input, backslashPattern, "/");
}

PGconn* establish_connection(){
    const char* conninfo = "dbname=postgres user=postgres password=177013 hostaddr=127.0.0.1 port=5432";

    PGconn* conn = PQconnectdb(conninfo);
    if (PQstatus(conn) != CONNECTION_OK) {
        errorLog << "Connection to database failed: " << PQerrorMessage(conn) << '\n';
        PQfinish(conn);
        return nullptr;
    }
    return conn;
}

bool deleteRowFromDatabase(PGconn* conn, const std::string& tableName,const std::string& filedName, const std::string& id) {
    if (PQstatus(conn) != CONNECTION_OK) {
        errorLog << "Database connection is not valid: " << PQerrorMessage(conn) << '\n';
        return false;
    }

    std::string query = "DELETE FROM " + tableName + " WHERE " + filedName + " = $1;";
    const char* paramValues[1] = {id.c_str()};
    PGresult* res = PQexecParams(conn, query.c_str(), 1, nullptr, paramValues, nullptr, nullptr, 0);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        errorLog << "Delete operation failed: " << PQerrorMessage(conn) << '\n';
        PQclear(res);
        return false;
    }
    PQclear(res);
    return true;
}

bool insertFileToDatabase(PGconn* conn, const std::string& filePath) {

    if (PQstatus(conn) != CONNECTION_OK) {
        errorLog << "Database connection is not valid: " << PQerrorMessage(conn) << '\n';
        return false;
    }

    std::ifstream file(filePath);
    if (!file) {
        errorLog << "File failed to open the file: " << filePath << '\n';
        return false;
    }
    std::string fileName = std::filesystem::path(filePath).filename().string();
    std::string fileExtension = std::filesystem::path(filePath).extension().string();
    auto fileSize = std::filesystem::file_size(filePath);


    std::cout<<fileName<<" ";

    const char* fileSql =
            "INSERT INTO search_engine.files (path, name, extension, size) "
            "VALUES ($1, $2, $3, $4) RETURNING id;";
    const char* fileParams[4] = {filePath.c_str(), fileName.c_str(), fileExtension.c_str(),
                                 std::to_string(fileSize).c_str()};
    PGresult* fileRes = PQexecParams(conn, fileSql, 4, nullptr, fileParams, nullptr, nullptr, 0);
    if (PQresultStatus(fileRes) != PGRES_TUPLES_OK) {
        errorLog << filePath << '\n' << "File metadata insertion failed: " << PQerrorMessage(conn) << '\n';
        PQclear(fileRes);
        return false;
    }

    int fileId = std::stoi(PQgetvalue(fileRes, 0, 0));
    PQclear(fileRes);

    std::string line;
    int line_number = 1;
    while (std::getline(file, line)) {
        const char* contentSql =
                "INSERT INTO search_engine.file_contents (filesid, content, line_number) VALUES ($1, $2, $3);";
        const char* contentParams[3] = {std::to_string(fileId).c_str(), line.c_str(), std::to_string(line_number).c_str()};
        PGresult* contentRes = PQexecParams(conn, contentSql, 3, nullptr, contentParams, nullptr, nullptr, 0);
        if (PQresultStatus(contentRes) != PGRES_COMMAND_OK) {
            errorLog << fileName << '\n' << "File content insertion failed: " << PQerrorMessage(conn) << '\n';
            PQclear(contentRes);
            deleteRowFromDatabase(conn, "search_engine.files", "path", filePath);
            return false;
        }
        PQclear(contentRes);
        ++line_number;
    }

    file.close();
    return true;
}

void printSearchResults(PGconn* conn, const std::string& searchPhrase) {
    if (PQstatus(conn) != CONNECTION_OK) {
        errorLog << "Database connection is not valid: " << PQerrorMessage(conn) << '\n';
        return;
    }

    const char* query = R"(
    WITH full_text_search AS (
        SELECT fc.*, f.name AS file_name
        FROM search_engine.file_contents fc
        JOIN search_engine.files f ON fc.filesid = f.id
        WHERE fc.content_tsvector @@ phraseto_tsquery('english', $1)
    ),
    partial_word_match AS (
        SELECT fc.*, f.name AS file_name
        FROM search_engine.file_contents fc
        JOIN search_engine.files f ON fc.filesid = f.id
        WHERE fc.content LIKE '%' || $1 || '%'
    )
    SELECT DISTINCT content, line_number, file_name
    FROM full_text_search
    UNION
    SELECT DISTINCT content, line_number, file_name
    FROM partial_word_match;
    )";

    const char* paramValues[1] = {searchPhrase.c_str()};

    PGresult* res = PQexecParams(conn, query, 1, nullptr, paramValues, nullptr, nullptr, 0);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        errorLog << "Search query failed: " << PQerrorMessage(conn) << '\n';
        PQclear(res);
        return;
    }

    int nRows = PQntuples(res);
    int nCols = PQnfields(res);

    std::cout << "Search results for: '" << searchPhrase << "':" << '\n';
    for (int i = 0; i < nRows; ++i) {
        for (int j = 0; j < nCols; ++j) {
            std::cout << PQfname(res, j) << ": " << PQgetvalue(res, i, j) << "\t";
        }
        std::cout << '\n';
    }

    PQclear(res);
}

void insertRec( PGconn* conn, const std::string& directoryPath) {
    try {
        if (!std::filesystem::exists(directoryPath) || !std::filesystem::is_directory(directoryPath)) {
            errorLog << "Invalid directory path: " << directoryPath << '\n';
            return;
        }

        for (const auto& entry : std::filesystem::recursive_directory_iterator(directoryPath)) {
            if (std::filesystem::is_regular_file(entry)) {
                if(insertFileToDatabase(conn,replaceBackslashes(entry.path().string()))){
                    std::cout<<"File succesfully inserted into the DB"<<'\n';
                }
                else
                    std::cout<<"File failed to insert into DB"<<'\n';
            }
        }
    } catch (const std::exception& e) {
        errorLog << "Error while traversing directory: " << e.what() << '\n';
    }
}



int main() {
    PGconn* conn = establish_connection();
    int op;
    std::string path;
    printf("Menu:\n");
    printf(" 1 - Insert 1 file into DB\n");
    printf(" 2 - Insert Reccursevly into DB\n");
    printf(" 3 - search from files\n");
    printf(" 0 - Exit\n\n");


    do
    {
        std::cin>>op;
        switch (op)
        {
            case 1:
                std::cin>>path;
                insertFileToDatabase(conn, path);
                break;
            case 2:
                std::cin>>path;
                insertRec(conn, path);
                break;
            case 3:
                std::string keyword;
                while(keyword.empty())
                    getline(std::cin, keyword);

                printSearchResults(conn, keyword);
                break;

        }
    }
    while (op!=0);
    return 0;
}