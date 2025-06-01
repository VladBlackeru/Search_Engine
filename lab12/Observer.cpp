#include "Observer.h"

void Subject::attach(Observer* o) {
    observers.push_back(o);
}

void Subject::detach(Observer* o) {
    observers.erase(std::remove(observers.begin(), observers.end(), o), observers.end());
}

void Subject::notify(const std::string& message) {
    for (auto observer : observers) {
        observer->update(message);
    }
}