#ifndef __EVENT_ATTRIBUTE_H__
#define __EVENT_ATTRIBUTE_H__
class EventAttribute;

#include "EventListener.hh"
#include "EventNode.hh"
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

class EventAttribute {
    friend class EventListener;
    public:
        EventAttribute(int eventType, string eventName);
        virtual ~EventAttribute();

        bool addEventListener (EventListener* listener);
        bool removeEventListener (EventListener* listener);
        void clearEventListener ();

    protected:
        void init();
        void process (EventNode* node);
        void finalize();

        int eventType;
        string eventName;
        unsigned long int eventCount;
        vector<EventListener*> listeners;
};
#endif
