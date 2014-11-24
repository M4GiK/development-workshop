









#include "GlobalStatisticsSummator.hh"

#include <stdio.h>
#include <iostream>
#include <fstream>
using namespace std;


void GlobalStatisticsSummator::pushBack(GlobalStatistics& gs){

	num_pushed++;

	 Makespan   += gs.getMakespan();
	 FlowTime   += gs.getFlowTime();
	 TotalPotentialTime   += gs.getTotalPotentialTime();
	 PercentTotalIdleTime   += gs.getPercentTotalIdleTime();
	 PercentTotalBusyTime   += gs.getPercentTotalBusyTime();
	 PercentTotalLostTime   += gs.getPercentTotalLostTime();
	 PercentAverageHostUtilisation   += gs.getPercentAverageHostUtilisation();
	 PercentMaxHostUtilisation   += gs.getPercentMaxHostUtilisation();
	 PercentMinHostUtilisation   += gs.getPercentMinHostUtilisation();
	 PercentSdHostUtilisation   += gs.getPercentSdHostUtilisation();
	 AverageNumberHosts   += gs.getAverageNumberHosts();
	 MaxNumberHosts   += gs.getMaxNumberHosts();
	 MinNumberHosts   += gs.getMinNumberHosts();
	 SdNumberHosts   += gs.getSdNumberHosts();
	 AverageFreeHosts   += gs.getAverageFreeHosts();
	 MaxFreeHosts   += gs.getMaxFreeHosts();
	 MinFreeHosts   += gs.getMinFreeHosts();
	 SdFreeHosts   += gs.getSdFreeHosts();
	 AverageGlobalQueueLength   += gs.getAverageGlobalQueueLength();
	 MaxGlobalQueueLength   += gs.getMaxGlobalQueueLength();
	 MinGlobalQueueLength   += gs.getMinGlobalQueueLength();
	 SdGlobalQueueLength   += gs.getSdGlobalQueueLength();
	 AverageWaitTime   += gs.getAverageWaitTime();
	 MaxWaitTime   += gs.getMaxWaitTime();
	 MinWaitTime   += gs.getMinWaitTime();
	 SdWaitTime   += gs.getSdWaitTime();
	 AverageSchedulesPerTask   += gs.getAverageSchedulesPerTask();
	 MaxSchedulesPerTask   += gs.getMaxSchedulesPerTask();
	 MinSchedulesPerTask   += gs.getMinSchedulesPerTask();
	 SdSchedulesPerTask   += gs.getSdSchedulesPerTask();
	 NumberSchedulerCalls   += gs.getNumberSchedulerCalls();
	 NumberSchedulerActivations   += gs.getNumberSchedulerActivations();
	 AverageActivationInterval   += gs.getAverageActivationInterval();
	 MaxActivationInterval   += gs.getMaxActivationInterval();
	 MinActivationInterval   += gs.getMinActivationInterval();
	 SdActivationInterval   += gs.getSdActivationInterval();
	 MaxEnergy += gs.getMaxEnergy();
	 Eparam += gs.getEparam();
	 EnergyI += gs.getEnergyI();
	 EnergyII += gs.getEnergyII();


}



void GlobalStatisticsSummator::outputStatistics(ostream& out) {
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
    Makespan/num_pushed,
    FlowTime/num_pushed,
    TotalPotentialTime/num_pushed,            PercentTotalIdleTime/num_pushed,
    PercentTotalBusyTime/num_pushed,          PercentTotalLostTime/num_pushed,
    PercentAverageHostUtilisation/num_pushed, PercentMaxHostUtilisation/num_pushed,
    PercentMinHostUtilisation/num_pushed,     PercentSdHostUtilisation/num_pushed,
    AverageNumberHosts/num_pushed,            MaxNumberHosts/num_pushed,
    MinNumberHosts/num_pushed,                SdNumberHosts/num_pushed,
    AverageFreeHosts/num_pushed,              MaxFreeHosts/num_pushed,
    MinFreeHosts/num_pushed,                  SdFreeHosts/num_pushed,
    AverageGlobalQueueLength/num_pushed,      MaxGlobalQueueLength/num_pushed,
    MinGlobalQueueLength/num_pushed,          SdGlobalQueueLength/num_pushed,
    AverageWaitTime/num_pushed,               MaxWaitTime/num_pushed,
    MinWaitTime/num_pushed,                   SdWaitTime/num_pushed,
    AverageSchedulesPerTask/num_pushed,       MaxSchedulesPerTask/num_pushed,
    MinSchedulesPerTask/num_pushed,           SdSchedulesPerTask/num_pushed,
    NumberSchedulerCalls/num_pushed,          NumberSchedulerActivations/num_pushed,
    AverageActivationInterval/num_pushed,     MaxActivationInterval/num_pushed,
    MinActivationInterval/num_pushed,         SdActivationInterval/num_pushed,
    EnergyI/num_pushed,						  EnergyII/num_pushed,
    MaxEnergy/num_pushed,					  Eparam/num_pushed
    );

  out << buffer;
  delete[] buffer;
}


