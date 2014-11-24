#include "TaskSelectionPolicy.hh"

TaskSelectionPolicy::TaskSelectionPolicy () {
    tasks.clear();
}

TaskSelectionPolicy::~TaskSelectionPolicy() {
}

tasksSet& TaskSelectionPolicy::getSelectedTasks() {
    return tasks;
}

int TaskSelectionPolicy::getNumberTasksScheduled() {
    return tasks.size();
}

void TaskSelectionPolicy::setSource (Scheduler* scheduler) {
    this->scheduler = scheduler;
}

/* Override this method in case you don't won't to clean it ...*/
void TaskSelectionPolicy::cleanUp() {
    tasks.clear();
}

void TaskSelectionPolicy::setReschedule (bool reschedule) {
    this->reschedule = reschedule;
}

void TaskSelectionPolicy::getRidOfTasks (Host* host, bool all) {
    scheduler->getRidOfTasks (host, all);
}

vector<Task*> TaskSelectionPolicy::pickUpTasks () {
    return scheduler->pickUpTasks();
}

void TaskSelectionPolicy::setTaskState (Task* task, int state) {
    scheduler->setTaskState (task, state);
}
