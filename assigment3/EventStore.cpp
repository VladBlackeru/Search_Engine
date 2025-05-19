#include "EventStore.h"

void EventStore::append(std::shared_ptr<Event> event) {
    events.push_back(event);
}

const std::vector<std::shared_ptr<Event>>& EventStore::get_all_events() const {
    return events;
}