#include "IndependentRunsStatistics.hh"

IndependentRunsStatistics::IndependentRunsStatistics() {
}

IndependentRunsStatistics::~IndependentRunsStatistics() {
}

double IndependentRunsStatistics::getNumberIndependentRuns () {
    return makespan.count();
}

double IndependentRunsStatistics::getAverageMakespan () {
    return makespan.mean();
}

double IndependentRunsStatistics::getMinMakespan() {
    return makespan.min();
}

double IndependentRunsStatistics::getMaxMakespan() {
    return makespan.max();
}

double IndependentRunsStatistics::getSdMakespan() {
    return makespan.sd();
}

double IndependentRunsStatistics::getConfidenceMakespan(double conf){
    return makespan.confidence_interval (conf);
}

double IndependentRunsStatistics::getAverageFlowTime () {
    return flowtime.mean();
}

double IndependentRunsStatistics::getMinFlowTime() {
    return flowtime.min();
}

double IndependentRunsStatistics::getMaxFlowTime() {
    return flowtime.max();
}

double IndependentRunsStatistics::getSdFlowTime() {
    return flowtime.sd();
}

double IndependentRunsStatistics::getConfidenceFlowTime(double conf) {
    return flowtime.confidence_interval (conf);
}

double IndependentRunsStatistics::getAverageUtilisation() {
    return utilisation.mean();
}

double IndependentRunsStatistics::getMinUtilisation() {
    return utilisation.min();
}

double IndependentRunsStatistics::getMaxUtilisation() {
    return utilisation.max();
}

double IndependentRunsStatistics::getSdUtilisation() {
    return utilisation.sd();
}

double IndependentRunsStatistics::getConfidenceUtilisation (double conf) {
    return utilisation.confidence_interval (conf);
}


extern double g_penalty_flowtime ;
extern double g_penalty_makespan ;
extern double g_penalty_utilizatin ;
extern int g_penalty_number ;
extern int g_number_tasks;
extern int g_number_crashed;

void IndependentRunsStatistics::outputStatistics(ostream& out){
    char* buffer = new char[5000];

    sprintf( buffer,
"------------------------- INDEPENDENT RUN STATISTICS --------------------------\n"
"#Independent Runs:    [%15.3f]\n"
"Average makespan:     [%15.3f]   Minimum makespan:   [%15.3f]\n"
"Maximum makespan:     [%15.3f]   Sd. makespan:       [%15.3f]\n"
"Confidence Interval:  [�%14.3f]\n"
"Average flowtime:     [%15.3f]   Minimum flowtime:   [%15.3f]\n"
"Maximum flowtime:     [%15.3f]   Sd. flowtime:       [%15.3f]\n"
"Confidence Interval:  [�%14.3f]\n"
"Average utilisation:  [%15.3f]   Minimum utilisation:[%15.3f]\n"
"Maximum utilisation:  [%15.3f]   Sd. utilisation:    [%15.3f]\n"
"Confidence Interval:  [�%14.3f]\n"

"Average number crashed:       [%15.3f]   \n"
"\n"

"-------------------------------------------------------------------------------\n",
    getNumberIndependentRuns(),
    getAverageMakespan(),               getMinMakespan(),
    getMaxMakespan(),                   getSdMakespan(),
    getConfidenceMakespan(),
    getAverageFlowTime(),               getMinFlowTime(),
    getMaxFlowTime(),                   getSdFlowTime(),
    getConfidenceFlowTime(),
    getAverageUtilisation(),            getMinUtilisation(),
    getMaxUtilisation(),                getSdUtilisation(),
    getConfidenceUtilisation(),

    (double)(g_number_crashed / getNumberIndependentRuns())

    );

    out << buffer;
    delete[] buffer;
}

void IndependentRunsStatistics::addMakespan (double makespan) {
    this->makespan.add(makespan);
}

void IndependentRunsStatistics::addFlowTime (double flowtime) {
    this->flowtime.add(flowtime);
}

void IndependentRunsStatistics::addUtilisation (double utilisation) {
    this->utilisation.add(utilisation);
}
