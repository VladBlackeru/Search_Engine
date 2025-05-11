#include "TradingSystem.h"
#include "Event.h"
#include <iostream>

void TradingSystem::place_order(const std::string& user, const std::string& side, double price, int quantity) {
    int orderId = orderBook.size() + 1;
    OrderPlaced* event = new OrderPlaced(orderId, user, side, price, quantity);
    eventStore.append(event);
    orderBook.push_back(event);

    if (side == "buy" || side == "BUY") {
        debit_funds(user, price * quantity);
    }
}

void TradingSystem::cancel_order(int orderId) {
    OrderCancelled* event = new OrderCancelled(orderId);
    eventStore.append(event);
}

void TradingSystem::execute_trade(int buyOrderId, int sellOrderId, double price, int quantity) {
    TradeExecuted* event = new TradeExecuted(buyOrderId, sellOrderId, price, quantity);
    eventStore.append(event);
}

void TradingSystem::debit_funds(const std::string& user, double amount) {
    FundsDebited* event = new FundsDebited(user, amount);
    eventStore.append(event);
    balances[user] -= amount;
}

void TradingSystem::credit_funds(const std::string& user, double amount) {
    FundsCredited* event = new FundsCredited(user, amount);
    eventStore.append(event);
    balances[user] += amount;
}

void TradingSystem::replay_events() {
    std::cout << "\n----- Replayed Event Log -----" << std::endl;
    eventStore.replay();

    std::cout << "\n----- Account Balances -----" << std::endl;
    for (const auto& pair : balances) {
        std::cout << "User: " << pair.first << ", Balance: " << pair.second << std::endl;
    }

    std::cout << "\n----- Order Book -----" << std::endl;
    for (const auto &order : orderBook) {
        order->print();
    }
}