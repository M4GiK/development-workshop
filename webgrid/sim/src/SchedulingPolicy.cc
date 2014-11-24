#include "SchedulingPolicy.hh"

SchedulingPolicy::SchedulingPolicy() {
    plan.clear();
    MaxEnergy = 0;
    Eparam = 0;
    EnergyI = 0;
    EnergyII = 0;
}

SchedulingPolicy::~SchedulingPolicy() {
}

schedule_& SchedulingPolicy::getSchedule() {
    return plan;
}

double SchedulingPolicy::getMaxEnergy() {
	return MaxEnergy;
}

double SchedulingPolicy::getEnergyI() {
	return EnergyI;
}

double SchedulingPolicy::getEnergyII() {
	return EnergyII;
}

double SchedulingPolicy::getEparam() {
	return Eparam;
}

void SchedulingPolicy::setSource (Scheduler* scheduler) {
    this->scheduler = scheduler;
}

/* Override this method in case you don't won't to clean it ...*/
void SchedulingPolicy::cleanUp() {
    plan.clear();
}

void SchedulingPolicy::assignHost (Task* task, Host* host) {
    scheduler->assignHost (task, host);
}
