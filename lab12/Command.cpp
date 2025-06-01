#include "Command.h"
#include "Projector.h"
#include "Camera.h"
#include <iostream>

TurnOnCommand::TurnOnCommand(Projector* proj) : projector(proj) {}

void TurnOnCommand::execute() {
    projector->turnOn();
}

TurnOffCommand::TurnOffCommand(Projector* proj) : projector(proj) {}

void TurnOffCommand::execute() {
    projector->turnOff();
}

CaptureImageCommand::CaptureImageCommand(Camera* cam) : camera(cam) {}

void CaptureImageCommand::execute() {
    std::string scene = "Scene from projector";
    camera->capture(scene);
}

void CommandInvoker::registerCommand(const std::string& name, Command* cmd) {
    commands[name] = cmd;
}

void CommandInvoker::executeCommand(const std::string& name) {
    if (commands.find(name) != commands.end()) {
        commands[name]->execute();
    }
    else {
        std::cout << "Command '" << name << "' not recognized." << std::endl;
    }
}