#ifndef __NGA_SCHEDULER_H__
#define __NGA_SCHEDULER_H__
class NGAScheduler;

#include <mpi.h>
#include "UtilsPackage.hh"
#include "SchedulingPolicy.hh"
#include "GAParams.hh"
#include <math.h>

class NGAScheduler : public SchedulingPolicy {
    /* This is a simple meta-scheduler */
    public:
        NGAScheduler(double secs, bool sga, GAParams params);
        virtual ~NGAScheduler();

    protected:
        /* Scheduling Policy methods implementation*/
        virtual void schedule (hostsSet& hosts, tasksSet& tasks);
    
    private:
        GAParams params;
        double seconds;
        bool   struggle;
            
};
#endif
