#ifndef __SCHEDULER_ACTIVATION_H__
#define __SCHEDULER_ACTIVATION_H__
class SchedulerActivation;

#include "EventListener.hh"
#include "Scheduler.hh"
#include "HostSelectionPolicy.hh"
#include "TaskSelectionPolicy.hh"
#include "SchedulingPolicy.hh"
#include "Host.hh"
#include "Task.hh"

class SchedulerActivation : public EventListener {
    friend class Scheduler;
    friend class HostSelectionPolicy;
    friend class TaskSelectionPolicy;
    friend class SchedulingPolicy;
    public:
        SchedulerActivation ();
        virtual ~SchedulerActivation();

        /* Statistics methods */
        bool    wasLastActivated();
        double  getLastActivationInterval();

    protected:
        /* Auxiliar methods */
        virtual bool checkActivation() = 0;
        virtual void setSource (Scheduler* scheduler);
        virtual void cleanUp();

        /* Definition of Scheduler Activation methods */
        virtual void performInit() = 0;
        virtual void reportEnter    (Task* task) = 0;
        virtual void reportStart    (Task* task) = 0;
        virtual void reportFinish   (Task* task) = 0;
        virtual void reportStopHost (Host* host) = 0;
        virtual void reportNewHost  (Host* host) = 0;
        virtual void reportSchedule (bool activated);
        virtual void reportSchedule (schedule_& plan);
        virtual void postSchedule   (bool activated) = 0;

        Scheduler*  scheduler;        /* Scheduler reference             */
        double      lastActivation;   /* Last useful activation          */
        double      lastActivation_;  /* Before last useful activ.       */
        bool        lastActivated;    /*Did the call activate the sched.?*/
        schedule_*  plan;             /* Last schedule reference         */
};
#endif
