#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include <unordered_map>
#include <vector>
#include <memory>
#include "Event.h"

struct Order {
    int orderId;
    int userId;
    OrderType orderType;
    int quantity;
    double price;
    bool active;

    Order(int orderId, int userId, OrderType orderType, int quantity, double price);
};

struct Trade {
    int tradeId;
    int buyOrderId;
    int sellOrderId;
    int quantity;
    double price;

    Trade(int tradeId, int buyOrderId, int sellOrderId, int quantity, double price);
};

class OrderBook {
private:
    std::unordered_map<int, Order> orders;
    std::vector<Trade> trades;
public:
    void apply(const std::shared_ptr<Event>& event);
    void replay(const std::vector<std::shared_ptr<Event>>& events);
    void printOrderBook() const;
};

#endif // ORDERBOOK_H