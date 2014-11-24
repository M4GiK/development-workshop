/*
 * HgsScheduler.hh
 *
 *  Created on: 2010-01-14
 *      Author: LukkoL
 */

#ifndef HGSSCHEDULER_HH_
#define HGSSCHEDULER_HH_
class HgsScheduler;


#include "UtilsPackage.hh"
#include "SchedulingPolicy.hh"
#include "GAParams.hh"
#include <math.h>

class HgsScheduler : public SchedulingPolicy {
	public:
		HgsScheduler(double secs, bool h_s, GAParams params, GAParams (&hgs_params)[4]);
		virtual ~HgsScheduler();

	protected:
		virtual void schedule (hostsSet& hosts, tasksSet& tasks);

    private:
		GAParams hgs_params[4];
		GAParams params;
        double seconds;
        bool   h_or_s;

};

#endif /* HGSSCHEDULER_HH_ */

