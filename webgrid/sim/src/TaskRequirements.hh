#ifndef __TASK_REQUIREMENTS_H__
#define __TASK_REQUIREMENTS_H__
class TaskRequirements;

#include "Features.hh"
#include "HostCharacteristics.hh"
#include <string>
using namespace std;

class TaskRequirements : public Features {
    friend class Task;
    friend class Scheduler;
    public:
        TaskRequirements();
        virtual ~TaskRequirements();

        /* More get methods */
        virtual double   getMaxCost();
        virtual double   getCostFactor();
        virtual int      getNumberCpus();
        virtual double   getMaxQueueTime();
        virtual double   getEstimatedWorkload();
        virtual double   getEarliestStartTime();
        virtual double   getDeadline();

    protected:
        /* More set methods */
        virtual void     setMaxCost    (double maxCost);
        virtual void     setCostFactor (double costFactor);
        virtual void     setNumberCpus (int nCpus);
        virtual void     setMaxQueueTime (double maxQueueTime);
        virtual void     setEstimatedWorkload (double estimatedWorkload);
        virtual void     setEarliestStartTime (double earliestStartTime);
        virtual void     setDeadline (double deadline);

        /* Other Methods */
        virtual void     unsetAllFeatures ();
        virtual double   matchMaking(HostCharacteristics* hCharact) = 0;

        double     maxCost;               /* Maximum economic cost       */
        double     costFactor;            /* Priority for maximum cost   */
        double     maxQueueTime;          /* Maximum time to be scheduled*/
        int        numberCpus;            /* Number of CPUs needed       */
        double     estimatedWorkload;     /* Estimated Workload          */
        double     earliestStartTime;     /* Earliest Start time         */
        double     deadline;              /* Deadline                    */
};
#endif
