#include "EventAttribute.hh"

EventAttribute::EventAttribute (int eventType, string eventName) {
    this->eventType = eventType;
    this->eventName = eventName;
    eventCount = 0;
    listeners.clear();
}

EventAttribute::~EventAttribute () {
}

bool EventAttribute::addEventListener (EventListener* listener) {
    bool found = false;

    for (unsigned int i = 0; !found && (i < listeners.size()); ++i) {
        if (listeners[i] == listener)   found = true;
    }

    if (!found) listeners.push_back(listener);

    return !found;
}

bool EventAttribute::removeEventListener (EventListener* listener) {
    vector<EventListener*>::iterator it;

    it = find(listeners.begin(), listeners.end(), listener);
    if (it != listeners.end()) {
        listeners.erase(it);
        return true;
    }
    return false;
}

void EventAttribute::clearEventListener () {
    listeners.clear();
}


void EventAttribute::init() {
    for (unsigned int i = 0; i < listeners.size(); ++i) {
        listeners[i]->init();
    }
}

void EventAttribute::process(EventNode* node) {
    for (unsigned int i = 0; i < listeners.size(); ++i) {
        listeners[i]->process (node);
    }
}

void EventAttribute::finalize() {
    for (unsigned int i = 0; i < listeners.size(); ++i) {
        listeners[i]->finalize();
    }
}
