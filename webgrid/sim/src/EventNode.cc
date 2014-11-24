#include "EventNode.hh"

EventNode::EventNode (EventListener* src, EventListener* dest, double time,
                      int eventType, int priority, vector<double> attrib,
                      void* data) {
    this->src = src;
    this->dest = dest;
    this->time = time;
    this->eventType = eventType;
    this->priority = priority;
    this->attrib = attrib;
    this->data = data;
}

EventNode::EventNode (EventListener* src, EventListener* dest, double time,
                      int eventType, int priority,  void* data) {
    this->src = src;
    this->dest = dest;
    this->time = time;
    this->eventType = eventType;
    this->priority = priority;
    this->attrib = vector<double> (0);
    this->data = data;
}

EventNode::~EventNode() {
}
