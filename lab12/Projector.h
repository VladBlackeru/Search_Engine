#ifndef PROJECTOR_H
#define PROJECTOR_H

#include <string>
#include "Observer.h"
#include "Strategy.h"
#include <iostream>
#include <typeinfo>

class Projector : public Subject {
private:
    std::string name;
    bool isOn;
    ProjectionStrategy* strategy;
public:
    Projector(const std::string& name, ProjectionStrategy* strat);
    void turnOn();
    void turnOff();
    void changeStrategy(ProjectionStrategy* newStrategy);
    std::string project(const std::string& image);
};

#endif // PROJECTOR_H