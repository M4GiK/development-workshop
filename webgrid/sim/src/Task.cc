#include "Task.hh"
#include <time.h>

/* Static variables must be initialized, otherwise it won't compile */
int Task::nextTid = 0;

Task::Task (double workLoad, double submitTime) {
    this->tid           = getNextTid();
    this->totalWorkLoad = workLoad;
    this->submitTime    = submitTime;
    this->startTime     = -1.0;
    this->finishTime    = -1.0;
    this->exeTime       = -1.0;
    numSchedules        = 0;
    setState(STATE_UNASSIGN);
    host = NULL;
    srand( time(NULL));
    const double      SD_min = 0.6;
    const double      SD_max = 0.9;
    this->sd            = (SD_min+(SD_max-SD_min)*rand()/double(RAND_MAX));
}

Task::~Task() {
}

int Task::getTid(){
    return tid;
}

double Task::getWorkLoad() {
    return totalWorkLoad;
}

int Task::getState() {
    return state;
}

Host* Task::getHost() {
    return host;
}

double Task::getSd() {
    return sd;
}

double Task::getSubmitTime() {
    return submitTime;
}

double Task::getStartTime() {
    return startTime;
}

double Task::getFinishTime() {
    return finishTime;
}

double Task::getExecutionTime () {
    if (getState() == STATE_FINISH) {
        return exeTime;
    } else if (getState() != STATE_UNASSIGN) {
        return host->getExecutionTime(this);
    } else {
        return 0;
    }
}

double Task::getEstimatedExecutionTime () {
    if (getState() != STATE_UNASSIGN) {
        return host->getEstimatedExecutionTime (this);
    } else {
        return infinity();
    }
}

int Task::getNumSchedules() {
    return numSchedules;
}

Requirements* Task::getRequirements () {
    return requirements;
}

bool Task::startTask() {
    return runTask();
}

bool Task::runTask() {
    if (getState() != STATE_WAIT) {
        return false;
    } else {
        if (host->runTask(this)) {
            startTime = getSimulationTime();
            setState(STATE_RUN);
            return true;
        } else {
            return false;
        }
    }
}

bool Task::finishTask() {
    /* Think about extending this class to suport migration, costs... */
    if (getState() != STATE_RUN) {
        return false;
    } else {
        if (host->finishTask(this)){
            exeTime = getExecutionTime();
            //exeTime = getSimulationTime() - startTime;
            finishTime = getSimulationTime();
            setState(STATE_FINISH);
            return true;
        } else {
            return false;
        }
    }
}

bool Task::endTask() {
    return finishTask();
}

bool Task::stopTask () {
    /* Think about extending this class to support migration ... */
    if ((getState() != STATE_RUN) && (getState() != STATE_WAIT)) {
        return false;
    } else {
        if (host->stopTask(this)) {
            unsetHost();
            startTime = -1.0;
            setState (STATE_UNASSIGN);
            return true;
        } else {
            return false;
        }
    }
}

bool Task::pauseTask  () {
    error ("Task.cc : pauseTask not implemented yet");
    return false;
}

bool Task::cancelTask () {
    error ("Task.cc : cancelTask not implemented yet");
    return false;
}

bool Task::resumeTask () {
    error ("Task.cc : resumeTask not implemented yet");
    return false;
}

void Task::setRequirements (Requirements* requirements) {
    this->requirements = requirements;
}

void Task::setState(int state) {
    this->state = state;
}

void Task::setHost(Host* host) {
    if (host != NULL) {
        if (this->host != NULL) unsetHost();
        this->host = host;
        setState(STATE_WAIT);
        numSchedules++;
    } else {
        unsetHost();
    }
}

void Task::unsetHost() {
    this->host = NULL;
    setState(STATE_UNASSIGN);
}

int Task::getNextTid() {
    int id = nextTid;
    nextTid++;
    return id;
}

void Task::resetTid() {
    nextTid = 0;
}
