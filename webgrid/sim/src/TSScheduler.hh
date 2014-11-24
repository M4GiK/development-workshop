#ifndef __TS_SCHEDULER_H__
#define __TS_SCHEDULER_H__
class TSScheduler;

#include "UtilsPackage.hh"
#include "SchedulingPolicy.hh"
#include "GAParams.hh"
#include <math.h>

class TSScheduler : public SchedulingPolicy {
    /* This is a simple meta-scheduler */
    public:
        TSScheduler(double secs, bool h_s, TabuParams params);
        virtual ~TSScheduler();

    protected:
        /* Scheduling Policy methods implementation*/
        virtual void schedule (hostsSet& hosts, tasksSet& tasks);
    
    private:        
        TabuParams params;
        double seconds;
        bool   h_or_s;
            
};
#endif
