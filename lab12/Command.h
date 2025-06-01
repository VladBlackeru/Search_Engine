#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <unordered_map>

class Projector;
class Camera;

class Command {
public:
    virtual void execute() = 0;
    virtual ~Command() {}
};

class TurnOnCommand : public Command {
private:
    Projector* projector;
public:
    TurnOnCommand(Projector* proj);
    void execute() override;
};

class TurnOffCommand : public Command {
private:
    Projector* projector;
public:
    TurnOffCommand(Projector* proj);
    void execute() override;
};

class CaptureImageCommand : public Command {
private:
    Camera* camera;
public:
    CaptureImageCommand(Camera* cam);
    void execute() override;
};

class CommandInvoker {
private:
    std::unordered_map<std::string, Command*> commands;
public:
    void registerCommand(const std::string& name, Command* cmd);
    void executeCommand(const std::string& name);
    ~CommandInvoker() = default;
};

#endif // COMMAND_H