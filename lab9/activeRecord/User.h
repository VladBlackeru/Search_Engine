#ifndef USER_ACTIVE_RECORD_H
#define USER_ACTIVE_RECORD_H

#include "Database.h"
#include <string>
#include <vector>
#include <memory>

class User {
public:
    User();
    User(const std::string& name, const std::string& email);

    static std::unique_ptr<User> findById(Database& db, int id);

    static std::vector<std::unique_ptr<User>> findAll(Database& db);

    bool save(Database& db);

    bool remove(Database& db);

    int getId() const;
    const std::string& getName() const;
    void setName(const std::string& newName);
    const std::string& getEmail() const;
    void setEmail(const std::string& newEmail);

private:
    int id_;
    std::string name_;
    std::string email_;
};

#endif // USER_ACTIVE_RECORD_H
