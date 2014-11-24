#include "SelectAllTasks.hh"

SelectAllTasks::SelectAllTasks() {
}

SelectAllTasks::~SelectAllTasks() {
}

bool SelectAllTasks::checkActivation() {
    GridSimulation* sim = scheduler->getSource();
    int numberOfTasks   = 0;
    if (reschedule) {
        numberOfTasks = sim->getTotalNumberTasks(false);//exclude executing
    } else {
        numberOfTasks = sim->getNumberTasksToSchedule();
    }
    return (numberOfTasks > 0);
}

void SelectAllTasks::selectTasks(hostsSet& hosts) {
    if (reschedule) {
        int numberOfHosts = hosts.size();
        for (int i = 0; i < numberOfHosts; ++i) {
            getRidOfTasks(hosts[i], false);
        }
    }
    tasks = pickUpTasks();

    int numberOfTasks = tasks.size();
    for (int i = 0; i < numberOfTasks; ++i) {
        setTaskState (tasks[i], STATE_SCHEDULED);
    }
}
