#include "TradingSystem.h"
#include <iostream>
#include <string>

void print_menu() {
    std::cout << "\n===== Trading System Menu =====\n";
    std::cout << "1. Place Order\n";
    std::cout << "2. Cancel Order\n";
    std::cout << "3. Execute Trade\n";
    std::cout << "4. Debit Funds\n";
    std::cout << "5. Credit Funds\n";
    std::cout << "6. Replay Events\n";
    std::cout << "7. Exit\n";
    std::cout << "Enter your choice: ";
}

int main() {
    TradingSystem system;
    int choice;

    while (true) {
        print_menu();
        std::cin >> choice;

        if (choice == 7) {
            break;
        }

        switch (choice) {
            case 1: {
                std::string user, side;
                double price;
                int quantity;
                std::cout << "Enter user name: ";
                std::cin >> user;
                std::cout << "Enter side (buy/sell): ";
                std::cin >> side;
                std::cout << "Enter price: ";
                std::cin >> price;
                std::cout << "Enter quantity: ";
                std::cin >> quantity;
                system.place_order(user, side, price, quantity);
                break;
            }
            case 2: {
                int orderId;
                std::cout << "Enter order id to cancel: ";
                std::cin >> orderId;
                system.cancel_order(orderId);
                break;
            }
            case 3: {
                int buyOrderId, sellOrderId, quantity;
                double price;
                std::cout << "Enter buy order id: ";
                std::cin >> buyOrderId;
                std::cout << "Enter sell order id: ";
                std::cin >> sellOrderId;
                std::cout << "Enter trade price: ";
                std::cin >> price;
                std::cout << "Enter quantity traded: ";
                std::cin >> quantity;
                system.execute_trade(buyOrderId, sellOrderId, price, quantity);
                break;
            }
            case 4: {
                std::string user;
                double amount;
                std::cout << "Enter user name: ";
                std::cin >> user;
                std::cout << "Enter amount to debit: ";
                std::cin >> amount;
                system.debit_funds(user, amount);
                break;
            }
            case 5: {
                std::string user;
                double amount;
                std::cout << "Enter user name: ";
                std::cin >> user;
                std::cout << "Enter amount to credit: ";
                std::cin >> amount;
                system.credit_funds(user, amount);
                break;
            }
            case 6: {
                system.replay_events();
                break;
            }
            default:
                std::cout << "Invalid choice. Try again.\n";
                break;
        }
    }
    return 0;
}