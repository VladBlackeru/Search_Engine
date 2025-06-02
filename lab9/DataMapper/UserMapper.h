#ifndef USER_MAPPER_H
#define USER_MAPPER_H

#include "Database.h"
#include "User.h"
#include <vector>
#include <memory>

// handles all sql
class UserMapper {
public:
    explicit UserMapper(Database& db);

    std::unique_ptr<User> findById(int id);

    std::vector<std::unique_ptr<User>> findAll();

    bool insert(User& user);

    bool update(const User& user);

    bool remove(User& user);

private:
    Database& db_;
};

#endif // USER_MAPPER_H
