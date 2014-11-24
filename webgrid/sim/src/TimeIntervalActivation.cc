#include "TimeIntervalActivation.hh"

TimeIntervalActivation::TimeIntervalActivation (Distribution* activation) {
    this->activation = activation;
}

TimeIntervalActivation::~TimeIntervalActivation() {
    delete activation;
}

bool TimeIntervalActivation::checkActivation() {
    return true;
}

void TimeIntervalActivation::performInit() {
    GridSimulation* sim = scheduler->getSource();
    if (scheduler->checkActivation()) {
        scheduleAfter (sim, 0, EVN_SCHEDULE, EVN_SCHEDULE_PRIO);
    } else {
        scheduleAfter (sim, activation->generate(), EVN_SCHEDULE,
                       EVN_SCHEDULE_PRIO);
    }
}

void TimeIntervalActivation::reportEnter (Task* task) {
    /* Nothing to do */
}

void TimeIntervalActivation::reportStart (Task* task) {
    /* Nothing to do */
}

void TimeIntervalActivation::reportFinish (Task* task) {
    /* Nothing to do */
}

void TimeIntervalActivation::reportStopHost (Host* host) {
    /* Nothing to do */
}

void TimeIntervalActivation::reportNewHost (Host* host) {
    /* Nothing to do */
}

void TimeIntervalActivation::postSchedule (bool activated) {
    /* Don't care if the scheduler was activated */
    GridSimulation* sim = scheduler->getSource();
    double next = activation->generate();
    scheduleAfter (sim, next, EVN_SCHEDULE, EVN_SCHEDULE_PRIO);
}
