#ifndef __TIME_INTERVAL_ACTIVATION_H__
#define __TIME_INTERVAL_ACTIVATION_H__
class TimeIntervalActivation;

#include "SchedulerActivation.hh"

class TimeIntervalActivation : public SchedulerActivation {
    public:
        TimeIntervalActivation (Distribution* activation);
        virtual ~TimeIntervalActivation();

    protected:
        /* Auxiliar methods implementation */
        virtual bool checkActivation();

        /* Scheduler Activation methods implementation */
        virtual void performInit();
        virtual void reportEnter    (Task* task);
        virtual void reportStart    (Task* task);
        virtual void reportFinish   (Task* task);
        virtual void reportStopHost (Host* host);
        virtual void reportNewHost  (Host* host);
        virtual void postSchedule   (bool activated);

        Distribution*   activation;     /* Activation time distribution */
};
#endif
