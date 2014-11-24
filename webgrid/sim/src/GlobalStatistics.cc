#include "GlobalStatistics.hh"

GlobalStatistics::GlobalStatistics () {
    totalTime = totalBusyTime = totalIdleTime = totalLostTime = 0.0;
    schedulerCalls = 0;
}

GlobalStatistics::~GlobalStatistics () {
}

double GlobalStatistics::getMakespan () {
    return makespan_flowtime.max();
}

double GlobalStatistics::getFlowTime () {
    return makespan_flowtime.sum();
}

double GlobalStatistics::getTotalPotentialTime() {
    return totalTime;
}

double GlobalStatistics::getTotalIdleTime() {
    return totalIdleTime;
}

double GlobalStatistics::getPercentTotalIdleTime() {
    return (totalIdleTime / totalTime) * 100.0;
}

double GlobalStatistics::getTotalBusyTime() {
    return totalBusyTime;
}

double GlobalStatistics::getPercentTotalBusyTime() {
    return (totalBusyTime / totalTime) * 100.0;
}

double GlobalStatistics::getTotalLostTime() {
    return totalLostTime;
}

double GlobalStatistics::getPercentTotalLostTime() {
    return (totalLostTime / totalTime) * 100.0;
}

double GlobalStatistics::getPercentAverageHostUtilisation() {
    return utilisation.mean_freq() * 100.0;
}

double GlobalStatistics::getPercentMaxHostUtilisation() {
    return utilisation.max() * 100.0;
}

double GlobalStatistics::getPercentMinHostUtilisation() {
    return utilisation.min() * 100.0;
}

double GlobalStatistics::getPercentSdHostUtilisation() {
    return utilisation.sd_freq() * 100.0;
}

double GlobalStatistics::getAverageNumberHosts() {
    return totalHosts.mean_freq();
}

double GlobalStatistics::getMaxNumberHosts() {
    return totalHosts.max();
}

double GlobalStatistics::getMinNumberHosts() {
    return totalHosts.min();
}

double GlobalStatistics::getSdNumberHosts() {
    return totalHosts.sd_freq();
}

double GlobalStatistics::getAverageFreeHosts() {
    return freeHosts.mean_freq();
}

double GlobalStatistics::getMaxFreeHosts() {
    return freeHosts.max();
}

double GlobalStatistics::getMinFreeHosts() {
    return freeHosts.min();
}

double GlobalStatistics::getSdFreeHosts() {
    return freeHosts.sd_freq();
}

double GlobalStatistics::getAverageGlobalQueueLength() {
    return queueLength.mean_freq();
}

double GlobalStatistics::getMaxGlobalQueueLength() {
    return queueLength.max();
}

double GlobalStatistics::getMinGlobalQueueLength() {
    return queueLength.min();
}

double GlobalStatistics::getSdGlobalQueueLength() {
    return queueLength.sd_freq();
}

double GlobalStatistics::getAverageWaitTime() {
    return waitTime.mean();
}

double GlobalStatistics::getMaxWaitTime() {
    return waitTime.max();
}

double GlobalStatistics::getMinWaitTime() {
    return waitTime.min();
}

double GlobalStatistics::getSdWaitTime() {
    return waitTime.sd();
}

double GlobalStatistics::getAverageSchedulesPerTask() {
    return schedulesPerTask.mean();
}

double GlobalStatistics::getMaxSchedulesPerTask() {
    return schedulesPerTask.max();
}

double GlobalStatistics::getMinSchedulesPerTask() {
    return schedulesPerTask.min();
}

double GlobalStatistics::getSdSchedulesPerTask() {
    return schedulesPerTask.sd();
}

double GlobalStatistics::getNumberSchedulerCalls() {
    return schedulerCalls;
}

double GlobalStatistics::getNumberSchedulerActivations() {
    return activationInterval.count();
}

double GlobalStatistics::getAverageActivationInterval() {
    return activationInterval.mean();
}

double GlobalStatistics::getMaxActivationInterval() {
    return activationInterval.max();
}

double GlobalStatistics::getMinActivationInterval() {
    return activationInterval.min();
}

double GlobalStatistics::getSdActivationInterval() {
    return activationInterval.sd();
}

double GlobalStatistics::getMaxEnergy() {
	return MaxEnergy;
}

double GlobalStatistics::getEnergyI() {
	return EnergyI;
}

double GlobalStatistics::getEnergyII() {
	return EnergyII;
}

double GlobalStatistics::getEparam() {
	return Eparam;
}

