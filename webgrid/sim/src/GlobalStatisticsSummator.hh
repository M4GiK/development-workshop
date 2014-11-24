#ifndef __GLOBAL_STATISTICS_SUMMATOR_H__
#define __GLOBAL_STATISTICS_SUMMATOR_H__

#include <stdio.h>
#include <iostream>
#include <fstream>
#include "GlobalStatistics.hh"
using namespace std;

class GlobalStatisticsSummator{

public:
	void outputStatistics(ostream& out);
	void pushBack(GlobalStatistics& gs);

	GlobalStatisticsSummator():

		num_pushed(0),

		Makespan(0),
		FlowTime(0),
		TotalPotentialTime(0),
		PercentTotalIdleTime(0),
		PercentTotalBusyTime(0),
		PercentTotalLostTime(0),
		PercentAverageHostUtilisation(0),
		PercentMaxHostUtilisation(0),
		PercentMinHostUtilisation(0),
		PercentSdHostUtilisation(0),
		AverageNumberHosts(0),
		MaxNumberHosts(0),
		MinNumberHosts(0),
		SdNumberHosts(0),
		AverageFreeHosts(0),
		MaxFreeHosts(0),
		MinFreeHosts(0),
		SdFreeHosts(0),
		AverageGlobalQueueLength(0),
		MaxGlobalQueueLength(0),
		MinGlobalQueueLength(0),
		SdGlobalQueueLength(0),
		AverageWaitTime(0),
		MaxWaitTime(0),
		MinWaitTime(0),
		SdWaitTime(0),
		AverageSchedulesPerTask(0),
		MaxSchedulesPerTask(0),
		MinSchedulesPerTask(0),
		SdSchedulesPerTask(0),
		NumberSchedulerCalls(0),
		NumberSchedulerActivations(0),
		AverageActivationInterval(0),
		MaxActivationInterval(0),
		MinActivationInterval(0),
		SdActivationInterval(0),
		MaxEnergy(0),
		Eparam(0),
		EnergyI(0),
		EnergyII(0)


	{}

public:

	int num_pushed;

	double Makespan;
	double FlowTime;
	double TotalPotentialTime;
	double PercentTotalIdleTime;
	double PercentTotalBusyTime;
	double PercentTotalLostTime;
	double PercentAverageHostUtilisation;
	double PercentMaxHostUtilisation;
	double PercentMinHostUtilisation;
	double PercentSdHostUtilisation;
	double AverageNumberHosts;
	double MaxNumberHosts;
	double MinNumberHosts;
	double SdNumberHosts;
	double AverageFreeHosts;
	double MaxFreeHosts;
	double MinFreeHosts;
	double SdFreeHosts;
	double AverageGlobalQueueLength;
	double MaxGlobalQueueLength;
	double MinGlobalQueueLength;
	double SdGlobalQueueLength;
	double AverageWaitTime;
	double MaxWaitTime;
	double MinWaitTime;
	double SdWaitTime;
	double AverageSchedulesPerTask;
	double MaxSchedulesPerTask;
	double MinSchedulesPerTask;
	double SdSchedulesPerTask;
	double NumberSchedulerCalls;
	double NumberSchedulerActivations;
	double AverageActivationInterval;
	double MaxActivationInterval;
	double MinActivationInterval;
	double SdActivationInterval;
	double MaxEnergy;
	double Eparam;
	double EnergyI;
	double EnergyII;





};

#endif
