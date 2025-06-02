#include "Database.h"
#include <iostream>

Database::Database(const std::string& connectionInfo) {
    conn = PQconnectdb(connectionInfo.c_str());
    if (PQstatus(conn) != CONNECTION_OK) {
        std::cerr << "Connection to database failed: "
                  << PQerrorMessage(conn) << std::endl;
    }
}

Database::~Database() {
    if (conn) {
        PQfinish(conn);
    }
}

PGconn* Database::getConnection() const {
    return conn;
}

bool Database::isConnected() const {
    return PQstatus(conn) == CONNECTION_OK;
}

bool Database::executeQuery(const std::string& query) {
    PGresult* res = PQexec(conn, query.c_str());
    bool success = (PQresultStatus(res) == PGRES_COMMAND_OK);
    if (!success) {
        std::cerr << "Query failed: " << PQerrorMessage(conn) << std::endl;
    }
    PQclear(res);
    return success;
}

PGresult* Database::executeParameterizedQuery(const std::string& query,
                                              int nParams,
                                              const char* const* paramValues)
{
    PGresult* res = PQexecParams(conn, query.c_str(), nParams,
                                 nullptr, // let libpq deduce parameter types
                                 paramValues, nullptr, nullptr, 0);
    return res;
}