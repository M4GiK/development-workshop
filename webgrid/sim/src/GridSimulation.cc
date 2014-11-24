#include "GridSimulation.hh"

GridSimulation::GridSimulation() : Simulation() {
    numTasks = remainTasks = finishedTasks = 0;

    registerEvent (EVN_ENTER,    "ENTER");
    registerEvent (EVN_START,    "START");
    registerEvent (EVN_FINISH,   "FINISH");
    registerEvent (EVN_SCHEDULE, "SCHEDULE");
    registerEvent (EVN_NEW_HOST, "NEW_HOST");
    registerEvent (EVN_STOP_HOST,"STOP_HOST");

    Grid::resetGid();
    Host::resetHid();
    Task::resetTid();
}

GridSimulation::~GridSimulation() {
    for (unsigned int i = 0; i < tasks.size(); ++i) delete tasks[i];
    for (unsigned int i = 0; i < grids.size(); ++i) delete grids[i];
}

void GridSimulation::addGrid (Distribution* mips, Distribution* delHost,
                              Distribution* addHost, int minHosts,
                              int initialHosts, int maxHosts,
                              string allocPolicy) {
    /* Grid class initializes the first events (EVN_NEW_HOST,   */
    /* EVN_STOP_HOST, ...) (see the source code)                */
    Grid* grid = new Grid (mips, delHost, addHost, minHosts, initialHosts,
                           maxHosts, allocPolicy, this);
    grids.push_back(grid);
}

void GridSimulation::setTasks (Distribution* iaTime,Distribution* workLoad,
                               int totalTasks, int initialTasks) {
    if ((totalTasks >= initialTasks) && (initialTasks >= 0)) {
        this->numTasks = totalTasks;
        this->remainTasks = totalTasks - initialTasks;
        this->iaTime = iaTime;
        this->workLoad = workLoad;
        for (int i = 0; i < initialTasks; ++i) {
            createNewTask();
        }
    } else error ("GridSimulation: Wrong number of tasks");
}

void GridSimulation::setScheduler (Scheduler* scheduler) {
    this->scheduler = scheduler;
    scheduler->setSource(this);
}

GlobalStatistics& GridSimulation::getGlobalStatistics() {
    return statistics;
}

vector<Grid*>& GridSimulation::getGrids() {
    return grids;
}

Grid* GridSimulation::getGridById (int gid) {
    for (unsigned int i = 0; i < grids.size(); ++i) {
        if ( grids[i]->getGid() == gid) return grids[i];
    }
    return NULL;
}

Task* GridSimulation::getTaskById (int tid) {
    for (unsigned int i = 0; i < tasks.size(); ++i) {
        if ( (tasks[i]->getTid()) == tid) return tasks[i];
    }
    Task* task = NULL;
    for (unsigned int i = 0; i < grids.size(); ++i) {
         task = grids[i]->getTaskById(tid);
        if (task != NULL) return task;
    }
    return NULL;
}

Host* GridSimulation::getHostById(int hid) {
    Host* host = NULL;
    for (unsigned int i = 0; i < grids.size(); ++i) {
        host = grids[i]->getHostById(hid);
        if (host != NULL) return host;
    }
    return NULL;
}

vector<Host*> GridSimulation::getAllHosts() {
    vector<Host*> res;

    for (unsigned int i = 0; i < grids.size(); ++i) {
        vector<Host*>& tmp = grids[i]->getAllHosts();
        for (unsigned int j = 0; j < tmp.size(); j++) {
            res.push_back(tmp[j]);
        }
    }
    return res;
}

vector<Host*> GridSimulation::getAllBusyHosts() {
    vector<Host*> res;

    for (unsigned int i = 0; i < grids.size(); ++i) {
        vector<Host*> tmp = grids[i]->getBusyHosts();
        for (unsigned int j = 0; j < tmp.size(); j++) {
            res.push_back(tmp[j]);
        }
    }
    return res;
}

vector<Host*> GridSimulation::getAllIdleHosts() {
    vector<Host*> res;

    for (unsigned int i = 0; i < grids.size(); ++i) {
        vector<Host*> tmp= grids[i]->getIdleHosts();
        for (unsigned int j = 0; j < tmp.size(); j++) {
            res.push_back(tmp[j]);
        }
    }
    return res;
}

vector<Task*>& GridSimulation::getTasksToSchedule () {
    return tasks;
}

int GridSimulation::getNumberGrids() {
    return grids.size();
}

int GridSimulation::getTotalNumberHosts() {
    int number = 0;
    for (unsigned int i = 0; i < grids.size(); ++i) {
        number += grids[i]->getNumberHosts();
    }
    return number;
}

