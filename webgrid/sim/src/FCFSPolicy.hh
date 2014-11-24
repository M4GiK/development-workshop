#ifndef __FCFS_POLICY_H__
#define __FCFS_POLICY_H__
class FCFSPolicy;

#include "SpaceSharePolicy.hh"
#include "Task.hh"
#include <vector>
#include <algorithm>
using namespace std;

/* First Come First Served Policy */
class FCFSPolicy: public SpaceSharePolicy {
    public:
        FCFSPolicy();
        virtual ~FCFSPolicy();

    protected:
        /* Space Share Policy auxiliar methods implementation */
        virtual void  specificSchedule ();
};
#endif
