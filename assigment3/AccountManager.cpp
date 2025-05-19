#include "AccountManager.h"
#include "Event.h"
#include <iostream>

void AccountManager::apply(const std::shared_ptr<Event>& event) {
    if (event->getType() == "FundsDebited") {
        auto fd = std::dynamic_pointer_cast<FundsDebited>(event);
        balances[fd->userId] -= fd->amount;
    }
    else if (event->getType() == "FundsCredited") {
        auto fc = std::dynamic_pointer_cast<FundsCredited>(event);
        balances[fc->userId] += fc->amount;
    }
}

void AccountManager::replay(const std::vector<std::shared_ptr<Event>>& events) {
    for (const auto& event : events) {
        apply(event);
    }
}

void AccountManager::printBalances() const {
    std::cout << "\nAccount Balances:\n";
    for (const auto& pair : balances) {
        std::cout << "User " << pair.first << ": " << pair.second << "\n";
    }
}