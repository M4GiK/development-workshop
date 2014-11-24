#include "Scheduler.hh"

Scheduler::Scheduler() {
}

Scheduler::~Scheduler () {
}

void Scheduler::setSource(GridSimulation* sim) {
    this->sim = sim;
}

void Scheduler::setReschedule (bool reschedule) {
    taskSelectionPolicy->setReschedule (reschedule);
}

void Scheduler::setSchedulerActivation(SchedulerActivation*  activation) {
    schedulerActivation = activation;
    activation->setSource (this);
}

void Scheduler::setHostSelectionPolicy(HostSelectionPolicy* hostSelection){
    hostSelectionPolicy = hostSelection;
    hostSelectionPolicy->setSource (this);
}

void Scheduler::setTaskSelectionPolicy(TaskSelectionPolicy* taskSelection){
    taskSelectionPolicy = taskSelection;
    taskSelectionPolicy->setSource (this);
}

void Scheduler::setSchedulingPolicy(SchedulingPolicy* strategy) {
    schedulingPolicy = strategy;
    schedulingPolicy->setSource (this);
}

bool Scheduler::schedule (bool force) {
    if (checkActivation() || force ) {
        cleanUp();

        tasksSet aux;
        hostSelectionPolicy->selectHosts(aux);
        hostsSet& hosts = hostSelectionPolicy->getSelectedHosts();

        taskSelectionPolicy->selectTasks(hosts);
        tasksSet& tasks = taskSelectionPolicy->getSelectedTasks();

        schedulingPolicy->schedule(hosts, tasks);
        schedule_& plan  =  schedulingPolicy->getSchedule();

        schedulerActivation->reportSchedule (plan);
        distributeSchedule (plan);
        return true;
    } else {
        schedulerActivation->reportSchedule (false);
        return false;
    }
}

bool Scheduler::checkActivation () {
    return (hostSelectionPolicy->checkActivation() &&
            taskSelectionPolicy->checkActivation() &&
            schedulerActivation->checkActivation()
           );
}

int Scheduler::getNumberHostsScheduled() {
    return hostSelectionPolicy->getNumberHostsScheduled();
}

int Scheduler::getNumberTasksScheduled() {
    return taskSelectionPolicy->getNumberTasksScheduled();
}

bool Scheduler::wasLastActivated() {
    return schedulerActivation->wasLastActivated();
}

double Scheduler::getLastActivationInterval() {
    return schedulerActivation->getLastActivationInterval();
}

double Scheduler::getMaxEnergy() {
	return schedulingPolicy->getMaxEnergy();
}

double Scheduler::getEnergyI() {
	return schedulingPolicy->getEnergyI();
}

double Scheduler::getEnergyII() {
	return schedulingPolicy->getEnergyII();
}

double Scheduler::getEparam() {
	return schedulingPolicy->getEparam();
}

GridSimulation* Scheduler::getSource() {
    return sim;
}


void Scheduler::cleanUp () {
    schedulerActivation->cleanUp();
    taskSelectionPolicy->cleanUp();
    hostSelectionPolicy->cleanUp();
    schedulingPolicy->cleanUp();
}

void Scheduler::distributeSchedule (schedule_& plan) {
    schedule_::iterator it;

    for (it = plan.begin(); it != plan.end(); ++it) {
        if ((*it).second.size() > 0) {
            transferTasks ((*it).first, (*it).second);
        }
    }
}

void Scheduler::performInit() {
    schedulerActivation->performInit();
    taskSelectionPolicy->performInit();
    hostSelectionPolicy->performInit();
    schedulingPolicy->performInit();
}

void Scheduler::reportEnter(Task* task) {
    schedulerActivation->reportEnter (task);
}

void Scheduler::reportStart(Task* task) {
    schedulerActivation->reportStart (task);
}

void Scheduler::reportFinish(Task* task) {
    schedulerActivation->reportFinish (task);
}

void Scheduler::reportStopHost(Host* host) {
    schedulerActivation->reportStopHost (host);
}

void Scheduler::reportNewHost(Host* host) {
    schedulerActivation->reportNewHost (host);
}

/* Since scheduler subclasses and scheduler related classes don't inherit*/
/* friend access modifier, we need methods to allow them access protected*/
/* methods, that are available to Scheduler.cc  ...                      */

void Scheduler::getRidOfTasks (Host* host,  bool all) {
    host->getRidOfTasks(all);
}

vector<Task*> Scheduler::pickUpTasks () {
    return sim->pickUpTasks();
}

void Scheduler::setTaskState (Task* task, int state) {
    task->setState(state);
}

void Scheduler::assignHost (Task* task, Host* host) {
    task->setHost(host);
}

void Scheduler::transferTasks (Host* host, vector<Task*>& schedule) {
    host->transferTasks (schedule);
}
