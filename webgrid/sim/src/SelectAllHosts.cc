#include "SelectAllHosts.hh"

SelectAllHosts::SelectAllHosts() {
}

SelectAllHosts::~SelectAllHosts() {
}

bool SelectAllHosts::checkActivation() {
    GridSimulation* sim = scheduler->getSource();
    int totalHosts = sim->getTotalNumberHosts();
    return (totalHosts > 0);
}

void SelectAllHosts::selectHosts(tasksSet& tasks) {
    //hosts = (scheduler->getSource())->getAllHosts();

    /* More efficient than GridSimulation::getAllHosts() : */
    GridSimulation* sim  = scheduler->getSource();
    vector<Grid*>& grids = sim->getGrids();
    for (unsigned int i = 0; i < grids.size(); ++i) {
        vector<Host*>& hsts = grids[i]->getAllHosts();
        for (unsigned int m = 0; m < hsts.size(); ++m) {
            hosts.push_back ( hsts[m] );
        }
    }
}
