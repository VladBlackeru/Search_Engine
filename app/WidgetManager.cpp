#include "WidgetManager.h"
#include <iostream>

WidgetManager::WidgetManager() {
    widgetMap["calculator"] = "[Calculator Widget]";
    widgetMap["weather"] = "[Weather Widget]";
    widgetMap["clock"] = "[Clock Widget]";
}

WidgetManager& WidgetManager::getInstance() {
    static WidgetManager instance;
    return instance;
}

void WidgetManager::displayRelevantWidget(const std::string& searchTerm) {
        if (widgetMap.find(searchTerm) != widgetMap.end()) {
            std::cout << "Widget: " << widgetMap[searchTerm] << "\n";
        }

}