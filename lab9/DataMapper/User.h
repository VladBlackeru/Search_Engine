#ifndef USER_DOMAIN_H
#define USER_DOMAIN_H

#include <string>

class User {
public:
    User();
    User(int id, const std::string& name, const std::string& email);

    int getId() const;
    void setId(int id);

    const std::string& getName() const;
    void setName(const std::string& name);

    const std::string& getEmail() const;
    void setEmail(const std::string& email);

private:
    int id_;
    std::string name_;
    std::string email_;
};

#endif // USER_DOMAIN_H
