#include <iostream>
#include <string>

class Button {
public:
    virtual void paint() = 0;
    virtual ~Button() {}
};

class Checkbox {
public:
    virtual void paint() = 0;
    virtual ~Checkbox() {}
};

class WindowsButton : public Button {
public:
    void paint() override {
        std::cout << "Rendering a button in Windows style.\n";
    }
};

class WindowsCheckbox : public Checkbox {
public:
    void paint() override {
        std::cout << "Rendering a checkbox in Windows style.\n";
    }
};

class MacButton : public Button {
public:
    void paint() override {
        std::cout << "Rendering a button in Mac style.\n";
    }
};

class MacCheckbox : public Checkbox {
public:
    void paint() override {
        std::cout << "Rendering a checkbox in Mac style.\n";
    }
};

class GUIFactory {
public:
    virtual Button*   createButton()   = 0;
    virtual Checkbox* createCheckbox() = 0;
    virtual ~GUIFactory() {}
};

class WindowsFactory : public GUIFactory {
public:
    Button*   createButton()   override { return new WindowsButton(); }
    Checkbox* createCheckbox() override { return new WindowsCheckbox(); }
};

class MacFactory : public GUIFactory {
public:
    Button*   createButton()   override { return new MacButton(); }
    Checkbox* createCheckbox() override { return new MacCheckbox(); }
};

void renderUI(GUIFactory* factory) {
    Button* btn = factory->createButton();
    Checkbox* cb = factory->createCheckbox();

    btn->paint();
    cb->paint();

    delete btn;
    delete cb;
}

int main() {
    std::cout << "Enter OS type (Windows/Mac): ";
    std::string osType;
    std::cin >> osType;

    GUIFactory* factory = nullptr;
    if (osType == "Windows") {
        factory = new WindowsFactory();
    } else if (osType == "Mac") {
        factory = new MacFactory();
    } else {
        std::cout << "Unknown OS type. Defaulting to Windows.\n";
        factory = new WindowsFactory();
    }

    renderUI(factory);

    delete factory;
    return 0;
}
