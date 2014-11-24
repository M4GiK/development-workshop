#ifndef __HOST_SELECTION_POLICY_H__
#define __HOST_SELECTION_POLICY_H__
class HostSelectionPolicy;

#include "EventListener.hh"
#include "Scheduler.hh"
#include "SchedulerActivation.hh"
#include "TaskSelectionPolicy.hh"
#include "SchedulingPolicy.hh"
#include "Task.hh"

class HostSelectionPolicy : public EventListener {
    friend class Scheduler;
    friend class SchedulerActivation;
    friend class TaskSelectionPolicy;
    friend class SchedulingPolicy;
    public:
        HostSelectionPolicy ();
        virtual ~HostSelectionPolicy();

        /* Get methods and statistics methods */
        hostsSet&  getSelectedHosts();
        int        getNumberHostsScheduled();

    protected:
        /* Auxiliar methods */
        virtual bool  checkActivation() = 0;
        virtual void  setSource (Scheduler* scheduler);
        virtual void  cleanUp();

        /* Definition of Host Selection Policy methods */
        virtual void  selectHosts(tasksSet& tasks) = 0;

        Scheduler*   scheduler;    /* Scheduler reference                */
        hostsSet     hosts;        /* Hosts to be considered in schedule */
};
#endif
