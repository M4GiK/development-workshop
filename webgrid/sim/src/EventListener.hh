#ifndef __EVENT_LISTENER_H__
#define __EVENT_LISTENER_H__
class EventListener;

#include "EventAttribute.hh"
#include "EventNode.hh"
#include "EventHandler.hh"
#include "UtilsPackage.hh"
#include <vector>
#include <string>
#include <fstream>
using namespace std;

class EventListener {  /* abstract */
    friend class Simulation;
    friend class EventAttribute;
    public:
        EventListener (EventHandler* rOOt);
        EventListener ();
        virtual ~EventListener ();

        /* Use this methods to treat an object as an EventListener */
        void setHandler (EventHandler* rOOt);
        void setTrace (bool flag);
        void setTraceFile (string fileName);
        void setTraceFile (char* fileName);
        double getLastEventTime();

        void registerEvent (int eventType, string eventName);
        bool addEventListener (int eventType, EventListener* listener);
        bool removeEventListener (int eventType, EventListener* listener);

        /* Use this adapter methods to access an EventHandler */
        void scheduleEventNode (EventNode* node);
        void scheduleAt (double clock_value, int eventType, int priority,
                         vector<double> vd, void* data = NULL);
        void scheduleAt (double clock_value, int eventType,
                         int priority = 0, void* data = NULL);
        void scheduleAt (EventListener* dest, double clock_value,
                         int eventType, int priority, vector<double> vd,
                         void* data = NULL);
        void scheduleAt (EventListener* dest, double clock_value,
                         int eventType, int priority = 0,
                         void *data = NULL);
        void scheduleAfter (double clock_value, int eventType,int priority,
                            vector<double> vd, void* data = NULL);
        void scheduleAfter (double clock_value, int eventType,
                            int priority = 0, void *data = NULL);
        void scheduleAfter (EventListener* dest, double clock_value,
                            int eventType, int priority, vector<double> vd,
                            void *data = NULL);
        void scheduleAfter (EventListener* dest, double clock_value,
                            int eventType, int priority = 0,
                            void* data = NULL);

        int cancelEvent (int eventType, vector<double> attrib,
                         bool allFlag = false);
        int cancelEvent (int eventType, bool allFlag = false);

    protected:
        virtual void init();
        virtual void performInit();
        virtual bool trace (EventNode* event);
        virtual void performTrace (EventNode* event);
        virtual void process (EventNode* event);
        virtual void performProcess (EventNode* event);
        virtual void finalize ();
        virtual void performFinalize();
        virtual void _trace_ (char* str);
        virtual void _trace_ (string str);

        double getSimulationTime();

        vector<EventAttribute*> eventAttr;
        double lastEventTime;
        bool enableTrace;
        ofstream traceFile;
        static EventHandler* handler;
};
#endif
