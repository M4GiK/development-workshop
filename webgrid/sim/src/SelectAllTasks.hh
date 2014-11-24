#ifndef __SELECT_ALL_TASKS_H__
#define __SELECT_ALL_TASKS_H__
class SelectAllTasks;

#include "TaskSelectionPolicy.hh"
#include "Task.hh"

class SelectAllTasks : public TaskSelectionPolicy {
    public:
        SelectAllTasks();
        virtual ~SelectAllTasks();

    protected:
        /* Auxiliar methods implementation */
        virtual bool checkActivation();

        /* Task Selection Policy methods implementation */
        virtual void selectTasks(hostsSet& hosts);
};
#endif
