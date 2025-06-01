#include "Strategy.h"

std::string StandardProjectionStrategy::project(const std::string& image) {
    return image;
}

std::string HighContrastProjectionStrategy::project(const std::string& image) {
    return "HighContrast(" + image + ")";
}

std::string DefaultCaptureStrategy::capture(const std::string& scene) {
    return "Default capture of " + scene;
}

std::string LowLightCaptureStrategy::capture(const std::string& scene) {
    return "LowLight processing of " + scene;
}