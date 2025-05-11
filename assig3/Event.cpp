#include "Event.h"

OrderPlaced::OrderPlaced(int id, std::string u, std::string s, double p, int q)
        : orderId(id), user(std::move(u)), side(std::move(s)), price(p), quantity(q) {
    type = EventType::OrderPlaced;
}

void OrderPlaced::print() const {
    std::cout << "OrderPlaced: " << user << " " << side << " " << quantity << " @ " << price << "$\n";
}

OrderCancelled::OrderCancelled(int id) : orderId(id) {
    type = EventType::OrderCancelled;
}

void OrderCancelled::print() const {
    std::cout << "Order cancelled: Order ID " << orderId << "\n";
}

TradeExecuted::TradeExecuted(int buyId, int sellId, double p, int q)
        : buyOrderId(buyId), sellOrderId(sellId), price(p), quantity(q) {
    type = EventType::TradeExecuted;
}

void TradeExecuted::print() const {
    std::cout << "Trade executed: BuyOrder " << buyOrderId << " matched with SellOrder " << sellOrderId
              << " for " << quantity << " @ " << price << "\n";
}

FundsDebited::FundsDebited(std::string u, double a) : user(std::move(u)), amount(a) {
    type = EventType::FundsDebited;
}

void FundsDebited::print() const {
    std::cout << "Funds Debited: " << user << " debited " << amount << "\n";
}

FundsCredited::FundsCredited(std::string u, double a) : user(std::move(u)), amount(a) {
    type = EventType::FundsCredited;
}

void FundsCredited::print() const {
    std::cout << "Funds Credited: " << user << " credited " << amount << "\n";
}