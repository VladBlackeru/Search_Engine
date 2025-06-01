#include "FileRepository.h"
#include "QueryParser.h"
#include "Ranking.h"
#include "Utils.h"
#include "WidgetManager.h"
#include "SearchCache.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <libpq-fe.h>
#include <string>
#include <algorithm>

bool FileRepository::deleteFileByPath(const std::string &filePath) {
    if (!db.isConnected()) {
        std::cerr << "Database connection is not valid: "
                  << PQerrorMessage(db.getConnection()) << std::endl;
        return false;
    }

    std::string query = "DELETE FROM search_engine.files WHERE path = $1;";
    const char* params[1] = { filePath.c_str() };
    PGresult* res = db.executeParameterizedQuery(query, 1, params);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "Delete operation failed: "
                  << PQerrorMessage(db.getConnection()) << std::endl;
        PQclear(res);
        return false;
    }
    PQclear(res);
    return true;
}

FileRepository::FileRepository(Database &db) : db(db) {}

void FileRepository::deleteTableContents(const std::string& tableName) {
    if (!db.isConnected()) {
        std::cerr << "Database connection is not valid: "
                  << PQerrorMessage(db.getConnection()) << std::endl;
        return;
    }

    std::string query = "DELETE FROM " + tableName + ";";
    bool success = db.executeQuery(query);
    if (success) {
        std::cerr << "Contents of table '" << tableName
                  << "' deleted successfully." << std::endl;
    } else {
        std::cerr << "Failed to delete contents of table '" << tableName << "'."
                  << std::endl;
    }
}

bool FileRepository::insertFile(const std::string& filePath) {
    if (!db.isConnected()) {
        std::cerr << "Database connection is not valid: "
                  << PQerrorMessage(db.getConnection()) << std::endl;
        return false;
    }

    std::ifstream file(filePath);
    if (!file) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return false;
    }

    std::filesystem::path p(filePath);
    std::string fileName = p.filename().string();
    std::string fileExtension = p.extension().string();
    auto fileSize = std::filesystem::file_size(filePath);

    // Insert file metadata.
    const char* fileSql =
            "INSERT INTO search_engine.files (path, name, extension, size) "
            "VALUES ($1, $2, $3, $4) RETURNING id;";
    std::string fileSizeStr = std::to_string(fileSize);
    const char* fileParams[4] = {
            filePath.c_str(),
            fileName.c_str(),
            fileExtension.c_str(),
            fileSizeStr.c_str()
    };

    PGresult* fileRes = db.executeParameterizedQuery(fileSql, 4, fileParams);
    if (PQresultStatus(fileRes) != PGRES_TUPLES_OK) {
        std::cerr << filePath << "\nFile metadata insertion failed: "
                  << PQerrorMessage(db.getConnection()) << std::endl;
        PQclear(fileRes);
        return false;
    }

    // Obtain the generated fileID.
    int fileId = std::stoi(PQgetvalue(fileRes, 0, 0));
    PQclear(fileRes);

    // Insert file contents line-by-line.
    std::string line;
    int line_number = 1;
    while (std::getline(file, line)) {
        const char* contentSql =
                "INSERT INTO search_engine.file_contents (filesid, content, line_number) "
                "VALUES ($1, $2, $3);";
        std::string fileIdStr = std::to_string(fileId);
        std::string lineNumStr = std::to_string(line_number);
        const char* contentParams[3] = { fileIdStr.c_str(), line.c_str(), lineNumStr.c_str() };
        PGresult* contentRes = db.executeParameterizedQuery(contentSql, 3, contentParams);
        if (PQresultStatus(contentRes) != PGRES_COMMAND_OK) {
            std::cerr << fileName << "\nFile content insertion failed: "
                      << PQerrorMessage(db.getConnection()) << std::endl;
            PQclear(contentRes);
            // Roll back metadata insertion if content insertion fails.
            deleteFileByPath(filePath);
            return false;
        }
        PQclear(contentRes);
        ++line_number;
    }
    file.close();
    return true;
}

void FileRepository::searchFiles(const std::string &searchPhrase) {
    if (!db.isConnected()) {
        std::cerr << "Database connection is not valid: "
                  << PQerrorMessage(db.getConnection()) << std::endl;
        return;
    }

    const char* query = R"(
    WITH full_text_search AS (
        SELECT fc.*, f.name AS file_name, f.path AS file_path
        FROM search_engine.file_contents fc
        JOIN search_engine.files f ON fc.filesid = f.id
        WHERE fc.content_tsvector @@ phraseto_tsquery('english', $1)
    ),
    partial_word_match AS (
        SELECT fc.*, f.name AS file_name, f.path AS file_path
        FROM search_engine.file_contents fc
        JOIN search_engine.files f ON fc.filesid = f.id
        WHERE fc.content LIKE '%' || $1 || '%'
    )
    SELECT DISTINCT content, line_number, file_name, file_path
    FROM full_text_search
    UNION
    SELECT DISTINCT content, line_number, file_name, file_path
    FROM partial_word_match;
    )";

    const char* paramValues[1] = { searchPhrase.c_str() };

    PGresult* res = db.executeParameterizedQuery(query, 1, paramValues);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cerr << "Search query failed: "
                  << PQerrorMessage(db.getConnection()) << std::endl;
        PQclear(res);
        return;
    }

    int nRows = PQntuples(res);
    int nCols = PQnfields(res);
    std::cout << "Search results for: '" << searchPhrase << "':" << std::endl;
    for (int i = 0; i < nRows; ++i) {
        for (int j = 0; j < nCols; ++j) {
            std::cout << PQfname(res, j) << ": "
                      << PQgetvalue(res, i, j) << "\t";
        }
        std::cout << std::endl;
    }
    PQclear(res);
}

