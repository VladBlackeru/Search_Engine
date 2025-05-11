
#include "EventStore.h"

EventStore::~EventStore() {
    for (auto event : events) {
        delete event;
    }
}

void EventStore::append(Event* event) {
    events.push_back(event);
}

const std::vector<Event*>& EventStore::get_all_events() const {
    return events;
}

void EventStore::replay() const {
    std::cout << "Replaying events...\n";
    for (const auto& event : events) {
        event->print();
    }
}