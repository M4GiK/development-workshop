#ifndef __CMA_SCHEDULER_H__
#define __CMA_SCHEDULER_H__
class CMAScheduler;

#include "UtilsPackage.hh"
#include "SchedulingPolicy.hh"
#include "Memetic.hh"
#include "GAParams.hh"
#include <math.h>

class CMAScheduler : public SchedulingPolicy {
    /* This is a simple meta-scheduler */
    public:
        CMAScheduler(double secs, int ls, CMAParams params);
        virtual ~CMAScheduler();

    protected:
        /* Scheduling Policy methods implementation*/
        virtual void schedule (hostsSet& hosts, tasksSet& tasks);
    
    private:
        CMAParams params;
        double seconds;
        int   lsearch;

		void set_lsearch_parameters (int nbr_tasks, Memetic::Setup& setup);
            
};
#endif
