#ifndef __RESOURCE_AND_TIME_ACTIVATION_H__
#define __RESOURCE_AND_TIME_ACTIVATION_H__
class ResourceAndTimeActivation;

#include "TimeIntervalActivation.hh"

class ResourceAndTimeActivation: public TimeIntervalActivation {
    public:
        ResourceAndTimeActivation (Distribution* activation);
        virtual ~ResourceAndTimeActivation();

    protected:
        /* Scheduler Activation methods implementation */
        virtual void reportStopHost (Host* host);
        virtual void reportNewHost  (Host* host);
};
#endif
