#ifndef __ADHOC_SCHEDULER_H__
#define __ADHOC_SCHEDULER_H__
class AdHocScheduler;

#include "UtilsPackage.hh"
#include "SchedulingPolicy.hh"
#include "InitialSolutions.hh"
using namespace AdHocHeuristics;
#include <math.h>

class AdHocScheduler: public SchedulingPolicy {
    /* This is a simple meta-scheduler */
    public:
        AdHocScheduler(string adhoc_heuristic);
        virtual ~AdHocScheduler();

    protected:
        /* Scheduling Policy methods implementation*/
        virtual void schedule (hostsSet& hosts, tasksSet& tasks);
    
    private:        
        heuristic_type heuristic;
            
};
#endif
