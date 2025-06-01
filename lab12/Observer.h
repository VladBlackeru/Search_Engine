#ifndef OBSERVER_H
#define OBSERVER_H

#include <string>
#include <vector>
#include <algorithm>

class Observer {
public:
    virtual void update(const std::string& message) = 0;
    virtual ~Observer() {}
};

class Subject {
protected:
    std::vector<Observer*> observers;
public:
    void attach(Observer* o);
    void detach(Observer* o);
    void notify(const std::string& message);
};

#endif // OBSERVER_H