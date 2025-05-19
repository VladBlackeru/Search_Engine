#ifndef EVENT_STORE_H
#define EVENT_STORE_H

#include <vector>
#include <memory>
#include "Event.h"

class EventStore {
private:
    std::vector<std::shared_ptr<Event>> events;
public:
    void append(std::shared_ptr<Event> event);
    const std::vector<std::shared_ptr<Event>>& get_all_events() const;
};

#endif // EVENT_STORE_H