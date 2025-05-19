#include "Event.h"

OrderPlaced::OrderPlaced(int orderId, int userId, OrderType orderType, int quantity, double price)
        : orderId(orderId), userId(userId), orderType(orderType), quantity(quantity), price(price) {}

std::string OrderPlaced::getType() const {
    return "OrderPlaced";
}

OrderCancelled::OrderCancelled(int orderId)
        : orderId(orderId) {}

std::string OrderCancelled::getType() const {
    return "OrderCancelled";
}

TradeExecuted::TradeExecuted(int tradeId, int buyOrderId, int sellOrderId, int quantity, double price)
        : tradeId(tradeId), buyOrderId(buyOrderId), sellOrderId(sellOrderId), quantity(quantity), price(price) {}

std::string TradeExecuted::getType() const {
    return "TradeExecuted";
}

FundsDebited::FundsDebited(int userId, double amount)
        : userId(userId), amount(amount) {}

std::string FundsDebited::getType() const {
    return "FundsDebited";
}

FundsCredited::FundsCredited(int userId, double amount)
        : userId(userId), amount(amount) {}

std::string FundsCredited::getType() const {
    return "FundsCredited";
}