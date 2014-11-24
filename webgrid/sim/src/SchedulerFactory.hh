#ifndef __SCHEDULER_FACTORY_H__
#define __SCHEDULER_FACTORY_H__
class SchedulerFactory;

#include "SchedulingPolicy.hh"
#include "SchedulerActivation.hh"
#include "HostSelectionPolicy.hh"
#include "TaskSelectionPolicy.hh"
#include "DistributionFactory.hh"
#include "Distribution.hh"
#include "UtilsPackage.hh"
#include "GAParams.hh"
#include <vector>
#include <string>
using namespace std;

class SchedulerFactory {
    public:
        SchedulerFactory ();
        virtual ~SchedulerFactory();
        virtual SchedulingPolicy*    CreateSchedulingPolicy    (string st,
        		GAParams params, GAParams (&hgs_params)[4], TabuParams tabuParams,
                        CMAParams cmaParams, MAParams maParams);
        virtual SchedulerActivation* CreateSchedulerActivation (string st);
        virtual HostSelectionPolicy* CreateHostSelectionPolicy (string st);
        virtual TaskSelectionPolicy* CreateTaskSelectionPolicy (string st);
};
#endif