int GridSimulation::getTotalNumberIdleHosts() {
    int number = 0;
    for (unsigned int i = 0; i < grids.size(); ++i) {
        number += grids[i]->getNumberIdleHosts();
    }
    return number;
}

int GridSimulation::getTotalNumberBusyHosts() {
    int number = 0;
    for (unsigned int i = 0; i < grids.size(); ++i) {
        number += grids[i]->getNumberBusyHosts();
    }
    return number;
}

int GridSimulation::getNumberTasksToSchedule() {
    return tasks.size();
}

int GridSimulation::getTotalNumberTasks(bool exec) {
    /* Parameter 'exec' tells whether the executing tasks are included */
    int number = 0;
    number += getNumberTasksToSchedule();
    for (unsigned int i = 0; i < grids.size(); ++i) {
        number += grids[i]->getNumberTasks(exec);
    }
    return number;
}

double GridSimulation::getTotalUtilisation (){
    double actual = 0.0, potential = 0.0;
    for (unsigned int i = 0; i < grids.size(); ++i) {
        double a = 0, p = 0;
        grids[i]->getUtilisation(a, p);
        actual += a;
        potential += p;
    }
    if (potential == 0) return 0;
    else                return actual/potential;
}

bool GridSimulation::isFinish() {
    return (finishedTasks == numTasks);
}

void GridSimulation::updateStat(EventNode* event) {
    /* Because of the large amount of statistics, every statistics is  */
    /* updated on appropiate methods...                                */
}

void GridSimulation::report() {
    /* Because of the large amount of statistics and for the sake of   */
    /* extensibility, consult GlobalStatistics class...                */
}

void GridSimulation::performInit() {
    traceInitSimulation();

    vector<Host*> h = getAllHosts();
    for (unsigned int i = 0; i < h.size(); ++i) traceNewHost(h[i]);
    vector<Task*> t = getTasksToSchedule();
    for (unsigned int i = 0; i < t.size(); ++i) traceEnter(t[i]);

    scheduler->performInit();   /* Decide the first scheduling event */
    updateContinuousStatistics();

    if (remainTasks > 0) {
    	/* This is the only time that EVN_ENTER has this priority, wich */
    	/* is lower than any other...                                   */
    	if (tasks.size() == 0) {
        	scheduleAfter (0, EVN_ENTER, 1);
        } else {
        	scheduleAfter (iaTime->generate(), EVN_ENTER, 1);
        }
    }
}

void GridSimulation::performProcess(EventNode* event) {
    int eventType = event->eventType;
    switch (eventType) {
        case EVN_ENTER:      processEnter(event);                    break;
        case EVN_START:      processStart(event);                    break;
        case EVN_FINISH:     processFinish(event);                   break;
        case EVN_SCHEDULE:   processSchedule(event);                 break;
        case EVN_NEW_HOST:   processNewHost(event);                  break;
        case EVN_STOP_HOST:  processStopHost(event);                 break;
        default:                                                     break;
    }
}

void GridSimulation::performFinalize() {
    traceFinalizeSimulation();
    updateContinuousStatistics();

    for (unsigned int i = 0; i < grids.size(); ++i) {
        grids[i]->performFinalize();
    }
    vector<Host*> rest = getAllHosts();
    for (unsigned int i = 0; i < rest.size(); ++i) {
        updateHostFinalizationStatistics (rest[i]);
    }
}

Task* GridSimulation::createNewTask () {
    double work =  workLoad->generate();
    Task* task  =  new Task (work, getSimTime());
    transferTask(task);
    return task;
}

void GridSimulation::transferTasks (vector<Task*>& tsks) {
    tasks.insert(tasks.end(), tsks.begin(), tsks.end());
}

void  GridSimulation::transferTask (Task* tsk) {
    tasks.push_back(tsk);
}

vector<Task*> GridSimulation::pickUpTasks () {
    vector<Task*> ret = tasks;
    tasks.clear();
    return ret;
}

void GridSimulation::processEnter(EventNode* event) {
    Task* task = createNewTask();
    remainTasks--;
    if (remainTasks > 0) {
        double arrive = iaTime->generate();
        scheduleAfter (arrive, EVN_ENTER, EVN_ENTER_PRIO);
    }

    scheduler->reportEnter(task);
    traceEnter (task);
    updateStatisticsEnter (task);
}

void GridSimulation::processStart (EventNode* event) {
    int tid = (int) event->attrib[0];
    Task* task = getTaskById(tid);
    task->startTask();

    scheduler->reportStart (task);
    traceStart (task);
    updateStatisticsStart (task);
}

