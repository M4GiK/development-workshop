#include "ResourceAndTimeActivation.hh"

ResourceAndTimeActivation::ResourceAndTimeActivation (Distribution* active)
                                          : TimeIntervalActivation(active){
}

ResourceAndTimeActivation::~ResourceAndTimeActivation() {
}

void ResourceAndTimeActivation::reportStopHost (Host* host) {
    //Cancel next scheduling event, because we want to schedule immediately
    cancelEvent (EVN_SCHEDULE, true);
    GridSimulation* sim = scheduler->getSource();
    scheduleAfter (sim, 0, EVN_SCHEDULE, EVN_SCHEDULE_PRIO); //schedule now
}

void ResourceAndTimeActivation::reportNewHost (Host* host) {
    //Cancel next scheduling event, because we want to schedule immediately
    cancelEvent (EVN_SCHEDULE, true);
    GridSimulation* sim = scheduler->getSource();
    scheduleAfter (sim, 0, EVN_SCHEDULE, EVN_SCHEDULE_PRIO); //schedule now
}
