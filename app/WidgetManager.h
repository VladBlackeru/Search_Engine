#ifndef APP_WIDGETMANAGER_H
#define APP_WIDGETMANAGER_H

#include <string>
#include <vector>
#include <map>

class WidgetManager {
public:
    static WidgetManager& getInstance();
    void displayRelevantWidget(const std::string& searchTerm);
private:
    WidgetManager();
    std::map<std::string, std::string> widgetMap;
};

#endif //APP_WIDGETMANAGER_H
