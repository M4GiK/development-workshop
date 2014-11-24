#ifndef __SIMPLE_METAPOLICY_H__
#define __SIMPLE_METAPOLICY_H__
class SimpleMetaPolicy;

#include "SchedulingPolicy.hh"

class SimpleMetaPolicy : public SchedulingPolicy {
    /* This is a simple meta-scheduler */
    public:
        SimpleMetaPolicy();
        virtual ~SimpleMetaPolicy();

    protected:
        /* Scheduling Policy methods implementation*/
        virtual void schedule (hostsSet& hosts, tasksSet& tasks);
};
#endif
