#ifndef __SPTF_POLICY_H__
#define __SPTF_POLICY_H__
class SPTFPolicy;

#include "SpaceSharePolicy.hh"
#include "Task.hh"
#include <vector>
#include <algorithm>
using namespace std;

/* Shortest Processing Time First */
class SPTFPolicy: public SpaceSharePolicy {
    public:
        SPTFPolicy();
        virtual ~SPTFPolicy();

    protected:
        /* Shortes Processing Time auxiliar methods implementation */
        virtual void  specificSchedule ();
        static  bool  sortPolicy (Task* a, Task* b);
};
#endif
