#include "Projector.h"

Projector::Projector(const std::string& name, ProjectionStrategy* strat)
        : name(name), isOn(false), strategy(strat) {}

void Projector::turnOn() {
    isOn = true;
    std::cout << "[Projector " << name << "] turned on" << std::endl;
    notify("Projector " + name + " is now ON");
}

void Projector::turnOff() {
    isOn = false;
    std::cout << "[Projector " << name << "] turned off" << std::endl;
    notify("Projector " + name + " is now OFF");
}

void Projector::changeStrategy(ProjectionStrategy* newStrategy) {
    strategy = newStrategy;
    std::cout << "[Projector " << name << "] changed strategy to "
              << typeid(*newStrategy).name() << std::endl;
}

std::string Projector::project(const std::string& image) {
    if (isOn) {
        std::string result = strategy->project(image);
        std::cout << "[Projector " << name << "] projecting: " << result << std::endl;
        return result;
    }
    else {
        std::cout << "[Projector " << name << "] is off and cannot project." << std::endl;
        return "";
    }
}