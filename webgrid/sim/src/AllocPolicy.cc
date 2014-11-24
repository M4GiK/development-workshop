#include "AllocPolicy.hh"

AllocPolicy::AllocPolicy () {
}

AllocPolicy::~AllocPolicy() {
}

void AllocPolicy::setHostState (int state) {
    host->setState (state);
}

void AllocPolicy::setHostExpectedReadyTime (double time) {
    host->setExpectedReadyTime (time);
}

GridSimulation* AllocPolicy::getSimulator () {
    return host->sim;
}

void AllocPolicy::transferTasksToSimulator (vector<Task*>& tasks) {
    host->transferTasksToSimulator (tasks);
}

void AllocPolicy::transferTaskToSimulator (Task* task) {
    host->transferTaskToSimulator (task);
}

bool AllocPolicy::taskRun (Task* task) {
    return host->taskRun(task);
}

bool AllocPolicy::taskFinish (Task* task) {
    return host->taskFinish(task);
}

bool AllocPolicy::taskStop (Task* task) {
    return host->taskStop(task);
}

bool AllocPolicy::taskPause (Task* task) {
    return host->taskPause(task);
}

bool AllocPolicy::taskCancel (Task* task) {
    return host->taskCancel(task);
}

bool AllocPolicy::taskResume (Task* task) {
    return host->taskResume(task);
}
