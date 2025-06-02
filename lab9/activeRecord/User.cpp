#include "User.h"
#include <libpq-fe.h>
#include <iostream>

User::User() : id_(0), name_(), email_() {}

User::User(const std::string& name, const std::string& email)
        : id_(0), name_(name), email_(email) {}

int User::getId() const {
    return id_;
}

const std::string& User::getName() const {
    return name_;
}

void User::setName(const std::string& newName) {
    name_ = newName;
}

const std::string& User::getEmail() const {
    return email_;
}

void User::setEmail(const std::string& newEmail) {
    email_ = newEmail;
}

std::unique_ptr<User> User::findById(Database& db, int id) {
    if (!db.isConnected()) {
        std::cerr << "Database not connected in findById\n";
        return nullptr;
    }

    const std::string sql = "SELECT id, name, email FROM users WHERE id = $1;";
    const char* params[1];
    std::string idStr = std::to_string(id);
    params[0] = idStr.c_str();

    PGresult* res = db.executeParameterizedQuery(sql, 1, params);
    if (!res) {
        std::cerr << "findById: QUERY FAILED\n";
        return nullptr;
    }

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cerr << "findById: No tuples returned or error\n";
        PQclear(res);
        return nullptr;
    }

    if (PQntuples(res) == 0) {
        PQclear(res);
        return nullptr;
    }

    char* id_c = PQgetvalue(res, 0, 0);
    char* name_c = PQgetvalue(res, 0, 1);
    char* email_c = PQgetvalue(res, 0, 2);

    auto user = std::make_unique<User>();
    user->id_ = std::atoi(id_c);
    user->name_ = name_c;
    user->email_ = email_c;

    PQclear(res);
    return user;
}

std::vector<std::unique_ptr<User>> User::findAll(Database& db) {
    std::vector<std::unique_ptr<User>> list;
    if (!db.isConnected()) {
        std::cerr << "Database not connected in findAll\n";
        return list;
    }

    const std::string sql = "SELECT id, name, email FROM users ORDER BY id;";
    PGresult* res = PQexec(db.getConnection(), sql.c_str());
    if (!res) {
        std::cerr << "findAll: QUERY FAILED\n";
        return list;
    }
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cerr << "findAll: No tuples returned or error\n";
        PQclear(res);
        return list;
    }

    int rows = PQntuples(res);
    for (int i = 0; i < rows; ++i) {
        auto user = std::make_unique<User>();
        user->id_ = std::atoi(PQgetvalue(res, i, 0));
        user->name_ = PQgetvalue(res, i, 1);
        user->email_ = PQgetvalue(res, i, 2);
        list.push_back(std::move(user));
    }

    PQclear(res);
    return list;
}

bool User::save(Database& db) {
    if (!db.isConnected()) {
        std::cerr << "Database not connected in save()\n";
        return false;
    }

    if (id_ == 0) {
        const std::string sql =
                "INSERT INTO users (name, email) "
                "VALUES ($1, $2) "
                "RETURNING id;";
        const char* params[2];
        params[0] = name_.c_str();
        params[1] = email_.c_str();

        PGresult* res = db.executeParameterizedQuery(sql, 2, params);
        if (!res) {
            std::cerr << "save(): INSERT failed\n";
            return false;
        }

        if (PQresultStatus(res) != PGRES_TUPLES_OK) {
            std::cerr << "save(): INSERT did not return new id\n";
            PQclear(res);
            return false;
        }
        char* newIdStr = PQgetvalue(res, 0, 0);
        id_ = std::atoi(newIdStr);
        PQclear(res);
        return true;
    } else {
        const std::string sql =
                "UPDATE users "
                "SET name = $1, email = $2 "
                "WHERE id = $3;";
        const char* params[3];
        params[0] = name_.c_str();
        params[1] = email_.c_str();
        std::string idStr = std::to_string(id_);
        params[2] = idStr.c_str();

        PGresult* res = db.executeParameterizedQuery(sql, 3, params);
        if (!res) {
            std::cerr << "save(): UPDATE failed\n";
            return false;
        }
        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            std::cerr << "save(): UPDATE did not succeed\n";
            PQclear(res);
            return false;
        }
        PQclear(res);
        return true;
    }
}

bool User::remove(Database& db) {
    if (!db.isConnected()) {
        std::cerr << "Database not connected in remove()\n";
        return false;
    }
    if (id_ == 0) {
        return false;
    }

    const std::string sql = "DELETE FROM users WHERE id = $1;";
    std::string idStr = std::to_string(id_);
    const char* params[1] = { idStr.c_str() };

    PGresult* res = db.executeParameterizedQuery(sql, 1, params);
    if (!res) {
        std::cerr << "remove(): DELETE failed\n";
        return false;
    }
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "remove(): DELETE did not succeed\n";
        PQclear(res);
        return false;
    }
    PQclear(res);
    id_ = 0;
    return true;
}
