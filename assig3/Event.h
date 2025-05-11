//
// Created by Vlod on 05/11/2025.
//

#ifndef ASSIG3_EVENT_H
#define ASSIG3_EVENT_H

#include <iostream>
#include <string>

enum class EventType {
    OrderPlaced,
    OrderCancelled,
    TradeExecuted,
    FundsDebited,
    FundsCredited
};

class Event {
public:
    EventType type;
    virtual ~Event() = default;
    virtual void print() const = 0;
};

class OrderPlaced : public Event {
public:
    int orderId;
    std::string user;
    std::string side; // "buy" or "sell"
    double price;
    int quantity;

    OrderPlaced(int id, std::string u, std::string s, double p, int q);
    void print() const override;
};

class OrderCancelled : public Event {
public:
    int orderId;

    OrderCancelled(int id);
    void print() const override;
};

class TradeExecuted : public Event {
public:
    int buyOrderId, sellOrderId;
    double price;
    int quantity;

    TradeExecuted(int buyId, int sellId, double p, int q);
    void print() const override;
};

class FundsDebited : public Event {
public:
    std::string user;
    double amount;

    FundsDebited(std::string u, double a);
    void print() const override;
};

class FundsCredited : public Event {
public:
    std::string user;
    double amount;

    FundsCredited(std::string u, double a);
    void print() const override;
};



#endif //ASSIG3_EVENT_H
