#include "Camera.h"
#include <iostream>

Camera::Camera(const std::string& name, CaptureStrategy* strat)
        : name(name), strategy(strat) {}

std::string Camera::capture(const std::string& scene) {
    std::string result = strategy->capture(scene);
    std::cout << "[Camera " << name << "] captured: " << result << std::endl;
    return result;
}

void Camera::update(const std::string& message) {
    std::cout << "[Camera " << name << "] received update: " << message << std::endl;
}