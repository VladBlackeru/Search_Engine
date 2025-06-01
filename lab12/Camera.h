#ifndef CAMERA_H
#define CAMERA_H

#include <string>
#include "Observer.h"
#include "Strategy.h"

class Camera : public Observer {
private:
    std::string name;
    CaptureStrategy* strategy;
public:
    Camera(const std::string& name, CaptureStrategy* strat);
    std::string capture(const std::string& scene);
    void update(const std::string& message) override;
};

#endif // CAMERA_H