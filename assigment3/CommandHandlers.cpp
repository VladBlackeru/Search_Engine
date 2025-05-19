#include "CommandHandlers.h"
#include "Event.h"
#include <iostream>
#include <memory>

namespace CommandHandlers {

    void placeOrder(EventStore& store, int orderId, int userId, OrderType orderType, int quantity, double price) {
        std::cout << "Placing Order: " << orderId << "\n";
        store.append(std::make_shared<OrderPlaced>(orderId, userId, orderType, quantity, price));
    }

    void cancelOrder(EventStore& store, int orderId) {
        std::cout << "Cancelling Order: " << orderId << "\n";
        store.append(std::make_shared<OrderCancelled>(orderId));
    }

    void executeTrade(EventStore& store, int tradeId, int buyOrderId, int sellOrderId, int quantity, double price) {
        std::cout << "Executing Trade: " << tradeId << "\n";
        store.append(std::make_shared<TradeExecuted>(tradeId, buyOrderId, sellOrderId, quantity, price));
    }

    void debitFunds(EventStore& store, int userId, double amount) {
        std::cout << "Debiting Funds from User " << userId << " Amount: " << amount << "\n";
        store.append(std::make_shared<FundsDebited>(userId, amount));
    }

    void creditFunds(EventStore& store, int userId, double amount) {
        std::cout << "Crediting Funds to User " << userId << " Amount: " << amount << "\n";
        store.append(std::make_shared<FundsCredited>(userId, amount));
    }

} // namespace CommandHandlers