void GlobalStatistics::outputStatistics(ostream& out) {
  char* buffer = new char[5000];
  sprintf( buffer,
"-------------------- RUN STATISTICS (avg. over all runs) ----------------------\n"
"Makespan:               [%13.2f]\n"
"Flowtime:               [%13.2f]\n"
"Total Potential Time:   [%13.2f] Total Idle Time:        [%12.3f%%]\n"
"Total Busy Time:        [%12.3f%%] Total Lost Time:        [%12.3f%%]\n"
"Avg. Host Utilisation:  [%12.3f%%] Max. Host Utilisation:  [%12.3f%%]\n"
"Min. Host Utilisation:  [%12.3f%%] Sd.  Host Utilisation:  [%12.3f%%]\n"
"Avg.Number Hosts:       [%13.2f] Max.Number Hosts:       [%13.2f]\n"
"Min.Number Hosts:       [%13.2f] Sd. Number Hosts:       [%13.2f]\n"
"Avg.Number Free Hosts:  [%13.2f] Max.Number Free Hosts:  [%13.2f]\n"
"Min.Number Free Hosts:  [%13.2f] Sd. Number Free Hosts:  [%13.2f]\n"
"Avg.Global Queue Length:[%13.2f] Max.Global Queue Length:[%13.2f]\n"
"Min.Global Queue Length:[%13.2f] Sd. Global Queue Length:[%13.2f]\n"
"Avg.Wait Time:          [%13.2f] Max.Wait Time:          [%13.2f]\n"
"Min.Wait Time:          [%13.2f] Sd.Wait Time:           [%13.2f]\n"
"Avg.Schedules x Task:   [%13.2f] Max.Schedules x Task:   [%13.2f]\n"
"Min.Schedules x Task:   [%13.2f] Sd.Schedules x Task:    [%13.2f]\n"
"#Scheduler Calls:       [%13.2f] #Activations:           [%13.2f]\n"
"Avg.Activation Interval:[%13.2f] Max.Activation Interval:[%13.2f]\n"
"Min.Activation Interval:[%13.2f] Sd. Activation Interval:[%13.2f]\n"
"Energy Case I:          [%13.2f] Energy Case II:         [%13.2f]\n"
"Max.Energy:             [%13.2f] Energy parameter:       [%13.2f]\n"
"-------------------------------------------------------------------------------\n",
    getMakespan(),
    getFlowTime(),
    getTotalPotentialTime(),            getPercentTotalIdleTime(),
    getPercentTotalBusyTime(),          getPercentTotalLostTime(),
    getPercentAverageHostUtilisation(), getPercentMaxHostUtilisation(),
    getPercentMinHostUtilisation(),     getPercentSdHostUtilisation(),
    getAverageNumberHosts(),            getMaxNumberHosts(),
    getMinNumberHosts(),                getSdNumberHosts(),
    getAverageFreeHosts(),              getMaxFreeHosts(),
    getMinFreeHosts(),                  getSdFreeHosts(),
    getAverageGlobalQueueLength(),      getMaxGlobalQueueLength(),
    getMinGlobalQueueLength(),          getSdGlobalQueueLength(),
    getAverageWaitTime(),               getMaxWaitTime(),
    getMinWaitTime(),                   getSdWaitTime(),
    getAverageSchedulesPerTask(),       getMaxSchedulesPerTask(),
    getMinSchedulesPerTask(),           getSdSchedulesPerTask(),
    getNumberSchedulerCalls(),          getNumberSchedulerActivations(),
    getAverageActivationInterval(),     getMaxActivationInterval(),
    getMinActivationInterval(),         getSdActivationInterval(),
    getEnergyI(),						getEnergyII(),
    getMaxEnergy(),						getEparam()
    );

  out << buffer;
  delete[] buffer;
}

void GlobalStatistics::addTaskFinishTime  (double time) {
    makespan_flowtime.add (time);
}

void GlobalStatistics::increaseTotalTime (double time) {
    totalTime += time;
}

void GlobalStatistics::increaseBusyTime (double time) {
    totalBusyTime += time;
}

void GlobalStatistics::increaseIdleTime (double time) {
    totalIdleTime += time;
}

void GlobalStatistics::increaseLostTime (double time) {
    totalLostTime += time;
}

void GlobalStatistics::addUtilisation (double utilisation, double time) {
    this->utilisation.add (utilisation, time);
}

void GlobalStatistics::addNumberHosts (double numberHosts, double time){
    totalHosts.add (numberHosts, time);
}

void GlobalStatistics::addNumberFreeHosts (double numberFreeHosts,
                                           double time) {
    freeHosts.add (numberFreeHosts, time);
}

void GlobalStatistics::addQueueLength (double queueLength, double time) {
    this->queueLength.add (queueLength, time);
}

void GlobalStatistics::addWaitTime (double waitTime) {
    this->waitTime.add (waitTime);
}

void GlobalStatistics::addSchedulesPerTask (double schedules) {
    schedulesPerTask.add (schedules);
}

void GlobalStatistics::increaseSchedulerCalls() {
    ++schedulerCalls;
}

void GlobalStatistics::addSchedulerActivationInterval (double interval) {
    activationInterval.add (interval);
}

void GlobalStatistics::setEnergy(double _MaxEnergy, double _EnergyI, double _EnergyII, double _Eparam) {
	MaxEnergy = _MaxEnergy;
	EnergyI   = _EnergyI;
	EnergyII  = _EnergyII;
	Eparam    = _Eparam;
}
