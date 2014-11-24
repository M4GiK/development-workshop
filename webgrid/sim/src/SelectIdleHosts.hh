#ifndef __SELECT_IDLE_HOSTS_H__
#define __SELECT_IDLE_HOSTS_H__
class SelectIdleHosts;

#include "HostSelectionPolicy.hh"
#include "GridSimulation.hh"
#include "Task.hh"

class SelectIdleHosts : public HostSelectionPolicy {
    public:
        SelectIdleHosts();
        virtual ~SelectIdleHosts();

    protected:
        /* Auxiliar methods implementation */
        virtual bool  checkActivation();

        /* Host Selection Policy methods implementation */
        virtual void  selectHosts(tasksSet& tasks);
};
#endif
