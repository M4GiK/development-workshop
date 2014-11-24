#ifndef __TASK_SELECTION_POLICY_H__
#define __TASK_SELECTION_POLICY_H__
class TaskSelectionPolicy;

#include "EventListener.hh"
#include "Scheduler.hh"
#include "SchedulerActivation.hh"
#include "HostSelectionPolicy.hh"
#include "SchedulingPolicy.hh"

class TaskSelectionPolicy : public EventListener {
    friend class Scheduler;
    friend class SchedulerActivation;
    friend class HostSelectionPolicy;
    friend class SchedulingPolicy;
    public:
        TaskSelectionPolicy ();
        virtual ~TaskSelectionPolicy();

        /* Get methods and statistics methods */
        tasksSet&  getSelectedTasks();
        int        getNumberTasksScheduled();

    protected:
        /* Auxiliar methods */
        virtual bool  checkActivation() = 0;
        virtual void  setSource (Scheduler* scheduler);
        virtual void  cleanUp();

        /* Definition of Task Selection Policy methods */
        virtual void setReschedule (bool reschedule);
        virtual void selectTasks(hostsSet& hosts) = 0;

        /* Method to allow friend access modifier */
        void          getRidOfTasks (Host* host, bool all = false);
        vector<Task*> pickUpTasks   ();
        void          setTaskState  (Task* task, int state);

        Scheduler*   scheduler;    /* Scheduler reference                */
        tasksSet     tasks;        /* Tasks to be considered in schedule */
        bool         reschedule;   /* Reschedule unstart tasks?          */
};
#endif
