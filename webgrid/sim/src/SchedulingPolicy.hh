#ifndef __SCHEDULING_POLICY_H__
#define __SCHEDULING_POLICY_H__
class SchedulingPolicy;

#include "EventListener.hh"
#include "Scheduler.hh"
#include "SchedulerActivation.hh"
#include "HostSelectionPolicy.hh"
#include "TaskSelectionPolicy.hh"

class SchedulingPolicy: public EventListener {
    friend class Scheduler;
    friend class SchedulerActivation;
    friend class HostSelectionPolicy;
    friend class TaskSelectionPolicy;
    public:
        SchedulingPolicy();
        virtual ~SchedulingPolicy();

        /* Get methods */
        virtual schedule_& getSchedule();

        /* Energy get methods */
        double getMaxEnergy();
        double getEnergyI();
        double getEnergyII();
        double getEparam();

    protected:
        /* Auxiliar methods */
        virtual void setSource (Scheduler* scheduler);
        virtual void cleanUp();

        /* Definition of Scheduling Policy methods */
        virtual void schedule (hostsSet& hosts, tasksSet& tasks) = 0;

        /* Method to allow friend access modifier */
        void  assignHost  (Task* task, Host* host);

        Scheduler* scheduler;         /* Scheduler reference   */
        schedule_  plan;              /*  -> THE schedule <-   */

        // energy
        double MaxEnergy;
        double EnergyI;
        double EnergyII;
        double Eparam;
};
#endif
