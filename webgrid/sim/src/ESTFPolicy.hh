#ifndef __ESTF_POLICY_H__
#define __ESTF_POLICY_H__
class ESTFPolicy;

#include "SpaceSharePolicy.hh"
#include "Task.hh"
#include <vector>
#include <algorithm>
using namespace std;

/* Earliest Submit Time First */
class ESTFPolicy: public SpaceSharePolicy {
    public:
        ESTFPolicy();
        virtual ~ESTFPolicy();

    protected:
        /* Space Share Policy auxiliar methods implementation */
        virtual void  specificSchedule ();
        static  bool  sortPolicy (Task* a, Task* b);
};
#endif
