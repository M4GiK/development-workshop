#include "SchedulerActivation.hh"

SchedulerActivation::SchedulerActivation () {
    lastActivation = lastActivation_ = 0.0;
    lastActivated = false;
}

SchedulerActivation::~SchedulerActivation() {
}

bool SchedulerActivation::wasLastActivated() {
    return lastActivated;
}

double SchedulerActivation::getLastActivationInterval() {
    return lastActivation - lastActivation_;
}

void SchedulerActivation::setSource (Scheduler* scheduler) {
    this->scheduler = scheduler;
}

/* Override this method in case you don't won't to clean it ...*/
void SchedulerActivation::cleanUp() {
}

void SchedulerActivation::reportSchedule (bool activated) {
    if (activated) {
        lastActivation_ =  lastActivation;
        lastActivation  =  getSimulationTime();
        lastActivated   =  true;
        postSchedule (true);
    } else {
        lastActivated   =  false;
        postSchedule (false);
    }
}

void SchedulerActivation::reportSchedule (schedule_& plan) {
    this->plan      =  &plan;
    reportSchedule (true);
}