void GridSimulation::processFinish (EventNode* event) {
    int tid = (int) event->attrib[0];
    Task* task = getTaskById (tid);
    task->finishTask();
    finishedTasks++;

    scheduler->reportFinish (task);
    traceFinish (task);
    updateStatisticsFinish (task);
    delete task;
}

void GridSimulation::processSchedule(EventNode* event)  {
    scheduler->schedule();
    traceSchedule();
    updateStatisticsSchedule();
}

void GridSimulation::processNewHost (EventNode* event) {
    int gid = (int) event->attrib[0];
    Grid* grid = getGridById(gid);
    Host* host = grid->addAHost();  /* NULL if it is not possible */

    scheduler->reportNewHost(host);
    traceNewHost(host);
    updateStatisticsNewHost(host);
}

void GridSimulation::processStopHost (EventNode* event) {
    int gid = (int) event->attrib[0];
    Grid* grid = getGridById(gid);
    Host* host = grid->stopAHost();  /* NULL if it is not possible */

    scheduler->reportStopHost(host);
    traceStopHost(host);
    updateStatisticsStopHost(host);
    if (host != NULL) { delete host; }
}

void GridSimulation::updateStatisticsEnter (Task* task) {
    updateQueueLength();
}

void GridSimulation::updateStatisticsStart(Task* task) {
    updateNumberFreeHosts();
    updateUtilisation();
}

void GridSimulation::updateStatisticsFinish(Task* task) {
    updateFinishedTaskStatistics(task);
    updateNumberFreeHosts();
    updateUtilisation();
    updateEnergy();
}

void GridSimulation::updateStatisticsSchedule() {
    updateSchedulingStatistics();
    int previousTasks  = scheduler->getNumberTasksScheduled();
    updateQueueLength(previousTasks); /* global queue before schedule */
    updateQueueLength();              /* global queue after schedule  */
    updateNumberFreeHosts();
    updateUtilisation();
}

void GridSimulation::updateStatisticsNewHost(Host* host) {
    if (host != NULL) {
        updateNumberHosts();
        updateNumberFreeHosts();
        updateUtilisation();
    }
}

void GridSimulation::updateStatisticsStopHost(Host* host) {
    if (host != NULL) {
        updateHostFinalizationStatistics(host);
        updateNumberHosts();
        updateNumberFreeHosts();
        updateUtilisation();
        updateQueueLength();
    }
}

void GridSimulation::updateQueueLength(int includeTasks) {
    double time     = getSimTime();
    int queueLength = getNumberTasksToSchedule() + includeTasks;
    statistics.addQueueLength (queueLength, time);
}

void GridSimulation::updateNumberFreeHosts() {
    double time              = getSimTime();
    int totalNumberFreeHosts = getTotalNumberIdleHosts();
    statistics.addNumberFreeHosts (totalNumberFreeHosts, time);
}

void GridSimulation::updateNumberHosts() {
    double time           = getSimTime();
    int totalNumberHosts  = getTotalNumberHosts();
    statistics.addNumberHosts (totalNumberHosts, time);
}

void GridSimulation::updateUtilisation() {
    double time         = getSimTime();
    double utilisation  = getTotalUtilisation();
    statistics.addUtilisation (utilisation, time);
}

void GridSimulation::updateFinishedTaskStatistics(Task* task) {
    double startTime  = task->getStartTime();
    double submitTime = task->getSubmitTime();
    double finishTime = task->getFinishTime();
    int numSchedules  = task->getNumSchedules();
    statistics.addTaskFinishTime(finishTime);
    statistics.addWaitTime (startTime - submitTime);
    statistics.addSchedulesPerTask (numSchedules);
}

void GridSimulation::updateSchedulingStatistics() {
    bool activated = scheduler->wasLastActivated();
    statistics.increaseSchedulerCalls();
    if (activated) {
        double interval = scheduler->getLastActivationInterval();
        statistics.addSchedulerActivationInterval(interval);
    }
}

void GridSimulation::updateHostFinalizationStatistics(Host* host) {
    double host_total_time   =  host->getTotalTime();
    double host_busy_time    =  host->getBusyTime();
    double host_idle_time    =  host->getIdleTime();
    double host_lost_time    =  host->getLostTime();
    statistics.increaseTotalTime(host_total_time);
    statistics.increaseBusyTime (host_busy_time);
    statistics.increaseIdleTime (host_idle_time);
    statistics.increaseLostTime (host_lost_time);
}

void GridSimulation::updateContinuousStatistics() {
    updateNumberHosts();
    updateNumberFreeHosts();
    updateQueueLength ();
    updateUtilisation();
}

