#include <iostream>
#include <string>
#include "Projector.h"
#include "Camera.h"
#include "Strategy.h"
#include "Command.h"

int main() {
    ProjectionStrategy* standardStrategy = new StandardProjectionStrategy();
    ProjectionStrategy* highContrastStrategy = new HighContrastProjectionStrategy();

    CaptureStrategy* defaultCapture = new DefaultCaptureStrategy();
    CaptureStrategy* lowLightCapture = new LowLightCaptureStrategy();

    Projector* projector = new Projector("MainProjector", standardStrategy);
    Camera* camera = new Camera("MainCamera", defaultCapture);

    projector->attach(camera);

    Command* turnOn = new TurnOnCommand(projector);
    Command* turnOff = new TurnOffCommand(projector);
    Command* capture = new CaptureImageCommand(camera);

    CommandInvoker invoker;
    invoker.registerCommand("turn_on", turnOn);
    invoker.registerCommand("capture", capture);
    invoker.registerCommand("turn_off", turnOff);

    std::cout << "\n--- Simulating ---\n" << std::endl;

    invoker.executeCommand("turn_on");
    std::string image = "Sample Image Data";
    projector->project(image);
    invoker.executeCommand("capture");

    projector->changeStrategy(highContrastStrategy);
    projector->project(image);

    invoker.executeCommand("turn_off");

    delete turnOn;
    delete turnOff;
    delete capture;
    delete projector;
    delete camera;
    delete standardStrategy;
    delete highContrastStrategy;
    delete defaultCapture;
    delete lowLightCapture;

    return 0;
}
