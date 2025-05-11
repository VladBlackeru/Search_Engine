//
// Created by Vlod on 05/11/2025.
//

#ifndef ASSIG3_EVENTSTORE_H
#define ASSIG3_EVENTSTORE_H


#include "Event.h"
#include <vector>

class EventStore {
private:
    std::vector<Event*> events;

public:
    ~EventStore();
    void append(Event* event);
    const std::vector<Event*>& get_all_events() const;
    void replay() const;
};

#endif //ASSIG3_EVENTSTORE_H
