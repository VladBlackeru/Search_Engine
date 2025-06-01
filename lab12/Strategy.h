#ifndef STRATEGY_H
#define STRATEGY_H

#include <string>

class ProjectionStrategy {
public:
    virtual std::string project(const std::string& image) = 0;
    virtual ~ProjectionStrategy() {}
};

class StandardProjectionStrategy : public ProjectionStrategy {
public:
    std::string project(const std::string& image) override;
};

class HighContrastProjectionStrategy : public ProjectionStrategy {
public:
    std::string project(const std::string& image) override;
};

class CaptureStrategy {
public:
    virtual std::string capture(const std::string& scene) = 0;
    virtual ~CaptureStrategy() {}
};

class DefaultCaptureStrategy : public CaptureStrategy {
public:
    std::string capture(const std::string& scene) override;
};

class LowLightCaptureStrategy : public CaptureStrategy {
public:
    std::string capture(const std::string& scene) override;
};

#endif // STRATEGY_H