void GridSimulation::updateEnergy() {
	statistics.setEnergy(scheduler->getMaxEnergy(),scheduler->getEnergyI(),scheduler->getEnergyII(),scheduler->getEparam());
}

void GridSimulation::traceEnter (Task* task) {
    if (enableTrace) {
        double time   = getSimTime();
        int    tid    = task->getTid();
        double work   = task->getWorkLoad();
        char*  buffer = new char[5000];

        sprintf (buffer, "time= %012.2f event= EVN_ENTER     info: "
                 "Task# %.5d, Work  = %012.2f\n", time, tid , work);
        _trace_(buffer);
        delete[] buffer;
    }
}

void GridSimulation::traceInitSimulation () {
    if (enableTrace) {
        char* buffer = new char[5000];
        sprintf (buffer, "\n%sBegin of simulation. Output trace is:%s\n",
                 repeat("�",21).c_str(), repeat("�",21).c_str() );
        _trace_(buffer);
        delete[] buffer;
    }
}

void GridSimulation::traceStart (Task* task) {
    if (enableTrace) {
        double time       = getSimTime();
        int    tid        = task->getTid();
        Host*  host       = task->getHost();
        int    hid        = host->getHid();
        double exeTime    = host->getEstimatedExecutionTime(task);
        double finishTime = time + exeTime;
        char*  buffer     = new char[5000];

        sprintf (buffer, "time= %012.2f event= EVN_START     info: "
                 "Task# %.5d  on Host# %.5d\n"
                 "                                           "
                 "   finishTime = %012.2f\n"
                 "                                           "
                 "   exeTime    = %012.2f\n", time, tid, hid, finishTime,
                 exeTime);

        _trace_(buffer);
        delete[] buffer;
    }
}

void GridSimulation::traceFinish (Task* task) {
    if (enableTrace) {
        double time   = getSimTime();
        int    tid    = task->getTid();
        Host*  host   = task->getHost();
        int    hid    = host->getHid();
        int    state  = host->getState();
        string stat   = (state == STATE_BUSY) ? "BUSY" : "IDLE";
        char*  buffer = new char[5000];

        sprintf (buffer, "time= %012.2f event= EVN_FINISH    info: "
                 "Task# %.5d, Host# %.5d,%s now\n", time, tid, hid,
                 stat.c_str());
        _trace_(buffer);
        delete[] buffer;
    }
}

void GridSimulation::traceSchedule () {
    if (enableTrace) {
        double time      = getSimTime();
        char*  buffer    = new char[5000];
        bool   activated = scheduler->wasLastActivated();
        if (!activated) {
            sprintf (buffer,"time= %012.2f event= EVN_SCHEDULE  info: "
                     "Not activated!\n", time);
        } else {
            int n_tasks = scheduler->getNumberTasksScheduled();
            int n_hosts = scheduler->getNumberHostsScheduled();
            sprintf (buffer,"time= %012.2f event= EVN_SCHEDULE  info: "
                     "Scheduled %.5d Tasks,%.5d Hosts\n", time, n_tasks,
                     n_hosts);
        }
        _trace_(buffer);
        delete[] buffer;
    }
}

void GridSimulation::traceNewHost (Host* host) {
    if (enableTrace) {
        double time   = getSimTime();
        char*  buffer = new char[5000];
        if (host != NULL) {
            int hid     = host->getHid();
            double mips = host->getMips();
            sprintf (buffer, "time= %012.2f event= EVN_NEW_HOST  info: "
                     "Host# %.5d, Mips  = %012.2f\n", time, hid, mips);
        } else {
            sprintf (buffer, "time= %012.2f event= EVN_NEW_HOST  info: "
                     "Failed! (Maximum number of Hosts)\n", time);
        }
        _trace_(buffer);
        delete[] buffer;
    }
}

void GridSimulation::traceStopHost (Host* host) {
    if (enableTrace) {
        double time   = getSimTime();
        char*  buffer = new char[5000];
        if (host != NULL) {
            int hid  = host->getHid();
            sprintf (buffer, "time= %012.2f event= EVN_STOP_HOST info: "
                     "Host# %.5d\n", time, hid);
        } else {
            sprintf (buffer, "time= %012.2f event= EVN_STOP_HOST info: "
                     "Failed! (Minimum number of Hosts)\n", time);
        }
        _trace_(buffer);
        delete[] buffer;
    }
}

void GridSimulation::traceFinalizeSimulation() {
    if (enableTrace) {
        char* buffer = new char[5000];
        sprintf (buffer, "%s\n", repeat("�",79).c_str());
        _trace_(buffer);
        delete[] buffer;
    }
}
