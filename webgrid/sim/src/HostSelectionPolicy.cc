#include "HostSelectionPolicy.hh"

HostSelectionPolicy::HostSelectionPolicy () {
    hosts.clear();
}

HostSelectionPolicy::~HostSelectionPolicy() {
}

hostsSet& HostSelectionPolicy::getSelectedHosts() {
    return hosts;
}

int  HostSelectionPolicy::getNumberHostsScheduled() {
    return hosts.size();
}

void HostSelectionPolicy::setSource (Scheduler* scheduler) {
    this->scheduler = scheduler;
}

/* Override this method in case you don't won't to clean it ...*/
void HostSelectionPolicy::cleanUp() {
    hosts.clear();
}
