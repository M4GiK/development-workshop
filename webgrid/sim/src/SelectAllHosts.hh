#ifndef __SELECT_ALL_HOSTS_H__
#define __SELECT_ALL_HOSTS_H__
class SelectAllHosts;

#include "HostSelectionPolicy.hh"
#include "GridSimulation.hh"
#include "Grid.hh"
#include "Host.hh"
#include "Task.hh"

class SelectAllHosts : public HostSelectionPolicy {
    public:
        SelectAllHosts();
        virtual ~SelectAllHosts();

    protected:
        /* Auxiliar methods implementation */
        virtual bool  checkActivation();

        /* Host Selection Policy methods implementation */
        virtual void  selectHosts(tasksSet& tasks);
};
#endif
