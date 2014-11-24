#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__
class Scheduler;

#include "Host.hh"
#include "Task.hh"
#include <vector>
#include <map>
using namespace std;
 typedef vector<Host*>              hostsSet;
 typedef vector<Task*>              tasksSet;
 typedef map<Host*,vector<Task*> >  schedule_;
#include "EventListener.hh"
#include "SchedulerActivation.hh"
#include "HostSelectionPolicy.hh"
#include "TaskSelectionPolicy.hh"
#include "SchedulingPolicy.hh"
#include "GridSimulation.hh"

class Scheduler : public EventListener{
    friend class GridSimulation;
    friend class SchedulerActivation;
    friend class HostSelectionPolicy;
    friend class TaskSelectionPolicy;
    friend class SchedulingPolicy;
    public:
        Scheduler ();
        virtual ~Scheduler();

        /* Scheduler configuration methods */
        void setSource (GridSimulation* sim);
        void setReschedule (bool reschedule = true);
        void setSchedulerActivation (SchedulerActivation*  activation);
        void setHostSelectionPolicy (HostSelectionPolicy* hostSelection);
        void setTaskSelectionPolicy (TaskSelectionPolicy* taskSelection);
        void setSchedulingPolicy (SchedulingPolicy* strategy);

        /* Scheduler methods */
        virtual bool schedule (bool force = false);
        virtual bool checkActivation ();

        /* Statistics methods */
        virtual int      getNumberHostsScheduled();
        virtual int      getNumberTasksScheduled();
        virtual bool     wasLastActivated();
        virtual double   getLastActivationInterval();

        /* Energy statistics */
        double getMaxEnergy();
        double getEnergyI();
        double getEnergyII();
        double getEparam();

        /* Other methods */
        GridSimulation*  getSource();

    protected:
        /* Auxiliar methods */
        virtual void cleanUp();
        virtual void distributeSchedule (schedule_& plan);

        /* Methods to keep the scheduler aware of the simulation status */
        virtual void performInit();
        virtual void reportEnter(Task* task);
        virtual void reportStart(Task* task);
        virtual void reportFinish(Task* task);
        virtual void reportStopHost(Host* host);
        virtual void reportNewHost(Host* host);

        /* Methods to allow friend access modifier be inherited ... */
        static void   getRidOfTasks (Host* host, bool all = false);
        vector<Task*> pickUpTasks   ();
        static void   setTaskState  (Task* task, int state);
        static void   assignHost    (Task* task, Host* host);
        static void   transferTasks (Host* host, vector<Task*>& schedule);

        GridSimulation*       sim;                 /* GridSimulation     */
        SchedulerActivation*  schedulerActivation; /* SchedulerActivation*/
        TaskSelectionPolicy*  taskSelectionPolicy; /* TaskSelectionPolicy*/
        HostSelectionPolicy*  hostSelectionPolicy; /* HostSelectionPolicy*/
        SchedulingPolicy*     schedulingPolicy;    /* SchedulingPolicy   */
};
#endif
