#include "SpaceSharePolicy.hh"

SpaceSharePolicy::SpaceSharePolicy(){
    executing = NULL;
    scheduled.clear();
    nextFinishTime = getSimulationTime();
}

SpaceSharePolicy::~SpaceSharePolicy() {
    for (unsigned int i = 0; i < scheduled.size(); ++i) {
        delete scheduled[i];
    }
    if (executing != NULL) delete executing;
}

double SpaceSharePolicy::getExecutionTime (Task* task) {
    /* If the task is being executed, returns its execution time up*/
    /* to the moment. Otherwise, it returns 0;                     */
    if ((task->getState() == STATE_RUN) && (task->getHost() == host)) {
        return (getSimulationTime() - task->getStartTime() );
    } else {
        return 0;
    }
}

double SpaceSharePolicy::getEstimatedExecutionTime (Task* task) {
    /* Returns the estimated execution time that the task would require */
    /* to complete on this host...                                      */
    return (task->getWorkLoad())/(host->getMips());
}

vector<Task*> SpaceSharePolicy::getExecutingTasks () {
    vector<Task*> ret;
    if (executing != NULL) ret.push_back (executing);
    return ret;
}

vector<Task*> SpaceSharePolicy::getAllTasks () {
    vector<Task*> ret;
    if (executing != NULL) ret.push_back (executing);
    ret.insert(ret.end(), scheduled.begin(), scheduled.end());
    return ret;
}

int SpaceSharePolicy::getNumberTasks (bool exec) {
    int number = scheduled.size();
    if ((executing != NULL) && exec) number++;
    return number;
}

int SpaceSharePolicy::getQueueLength () {
    return scheduled.size();
}

Task* SpaceSharePolicy::getTaskById (int tid) {
    if ((executing!= NULL) && (executing->getTid() == tid)) {
        return executing;
    }
    for (unsigned int i = 0; i < scheduled.size(); ++i) {
        if (scheduled[i]->getTid() == tid ) return scheduled[i];
    }
    return NULL;
}

double SpaceSharePolicy::getNextFinishTime () {
    if (nextFinishTime < getSimulationTime()) {
        nextFinishTime = getSimulationTime();
    }
    return nextFinishTime;
}

void SpaceSharePolicy::setSource (Host* host) {
    this->host = host;
    nextFinishTime = host->getExpectedReadyTime();
    getRidOfTasks (true);
}

void SpaceSharePolicy::transferTasks (vector<Task*>& tasks) {
    double sumWorkTime = 0;

    for (unsigned int i = 0; i < tasks.size(); ++i) {
        sumWorkTime += getEstimatedExecutionTime(tasks[i]);
    }
    scheduled.insert(scheduled.end(), tasks.begin(), tasks.end());
    specificSchedule ();
    setHostExpectedReadyTime(host->getExpectedReadyTime() + sumWorkTime);
    if (executing == NULL) {
        dispatchNext();
    }
}

void SpaceSharePolicy::dispatchNext() {
    if (executing != NULL) return;
    if ((executing == NULL) && scheduled.size() > 0) {
        Task* task = scheduled.front();
        scheduleAfter (getSimulator(), 0, EVN_START, EVN_START_PRIO,
                       wrap(1, (double) task->getTid() ));
        setHostState(STATE_BUSY);
    } else {
        setHostState(STATE_IDLE);
    }
}

void SpaceSharePolicy::getRidOfTasks (bool include_executing) {
    if (include_executing && (executing == NULL) && scheduled.size() > 0) {
        executing = scheduled.front();
        scheduled.erase(scheduled.begin());
        cancelEvent (EVN_START, wrap(1, (double) executing->getTid()));
    }

    if (include_executing && (executing != NULL)) taskStop(executing);
    while (scheduled.size() > 0) { taskStop(scheduled.front()); }
}

bool SpaceSharePolicy::runTask (Task* task) {
    if ( (executing == NULL) && (task == scheduled.front()) ) {
        scheduled.erase (scheduled.begin()); //scheduled.pop_front();
        executing = task;
        setHostState (STATE_BUSY);
        double estimatedExecutionTime = getEstimatedExecutionTime (task);
        /* task->estimatedExecutionTime=getEstimatedExecutionTime(task); */
        setNextFinishTime (getSimulationTime() + estimatedExecutionTime);
        scheduleAfter (getSimulator(), estimatedExecutionTime, EVN_FINISH,
                       EVN_FINISH_PRIO, wrap(1,(double)task->getTid()) );
        return true;
    } else {
        return false;
    }
}

bool SpaceSharePolicy::finishTask (Task* task) {
    if ( executing == task ) {
        executing = NULL;
        dispatchNext();
        return true;
    } else {
        return false;
    }
}

bool SpaceSharePolicy::stopTask (Task* task) {
    if (executing == task) {
        double expectedReadyTime = host->getExpectedReadyTime();
        expectedReadyTime = expectedReadyTime - nextFinishTime +
                            getSimulationTime();
        setHostExpectedReadyTime (expectedReadyTime);
        setNextFinishTime(getSimulationTime());
        transferTaskToSimulator (task);
        cancelEvent (EVN_FINISH, wrap (1, (double) task->getTid() ));
        executing = NULL;
        dispatchNext();
        return true;
    } else if ((executing == NULL) && (scheduled.size() > 0) &&
               (scheduled.front() == task)) {
        /* This is very improbable, but it can happen if dispatchNext  */
        /* and stopHost occur at the same simulation time, without the */
        /* execution of startTask for the first task ...               */
        cancelEvent (EVN_START, wrap (1, (double) task->getTid() ) );
        executing = scheduled.front();
        scheduled.erase (scheduled.begin());
        return stopTask (task);  // same as the first case ;)
    } else if (task->getHost() == host) {
        double exeTime = getEstimatedExecutionTime(task);
        /* Better if: exeTime = task->getEstimatedExecutionTime();     */
        double expectedReadyTime = host->getExpectedReadyTime() - exeTime;
        setHostExpectedReadyTime (expectedReadyTime);
        transferTaskToSimulator (task);
        scheduled.erase( find (scheduled.begin(), scheduled.end(), task) );
        return true;
    } else {
        return false;
    }
}

bool SpaceSharePolicy::pauseTask  (Task* task) {
    error ("SpaceSharePolicy.cc: pauseTask not implemented yet");
    return false;
}

bool SpaceSharePolicy::cancelTask (Task* task) {
    error ("SpaceSharePolicy.cc: cancelTask not implemented yet");
    return false;
}

bool SpaceSharePolicy::resumeTask (Task* task) {
    error ("SpaceSharePolicy.cc: resumeTask not implemented yet");
    return false;
}

void SpaceSharePolicy::setNextFinishTime (double time) {
    this->nextFinishTime = time;
}
