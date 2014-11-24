#ifndef __HOST_CHARACTERISTICS_H__
#define __HOST_CHARACTERISTICS_H__
class HostCharacteristics;

#include "Features.hh"
#include "TaskRequirements.hh"
#include <string>
using namespace std;

class HostCharacteristics : public Features {
    friend class Host;
    friend class Scheduler;
    public:
        HostCharacteristics();
        virtual ~HostCharacteristics();

        /* More get methods */
        virtual double   getMaxLoad();
        virtual double   getMinLoad();
        virtual double   getMaxWorkload();
        virtual double   getMinWorkload();

    protected:
        /* More set methods */
        virtual void     setMaxLoad (double maxLoad);
        virtual void     setMinLoad (double minLoad);
        virtual void     setMaxWorkload (double maxWorkload);
        virtual void     setMinWorkload (double minWorkload);

        /* Other methods */
        virtual void     unsetAllFeatures ();
        virtual double   matchMaking (TaskRequirements* tRequirements) = 0;

        double     maxLoad;          /* Maximum possible load            */
        double     minLoad;          /* Minimum possible load            */
        double     maxWorkload;      /* Maximum admissible workload      */
        double     minWorkload;      /* Minimum admissible workload      */
};
#endif
