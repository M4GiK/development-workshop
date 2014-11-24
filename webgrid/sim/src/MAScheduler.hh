#ifndef __MA_SCHEDULER_H__
#define __MA_SCHEDULER_H__
class MAScheduler;

#include "UtilsPackage.hh"
#include "SchedulingPolicy.hh"
#include "Memetic.hh"
#include "GAParams.hh"
#include <math.h>

class MAScheduler : public SchedulingPolicy {
    /* This is a simple meta-scheduler */
    public:
        MAScheduler(double secs, int ls, MAParams params);
        virtual ~MAScheduler();

    protected:
        /* Scheduling Policy methods implementation*/
        virtual void schedule (hostsSet& hosts, tasksSet& tasks);
    
    private:
        MAParams params;
        double seconds;
        int   lsearch;

		void set_lsearch_parameters (int nbr_tasks, Memetic::Setup& setup);
            
};
#endif
