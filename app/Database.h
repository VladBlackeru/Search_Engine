//
// Created by Vlod on 05/11/2025.
//

#ifndef APP_DATABASE_H
#define APP_DATABASE_H


#ifndef DATABASE_H
#define DATABASE_H

#include <libpq-fe.h>
#include <string>

class Database {
public:
    explicit Database(const std::string& connectionInfo);
    ~Database();

    PGconn* getConnection() const;

    bool isConnected() const;

    bool executeQuery(const std::string& query);

    PGresult* executeParameterizedQuery(const std::string& query, int nParams, const char* const* paramValues);

private:
    PGconn* conn;
};

#endif // DATABASE_H

#endif //APP_DATABASE_H
