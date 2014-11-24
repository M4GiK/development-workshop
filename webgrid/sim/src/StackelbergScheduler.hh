/*
 * StackelbergScheduler.hh
 *
 *  Created on: Mar 15, 2010
 *      Author: gumer
 */

#ifndef STACKELBERGSCHEDULER_HH_
#define STACKELBERGSCHEDULER_HH_
class StackelbergScheduler;

#include "UtilsPackage.hh"
#include "SchedulingPolicy.hh"
#include "GAParams.hh"
#include <math.h>

class StackelbergScheduler : public SchedulingPolicy {

	public:
		StackelbergScheduler(double secs, bool h_s, GAParams params);
		virtual ~StackelbergScheduler();

	protected:

		virtual void schedule (hostsSet& hosts, tasksSet& tasks);

    private:
		GAParams params;
        double seconds;
        bool   h_or_s;

};

#endif /* STACKELBERGSCHEDULER_HH_ */
