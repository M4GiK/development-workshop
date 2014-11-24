#ifndef __SIMULATION_H__
#define __SIMULATION_H__
class Simulation;

#include "EventHandler.hh"
#include "EventListener.hh"
#include "EventNode.hh"
#include "UtilsPackage.hh"

class Simulation: public EventHandler, public EventListener {
    public:
        Simulation (Simulation* rOOt = NULL);
        virtual ~Simulation ();

        virtual void run();

    protected:
        virtual bool isFinish () = 0;
        virtual void updateStat (EventNode* event) = 0;
        virtual void report () = 0;
};
#endif
