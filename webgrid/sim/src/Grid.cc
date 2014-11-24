#include "Grid.hh"

/* Static variables must be initialized, otherwise it won't compile */
int Grid::nextGid = 0;

Grid::Grid (Distribution* mips, Distribution* delHosts,
            Distribution* addHosts, int minHosts, int initialHosts,
            int maxHosts, string strAllocPolicy, GridSimulation* sim){
    if ((minHosts <= initialHosts) && (initialHosts <= maxHosts) &&
        (minHosts >= 0) && (maxHosts > 0)) {
        this->gid  = getNextGid();
        this->mips = mips;
        this->delHosts = delHosts;
        this->addHosts = addHosts;
        this->minHosts = minHosts;
        this->initialHosts = initialHosts;
        this->maxHosts = maxHosts;
        this->strAllocPolicy = strAllocPolicy;
        this->sim  = sim;
        hosts.clear();
        createRandomGrid(initialHosts, strAllocPolicy);
        performInit();
    } else error("Grid.cc: initialHosts, minHosts, maxHosts are invalid");
}

Grid::~Grid(){
    for (unsigned int i = 0; i < hosts.size(); ++i) {
        delete (hosts[i]);
    }
}

int Grid::getGid() {
    return gid;
}

GCharacteristics* Grid::getCharacteristics () {
    return gCharacter;
}

int Grid::getNumberHosts () {
    return hosts.size();
}

int Grid::getNumberIdleHosts () {
    int count = 0;
    for (unsigned int i = 0; i < hosts.size(); ++i) {
        if (hosts[i]->getState() == STATE_IDLE) count++;
    }
    return count;
}

int Grid::getNumberBusyHosts () {
    int count = 0;
    for (unsigned int i = 0; i < hosts.size(); ++i) {
        if (hosts[i]->getState() == STATE_BUSY) count++;
    }
    return count;
}

void Grid::getUtilisation (double& actual, double& potential) {
    potential = actual = 0.0;
    for (unsigned int i = 0; i < hosts.size(); ++i) {
        potential += hosts[i]->getMips();
        if (hosts[i]->getState() == STATE_BUSY) {
            actual += hosts[i]->getMips();
        }
    }
}

vector<Host*>& Grid::getAllHosts() {
    return hosts;
}

vector<Host*> Grid::getIdleHosts() {
    vector<Host*> result;
    for (unsigned int i = 0; i < hosts.size(); ++i) {
        if (hosts[i]->getState() == STATE_IDLE) {
            result.push_back(hosts[i]);
        }
    }
    return result;
}

vector<Host*> Grid::getBusyHosts() {
    vector<Host*> result;
    for (unsigned int i = 0; i < hosts.size(); ++i) {
        if (hosts[i]->getState() == STATE_BUSY) {
            result.push_back(hosts[i]);
        }
    }
    return result;
}

Host* Grid::getHostById (int hid) {
    for (unsigned int i = 0; i < hosts.size(); ++i) {
        if (hosts[i]->getHid() == hid) return hosts[i];
    }
    return NULL;
}

int Grid::getNumberTasks(bool exec){
    int number = 0;
    for (unsigned int i = 0; i < hosts.size(); ++i) {
        number += hosts[i]->getNumberTasks(exec);
    }
    return number;
}

Task* Grid::getTaskById (int tid) {
    Task* task = NULL;
    for (unsigned int i = 0; i < hosts.size(); ++i) {
        task = hosts[i]->getTaskById(tid);
        if (task != NULL) return task;
    }
    return NULL;
}

void Grid::setCharacteristics (GCharacteristics* gCharacter) {
    this->gCharacter= gCharacter;
}

Host* Grid::addAHost () {
    scheduleAfter (sim, addHosts->generate(), EVN_NEW_HOST,
                   EVN_NEW_HOST_PRIO, wrap(1, (double) gid));
    if (int(hosts.size()) < maxHosts) {
        double m = mips->generate();
        AllocPolicyFactory apf;
        AllocPolicy* ap = apf.CreateAllocPolicy (strAllocPolicy);
        Host* host = new Host (m, ap, sim);
        host->setParent(this);
        addHost(host);
        return host;
    } else return NULL;
}

bool Grid::addHost (Host* host) {
    Host* hst = sim->getHostById(host->getHid());
    if (hst == NULL) {
        host->setParent(this);
        hosts.push_back(host);
        return true;
    } else return false;
}

Host* Grid::stopAHost () {
    scheduleAfter (sim, delHosts->generate(), EVN_STOP_HOST,
                   EVN_STOP_HOST_PRIO, wrap(1, (double) gid) );
    if (int(hosts.size()) > minHosts) {
        int w = randint (0, hosts.size() - 1);
        Host* host = hosts[w];
        stopHost(hosts[w]);
        return host;
    } else return NULL;
}

bool Grid::stopHost (Host* host) {
    host->stopHost();

    vector<Host*>::iterator it;
    it = find(hosts.begin(), hosts.end(), host);
    if (it != hosts.end()) {
        hosts.erase(it);
        return true;
    }
    return false;
}

void Grid::performInit() {
    if (minHosts < maxHosts) {
        scheduleAfter (sim, addHosts->generate(), EVN_NEW_HOST,
                       EVN_NEW_HOST_PRIO, wrap(1, (double) gid) );
        scheduleAfter (sim, delHosts->generate(), EVN_STOP_HOST,
                       EVN_STOP_HOST_PRIO, wrap(1, (double) gid) );
    }
}

void Grid::performFinalize() {
    for (unsigned int i = 0; i < hosts.size(); ++i) {
        hosts[i]->stopHost();
    }
}

void Grid::createRandomGrid (int numHosts, string strAllocPolicy) {
    AllocPolicyFactory apf;
    for (int i = 0; i < numHosts; ++i) {
        double m = mips->generate();
        AllocPolicy* ap = apf.CreateAllocPolicy (strAllocPolicy);
        Host* host = new Host (m, ap, sim);
        addHost(host);
    }
    //createRandomConnections();
}

int Grid::getNextGid() {
    int id = nextGid;
    nextGid++;
    return id;
}

void Grid::resetGid() {
    nextGid = 0;
}
