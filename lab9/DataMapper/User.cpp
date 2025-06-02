#include "User.h"

User::User() : id_(0), name_(), email_() {}

User::User(int id, const std::string& name, const std::string& email)
        : id_(id), name_(name), email_(email) {}

int User::getId() const {
    return id_;
}

void User::setId(int id) {
    id_ = id;
}

const std::string& User::getName() const {
    return name_;
}

void User::setName(const std::string& name) {
    name_ = name;
}

const std::string& User::getEmail() const {
    return email_;
}

void User::setEmail(const std::string& email) {
    email_ = email;
}
