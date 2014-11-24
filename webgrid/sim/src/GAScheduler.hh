/*
 * StackelbergScheduler.hh
 *
 *  Created on: ??
 *      Author: ??
 */

#ifndef __GA_SCHEDULER_H__
#define __GA_SCHEDULER_H__
class GAScheduler;

#include "UtilsPackage.hh"
#include "SchedulingPolicy.hh"
#include "GAParams.hh"
#include <math.h>

class GAScheduler : public SchedulingPolicy {
    /* This is a simple meta-scheduler */
    public:
        GAScheduler(double secs, bool h_s, GAParams params);
        virtual ~GAScheduler();

    protected:
        /* Scheduling Policy methods implementation*/
        virtual void schedule (hostsSet& hosts, tasksSet& tasks);
    
    private:
        GAParams params;
        double seconds;
        bool   h_or_s;
            
};
#endif
