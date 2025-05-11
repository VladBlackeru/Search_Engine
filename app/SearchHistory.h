//
// Created by Vlod on 05/12/2025.
//

#ifndef APP_SEARCHHISTORY_H
#define APP_SEARCHHISTORY_H

#include "SearchObserver.h"
#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <algorithm>

class SearchHistory : public SearchObserver {
public:
    void update(const std::string &query) override;
    void printHistory() const;
    void suggestQueries() const;
private:
    std::vector<std::string> history;
};



#endif //APP_SEARCHHISTORY_H