void FileRepository::insertRecursively(const std::string &directoryPath) {
    try {
        if (!std::filesystem::exists(directoryPath) ||
            !std::filesystem::is_directory(directoryPath)) {
            std::cerr << "Invalid directory path: " << directoryPath << std::endl;
            return;
        }
        for (const auto& entry : std::filesystem::recursive_directory_iterator(directoryPath)) {
            if (std::filesystem::is_regular_file(entry)) {
                // Use our utility function to convert any backslashes.
                std::string filePath = replaceBackslashes(entry.path().string());
                if (insertFile(filePath)) {
                    std::cout << "File " << filePath
                              << " successfully inserted into the DB" << std::endl;
                } else {
                    std::cout << "File " << filePath
                              << " failed to insert into DB" << std::endl;
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error while traversing directory: " << e.what() << std::endl;
    }
}

void FileRepository::searchFilesAdvanced(const std::string &queryStr) {
    if (!db.isConnected()) {
        std::cerr << "Database connection is not valid: "
                  << PQerrorMessage(db.getConnection()) << std::endl;
        return;
    }

    notifySearchObservers(queryStr);

    QueryTerms terms = QueryParser::parse(queryStr);

    const char* query = R"(
WITH full_text_search AS (
    SELECT fc.content, fc.line_number,
           f.name AS file_name, f.path AS file_path,
           f.size, f.extension
    FROM search_engine.file_contents fc
    JOIN search_engine.files f ON fc.filesid = f.id
    WHERE fc.content_tsvector @@ phraseto_tsquery('english', $1)
      AND f.path LIKE '%' || $2 || '%'
),
partial_word_match AS (
    SELECT fc.content, fc.line_number,
           f.name AS file_name, f.path AS file_path,
           f.size, f.extension
    FROM search_engine.file_contents fc
    JOIN search_engine.files f ON fc.filesid = f.id
    WHERE fc.content LIKE '%' || $1 || '%'
      AND f.path LIKE '%' || $2 || '%'
)
SELECT DISTINCT content, line_number, file_name, file_path, size, extension
FROM full_text_search
UNION
SELECT DISTINCT content, line_number, file_name, file_path, size, extension
FROM partial_word_match;
)";
    std::string contents="";
    for(auto s: terms.contentTerms){
        contents += s;
        contents += " ";
    }
    contents.pop_back();
    std::cout<<contents;
    const char* paramValues[2] = {
            contents.c_str(),
            terms.pathTerms[0].c_str()
    };
    WidgetManager::getInstance().displayRelevantWidget(contents.c_str());
    SearchCache& cache = SearchCache::getInstance();
    std::vector<SearchResult> results;

    if (cache.hasCachedResult(contents)) {
        std::cout << "Cache hit! Using stored search results.\n";
        results = cache.getCachedResult(contents);
        results = cache.getCachedResult(contents);
        results = cache.getCachedResult(contents);
    }
    else {
        PGresult *res = db.executeParameterizedQuery(query, 2, paramValues);

        if (PQresultStatus(res) != PGRES_TUPLES_OK) {
            std::cerr << "Advanced search query failed: "
                      << PQerrorMessage(db.getConnection()) << std::endl;
            PQclear(res);
            return;
        }

        int nRows = PQntuples(res);

        for (int i = 0; i < nRows; ++i) {
            SearchResult result;
            result.content = PQgetvalue(res, i, PQfnumber(res, "content"));
            result.line_number = std::stoi(PQgetvalue(res, i, PQfnumber(res, "line_number")));
            result.file_name = PQgetvalue(res, i, PQfnumber(res, "file_name"));
            result.file_path = PQgetvalue(res, i, PQfnumber(res, "file_path"));
            result.file_size = std::stol(PQgetvalue(res, i, PQfnumber(res, "size")));
            result.file_extension = PQgetvalue(res, i, PQfnumber(res, "extension"));

            result.score = Ranking::computeScore(result, terms);
            results.push_back(result);
        }
        cache.storeResult(contents, results);
        PQclear(res);
    }
    std::sort(results.begin(), results.end(), [](const SearchResult &a, const SearchResult &b) {
        return a.score > b.score;
    });

    std::cout << "Advanced Search results for query: '" << queryStr << "'\n";
    for (const auto &r : results) {
        std::cout << "Score: " << r.score << " | File: " << r.file_name
                  << " | Path: " << r.file_path
                  << " | Line: " << r.line_number << "\nContent: " << r.content
                  << "\n-------------------------------------\n";
    }
}


void FileRepository::registerSearchObserver(SearchObserver* observer) {
    observers.push_back(observer);
}

void FileRepository::removeSearchObserver(SearchObserver* observer) {
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}

void FileRepository::notifySearchObservers(const std::string& query) {
    for (auto observer : observers) {
        observer->update(query);
    }
}