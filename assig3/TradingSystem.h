//
// Created by Vlod on 05/11/2025.
//

#ifndef ASSIG3_TRADINGSYSTEM_H
#define ASSIG3_TRADINGSYSTEM_H


#include "EventStore.h"
#include <unordered_map>
#include <vector>

class TradingSystem {
private:
    EventStore eventStore;
    std::unordered_map<std::string, double> balances;
    std::vector<OrderPlaced*> orderBook;

public:
    void place_order(const std::string& user, const std::string& side, double price, int quantity);
    void cancel_order(int orderId);
    void execute_trade(int buyOrderId, int sellOrderId, double price, int quantity);
    void debit_funds(const std::string& user, double amount);
    void credit_funds(const std::string& user, double amount);
    void replay_events();
};

#endif //ASSIG3_TRADINGSYSTEM_H
