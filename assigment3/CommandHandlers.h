#ifndef COMMAND_HANDLERS_H
#define COMMAND_HANDLERS_H

#include "EventStore.h"
#include "Event.h"

namespace CommandHandlers {
    void placeOrder(EventStore& store, int orderId, int userId, OrderType orderType, int quantity, double price);
    void cancelOrder(EventStore& store, int orderId);
    void executeTrade(EventStore& store, int tradeId, int buyOrderId, int sellOrderId, int quantity, double price);
    void debitFunds(EventStore& store, int userId, double amount);
    void creditFunds(EventStore& store, int userId, double amount);
}

#endif // COMMAND_HANDLERS_H