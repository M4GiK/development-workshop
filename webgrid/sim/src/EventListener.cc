#include "EventListener.hh"

/* Static variables must be initialized, otherwise it won't compile */
EventHandler* EventListener::handler = NULL;

EventListener::EventListener(EventHandler* rOOt) {
    eventAttr.clear();
    lastEventTime = 0.0;
    enableTrace   = false;
    handler       = rOOt;
}

EventListener::EventListener () {
    eventAttr.clear();
    lastEventTime = 0.0;
    enableTrace   = false;
}

EventListener::~EventListener() {
    for (unsigned int i = 0; i < eventAttr.size(); ++i) {
        delete eventAttr[i];
    }
    traceFile.close();
}

void EventListener::setHandler(EventHandler* rOOt) {
    this->handler = rOOt;
}

void EventListener::setTrace(bool flag) {
    enableTrace = flag;
}

void EventListener::setTraceFile (string fileName) {
    if (enableTrace) {
        traceFile.open (fileName.c_str(), ios::app);  /* append mode */
        if (!traceFile) error("EventListener.cc: Can't open trace file");
    }
}

void EventListener::setTraceFile (char* fileName) {
    if (enableTrace) {
        traceFile.open (fileName, ios::app);          /* append mode */
        if (!traceFile) error("EventListener.cc: Can't open trace file");
    }
}

double EventListener::getLastEventTime() {
    return lastEventTime;
}

void EventListener::registerEvent (int eventType, string eventName) {
    EventAttribute* ea;

    if (int(eventAttr.size()) <= eventType) {
        eventAttr.resize(eventType+1, NULL);
    }
    if ((ea=eventAttr[eventType]) == NULL) {
        ea= eventAttr[eventType] = new EventAttribute(eventType,eventName);
    } else {
        ea->eventName = eventName;
        ea->clearEventListener();
    }
}

bool EventListener::addEventListener  (int eventType,
                                       EventListener* listener) {
    EventAttribute* ea;

    if (int(eventAttr.size())<=eventType||(ea=eventAttr[eventType])==NULL){
        return false;
    }
    return ea->addEventListener(listener);
}

bool EventListener::removeEventListener (int eventType,
                                         EventListener* listener) {
    EventAttribute* ea;

    if (int(eventAttr.size())<=eventType||(ea=eventAttr[eventType])==NULL){
        return false;
    }
    return ea->removeEventListener(listener);
}



void EventListener::scheduleEventNode(EventNode* event) {
    handler->scheduleEvent(event);
}

void EventListener::scheduleAt (double clock_value, int eventType,
                               int priority, vector<double> vd,void* data){
    handler->scheduleEvent ( new EventNode ( this, this, clock_value,
                             eventType, priority, vd, data )
                           );
}

void EventListener::scheduleAt (double clock_value, int eventType,
                                int priority, void* data) {
    handler->scheduleEvent ( new EventNode ( this, this, clock_value,
                             eventType, priority, data )
                           );
}

void EventListener::scheduleAt(EventListener* dest, double clock_value,
                              int eventType, int priority,
                              vector<double> vd, void* data) {
    handler->scheduleEvent ( new EventNode ( this, dest, clock_value,
                             eventType, priority, vd, data )
                           );
}

void EventListener::scheduleAt(EventListener* dest, double clock_value,
                              int eventType, int priority, void* data) {
    handler->scheduleEvent ( new EventNode ( this, dest, clock_value,
                             eventType, priority, data)
                           );
}

void EventListener::scheduleAfter(double clock_value, int eventType,
                                 int priority, vector<double> vd,
                                 void* data) {
    double simTime = handler->getSimTime();
    handler->scheduleEvent ( new EventNode ( this, this,
                             simTime + clock_value, eventType,
                             priority, vd, data)
                            );
}

void EventListener::scheduleAfter(double clock_value, int eventType,
                                 int priority, void* data) {
    double simTime = handler->getSimTime();
    handler->scheduleEvent ( new EventNode ( this, this,
                             simTime + clock_value, eventType,
                             priority, data)
                           );
}

void EventListener::scheduleAfter(EventListener* dest, double clock_value,
                                 int eventType, int priority,
                                 vector<double> vd,void* data) {
    double simTime = handler->getSimTime();
    handler->scheduleEvent ( new EventNode (this, dest,
                             simTime + clock_value, eventType,
                             priority, vd, data)
                            );
}

void EventListener::scheduleAfter(EventListener* dest, double clock_value,
                                 int eventType, int priority, void* data) {
    double simTime = handler->getSimTime();
    handler->scheduleEvent ( new EventNode(this, dest, simTime +
                             clock_value, eventType, priority, data)
                           );
}

int EventListener::cancelEvent(int eventType, vector<double> attrib,
                                bool allFlag) {
    return handler->deleteEvent ( eventType, attrib, allFlag );
}

int EventListener::cancelEvent(int eventType, bool allFlag){
    return handler->deleteEvent ( eventType, allFlag );
}



void EventListener::init() {
    for (unsigned int i = 0; i < eventAttr.size(); ++i) {
        if (eventAttr[i] != NULL) eventAttr[i]->init();
    }
    performInit();
}

void EventListener::performInit() {
}

bool EventListener::trace(EventNode* event) {
    int eventType = event->eventType;

    if (!enableTrace || event->dest != this) {
        return false;
    }
    eventAttr[eventType]->eventCount++;
    performTrace (event);
    return true;
}

void EventListener::performTrace (EventNode* event) {
}

void EventListener::process(EventNode* event) {
    lastEventTime = handler->getSimTime();
    performProcess(event);
    trace(event);
    eventAttr[event->eventType]->process(event);
}

void EventListener::performProcess(EventNode* event) {
}

void EventListener::finalize () {
    for (unsigned int i = 0; i < eventAttr.size(); ++i) {
        if (eventAttr[i] != NULL)  eventAttr[i]->finalize();
    }
    performFinalize();
}

void EventListener::performFinalize () {
}

void EventListener::_trace_ (char* str) {
    if (enableTrace) {
        traceFile << str;
    }
}

void EventListener::_trace_ (string str) {
    if (enableTrace) {
        traceFile << str;
    }
}

double EventListener::getSimulationTime() {
    return handler->getSimTime();
}
