#include "OrderBook.h"
#include "Event.h"
#include <iostream>

Order::Order(int orderId, int userId, OrderType orderType, int quantity, double price)
        : orderId(orderId), userId(userId), orderType(orderType), quantity(quantity), price(price), active(true) {}

Trade::Trade(int tradeId, int buyOrderId, int sellOrderId, int quantity, double price)
        : tradeId(tradeId), buyOrderId(buyOrderId), sellOrderId(sellOrderId), quantity(quantity), price(price) {}

void OrderBook::apply(const std::shared_ptr<Event>& event) {
    if (event->getType() == "OrderPlaced") {
        auto op = std::dynamic_pointer_cast<OrderPlaced>(event);
        orders.emplace(op->orderId, Order(op->orderId, op->userId, op->orderType, op->quantity, op->price));
    }
    else if (event->getType() == "OrderCancelled") {
        auto oc = std::dynamic_pointer_cast<OrderCancelled>(event);
        auto it = orders.find(oc->orderId);
        if (it != orders.end()) {
            it->second.active = false;
        }
    }
    else if (event->getType() == "TradeExecuted") {
        auto te = std::dynamic_pointer_cast<TradeExecuted>(event);
        trades.push_back(Trade(te->tradeId, te->buyOrderId, te->sellOrderId, te->quantity, te->price));
        auto itBuy = orders.find(te->buyOrderId);
        if (itBuy != orders.end()) {
            itBuy->second.quantity -= te->quantity;
            if (itBuy->second.quantity <= 0)
                itBuy->second.active = false;
        }
        auto itSell = orders.find(te->sellOrderId);
        if (itSell != orders.end()) {
            itSell->second.quantity -= te->quantity;
            if (itSell->second.quantity <= 0)
                itSell->second.active = false;
        }
    }
}

void OrderBook::replay(const std::vector<std::shared_ptr<Event>>& events) {
    for (const auto& event : events) {
        apply(event);
    }
}

void OrderBook::printOrderBook() const {
    std::cout << "Order Book:\n";
    for (const auto& pair : orders) {
        const Order& order = pair.second;
        std::cout << "OrderID: " << order.orderId
                  << " | User: " << order.userId
                  << " | Type: " << orderTypeToString(order.orderType)
                  << " | Quantity: " << order.quantity
                  << " | Price: " << order.price
                  << " | Active: " << (order.active ? "Yes" : "No") << "\n";
    }
    std::cout << "\nTrade History:\n";
    for (const auto& trade : trades) {
        std::cout << "TradeID: " << trade.tradeId
                  << " | BuyOrderID: " << trade.buyOrderId
                  << " | SellOrderID: " << trade.sellOrderId
                  << " | Quantity: " << trade.quantity
                  << " | Price: " << trade.price << "\n";
    }
}