#include "UserMapper.h"
#include <libpq-fe.h>
#include <iostream>

UserMapper::UserMapper(Database& db) : db_(db) {}

std::unique_ptr<User> UserMapper::findById(int id) {
    if (!db_.isConnected()) {
        std::cerr << "UserMapper::findById: DB not connected\n";
        return nullptr;
    }

    const std::string sql = "SELECT id, name, email FROM users WHERE id = $1;";
    std::string idStr = std::to_string(id);
    const char* params[1] = { idStr.c_str() };

    PGresult* res = db_.executeParameterizedQuery(sql, 1, params);
    if (!res || PQresultStatus(res) != PGRES_TUPLES_OK) {
        if (res) PQclear(res);
        std::cerr << "UserMapper::findById: query failed\n";
        return nullptr;
    }

    if (PQntuples(res) == 0) {
        PQclear(res);
        return nullptr;
    }

    int foundId = std::atoi(PQgetvalue(res, 0, 0));
    std::string name = PQgetvalue(res, 0, 1);
    std::string email = PQgetvalue(res, 0, 2);
    PQclear(res);

    return std::make_unique<User>(foundId, name, email);
}

std::vector<std::unique_ptr<User>> UserMapper::findAll() {
    std::vector<std::unique_ptr<User>> list;
    if (!db_.isConnected()) {
        std::cerr << "UserMapper::findAll: DB not connected\n";
        return list;
    }

    const std::string sql = "SELECT id, name, email FROM users ORDER BY id;";
    PGresult* res = PQexec(db_.getConnection(), sql.c_str());
    if (!res || PQresultStatus(res) != PGRES_TUPLES_OK) {
        if (res) PQclear(res);
        std::cerr << "UserMapper::findAll: query failed\n";
        return list;
    }

    int rows = PQntuples(res);
    for (int i = 0; i < rows; ++i) {
        int foundId = std::atoi(PQgetvalue(res, i, 0));
        std::string name = PQgetvalue(res, i, 1);
        std::string email = PQgetvalue(res, i, 2);
        list.push_back(std::make_unique<User>(foundId, name, email));
    }
    PQclear(res);
    return list;
}

bool UserMapper::insert(User& user) {
    if (!db_.isConnected()) {
        std::cerr << "UserMapper::insert: DB not connected\n";
        return false;
    }

    if (user.getId() != 0) {
        std::cerr << "UserMapper::insert: user already has an ID\n";
        return false;
    }

    const std::string sql =
            "INSERT INTO users (name, email) VALUES ($1, $2) RETURNING id;";
    const char* params[2] = { user.getName().c_str(), user.getEmail().c_str() };

    PGresult* res = db_.executeParameterizedQuery(sql, 2, params);
    if (!res || PQresultStatus(res) != PGRES_TUPLES_OK) {
        if (res) PQclear(res);
        std::cerr << "UserMapper::insert: INSERT failed\n";
        return false;
    }

    char* id_c = PQgetvalue(res, 0, 0);
    int newId = std::atoi(id_c);
    user.setId(newId);

    PQclear(res);
    return true;
}

// --- update ---
bool UserMapper::update(const User& user) {
    if (!db_.isConnected()) {
        std::cerr << "UserMapper::update: DB not connected\n";
        return false;
    }
    if (user.getId() == 0) {
        std::cerr << "UserMapper::update: user has no ID\n";
        return false;
    }

    const std::string sql =
            "UPDATE users SET name = $1, email = $2 WHERE id = $3;";
    std::string idStr = std::to_string(user.getId());
    const char* params[3] = {
            user.getName().c_str(),
            user.getEmail().c_str(),
            idStr.c_str()
    };

    PGresult* res = db_.executeParameterizedQuery(sql, 3, params);
    if (!res || PQresultStatus(res) != PGRES_COMMAND_OK) {
        if (res) PQclear(res);
        std::cerr << "UserMapper::update: UPDATE failed\n";
        return false;
    }
    PQclear(res);
    return true;
}

bool UserMapper::remove(User& user) {
    if (!db_.isConnected()) {
        std::cerr << "UserMapper::remove: DB not connected\n";
        return false;
    }
    if (user.getId() == 0) {
        std::cerr << "UserMapper::remove: user has no ID\n";
        return false;
    }

    const std::string sql = "DELETE FROM users WHERE id = $1;";
    std::string idStr = std::to_string(user.getId());
    const char* params[1] = { idStr.c_str() };

    PGresult* res = db_.executeParameterizedQuery(sql, 1, params);
    if (!res || PQresultStatus(res) != PGRES_COMMAND_OK) {
        if (res) PQclear(res);
        std::cerr << "UserMapper::remove: DELETE failed\n";
        return false;
    }
    PQclear(res);
    user.setId(0);
    return true;
}
