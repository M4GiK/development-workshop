#include "SelectIdleHosts.hh"

SelectIdleHosts::SelectIdleHosts() {
}

SelectIdleHosts::~SelectIdleHosts() {
}

bool SelectIdleHosts::checkActivation() {
    GridSimulation* sim = scheduler->getSource();
    int totalIdleHosts = sim->getTotalNumberIdleHosts();
    return (totalIdleHosts > 0);
}

void SelectIdleHosts::selectHosts(tasksSet& tasks) {
    GridSimulation* sim = scheduler->getSource();
    hosts = sim->getAllIdleHosts();
}
