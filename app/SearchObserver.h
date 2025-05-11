//
// Created by Vlod on 05/12/2025.
//

#ifndef APP_SEARCHOBSERVER_H
#define APP_SEARCHOBSERVER_H

#include <string>

// Interface
class SearchObserver {
public:
    virtual void update(const std::string &query) = 0;
    virtual ~SearchObserver() {}
};


#endif //APP_SEARCHOBSERVER_H
