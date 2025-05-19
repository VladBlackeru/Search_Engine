#ifndef ACCOUNT_MANAGER_H
#define ACCOUNT_MANAGER_H

#include <unordered_map>
#include <memory>
#include <vector>
#include "Event.h"

class AccountManager {
private:
    std::unordered_map<int, double> balances;
public:
    void apply(const std::shared_ptr<Event>& event);
    void replay(const std::vector<std::shared_ptr<Event>>& events);
    void printBalances() const;
};

#endif // ACCOUNT_MANAGER_H