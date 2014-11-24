#ifndef __EVENT_HANDLER_H__
#define __EVENT_HANDLER_H__
class EventHandler;

#include "BinaryTree.hh"
#include "EventNode.hh"
#include "UtilsPackage.hh"
#include <vector>
using namespace std;

class EventHandler: public BinaryTree {
    public:
        EventHandler();
        virtual ~EventHandler();

        void reset();
        EventNode* nextEvent();

        void scheduleEvent (EventNode* newEntry);
        int  deleteEvent (int eventType, vector<double> attrib,
                          bool allFlag = false);
        int  deleteEvent (int eventType, bool allFlag = false);

        double getSimTime();

    protected:
        double simTime;
};
#endif
