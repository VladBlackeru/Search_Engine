#ifndef EVENT_H
#define EVENT_H

#include <string>

enum class OrderType {
    BUY,
    SELL
};

inline std::string orderTypeToString(OrderType t) {
    return (t == OrderType::BUY) ? "BUY" : "SELL";
}

class Event {
public:
    virtual ~Event() {}
    virtual std::string getType() const = 0;
};

class OrderPlaced : public Event {
public:
    int orderId;
    int userId;
    OrderType orderType;
    int quantity;
    double price;

    OrderPlaced(int orderId, int userId, OrderType orderType, int quantity, double price);

    std::string getType() const override;
};

// Event when an order is cancelled.
class OrderCancelled : public Event {
public:
    int orderId;

    OrderCancelled(int orderId);

    std::string getType() const override;
};

class TradeExecuted : public Event {
public:
    int tradeId;
    int buyOrderId;
    int sellOrderId;
    int quantity;
    double price;

    TradeExecuted(int tradeId, int buyOrderId, int sellOrderId, int quantity, double price);

    std::string getType() const override;
};

class FundsDebited : public Event {
public:
    int userId;
    double amount;

    FundsDebited(int userId, double amount);

    std::string getType() const override;
};

class FundsCredited : public Event {
public:
    int userId;
    double amount;

    FundsCredited(int userId, double amount);

    std::string getType() const override;
};

#endif // EVENT_H