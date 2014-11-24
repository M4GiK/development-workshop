#include "Host.hh"
#include <time.h>

/* Static variables must be initialized, otherwise it won't compile */
int Host::nextHid = 0;

Host::Host (double mips, GridSimulation* sim) {
    this->hid  = getNextHid();
    this->mips = mips;
    this->sim  = sim;
    setState(STATE_IDLE);
    expectedReadyTime  =  getSimulationTime();
    wakeupTime         =  getSimulationTime();
    shutdownTime       =  -1.0;
    busyTime           =  0.0;
    lostTime           =  0.0;
    parent             =  NULL;
    allocPolicy        =  NULL;
    hCharacter         =  NULL;
    srand( time(NULL));
    const double       TL_min = 0.3;
    const double       TL_max = 1.0;
    tl                 =  (TL_min+(TL_max-TL_min)*rand()/double(RAND_MAX));
}

Host::Host (double mips, AllocPolicy* allocPolicy, GridSimulation* sim) {
    this->hid  = getNextHid();
    this->mips = mips;
    this->sim  = sim;
    setAllocPolicy (allocPolicy);
    setState(STATE_IDLE);
    expectedReadyTime  =  getSimulationTime();
    wakeupTime         =  getSimulationTime();
    shutdownTime       =  -1.0;
    busyTime           =  0.0;
    lostTime           =  0.0;
    parent             =  NULL;
    hCharacter         =  NULL;
}

Host::~Host() {
    shutdownTime = getSimulationTime();
    if (allocPolicy != NULL) delete allocPolicy;
    if (hCharacter  != NULL) delete hCharacter;
}

int Host::getHid() {
    return hid;
}

double Host::getMips() {
    return mips;
}

int Host::getState() {
    return state;
}

double Host::getTl() {
    return tl;
}

double Host::getExpectedReadyTime() {
    if (expectedReadyTime < getSimulationTime()) {
        expectedReadyTime = getSimulationTime();
    }
    return expectedReadyTime;
}

double Host::getWakeUpTime() {
    return wakeupTime;
}

double Host::getShutDownTime() {
    return shutdownTime;
}

double Host::getTotalTime() {
    if (getState() == STATE_STOP) {
        return shutdownTime - wakeupTime;
    } else {
        return getSimulationTime() - wakeupTime;
    }
}

double Host::getBusyTime() {
    if (getState() == STATE_BUSY) {
        double currentExeTime = 0;   vector<Task*> v = getExecutingTasks();
        for (unsigned int i = 0; i < v.size(); ++i) {
            currentExeTime += getExecutionTime(v[i]);
        }
        //return getSimulationTime() - executing->getStartTime() +busyTime;
        return currentExeTime + busyTime;
    } else {
        return busyTime;
    }
}

double Host::getIdleTime() {
    return getTotalTime() - getBusyTime() - getLostTime();
}

double Host::getLostTime() {
    return lostTime;
}

HCharacteristics* Host::getCharacteristics () {
    return hCharacter;
}

double Host::getExecutionTime (Task* task) {
    return allocPolicy->getExecutionTime (task);
}

double Host::getEstimatedExecutionTime (Task* task) {
    return allocPolicy->getEstimatedExecutionTime (task);
}

vector<Task*> Host::getExecutingTasks() {
    return allocPolicy->getExecutingTasks();
}

Grid* Host::getParent() {
    return parent;
}

Task* Host::getTaskById (int tid){
    return allocPolicy->getTaskById (tid);
}

vector<Task*> Host::getAllTasks(){
    return allocPolicy->getAllTasks();
}

int Host::getNumberTasks (bool exec) {
    return allocPolicy->getNumberTasks(exec);
}

int Host::getQueueLength() {
    return allocPolicy->getQueueLength();
}

void Host::setAllocPolicy(AllocPolicy* allocPolicy) {
    this->allocPolicy = allocPolicy;
    allocPolicy->setSource(this);
}

void Host::setCharacteristics (HCharacteristics* hCharacter) {
    this->hCharacter = hCharacter;
}

void Host::setParent(Grid* grid) {
    parent = grid;
}

void Host::transferTasks (vector<Task*>& tasks) {
    allocPolicy->transferTasks (tasks);
}

void Host::stopHost() {
    getRidOfTasks(true);
    shutdownTime = getSimulationTime();
    setState(STATE_STOP);
}

void Host::getRidOfTasks (bool include_executing) {
    allocPolicy->getRidOfTasks (include_executing);
}

bool Host::runTask (Task* task) {
    if (allocPolicy->runTask(task)) {
        return true;
    } else {
        return false;
    }
}

bool Host::finishTask (Task* task) {
    if (allocPolicy->finishTask(task)) {
        busyTime += getExecutionTime(task);
        return true;
    } else {
        return false;
    }
}

bool Host::stopTask (Task* task) {
    if (allocPolicy->stopTask(task)) {
        if (task->getState() == STATE_RUN) {
            lostTime += getExecutionTime (task);
        }
        return true;
    } else {
        return false;
    }
}

bool Host::pauseTask  (Task* task) {
    if (allocPolicy->pauseTask(task)) {
        return true;
    } else {
        return false;
    }
}

bool Host::cancelTask (Task* task) {
    if (allocPolicy->cancelTask(task)) {
        return true;
    } else {
        return false;
    }
}

bool Host::resumeTask (Task* task) {
    if (allocPolicy->resumeTask(task)) {
        return true;
    } else {
        return false;
    }
}

void Host::setState (int state) {
    this->state = state;
}

void Host::setExpectedReadyTime(double readyTime) {
    this->expectedReadyTime = readyTime;
}

void Host::transferTasksToSimulator (vector<Task*>& tasks) {
    sim->transferTasks (tasks);
}

void Host::transferTaskToSimulator (Task* task) {
    sim->transferTask (task);
}

bool Host::taskRun (Task* task) {
    return task->runTask();
}

bool Host::taskFinish (Task* task) {
    return task->finishTask();
}

bool Host::taskStop (Task* task) {
    return task->stopTask();
}

bool Host::taskPause (Task* task) {
    return task->pauseTask();
}

bool Host::taskCancel (Task* task) {
    return task->cancelTask();
}

bool Host::taskResume (Task* task) {
    return task->resumeTask();
}

int Host::getNextHid() {
    int id = nextHid;
    nextHid++;
    return id;
}

void Host::resetHid() {
    nextHid = 0;
}
