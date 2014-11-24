#ifndef __GLOBAL_STATISTICS_H__
#define __GLOBAL_STATISTICS_H__
class GlobalStatistics;

#include "Accumulator.hh"
#include <stdio.h>
#include <iostream>
#include <fstream>
using namespace std;

class GlobalStatistics {
    friend class GridSimulation;
    public:
        GlobalStatistics ();
        virtual ~GlobalStatistics();

        /* Get Methods */
        double getMakespan ();
        double getFlowTime ();
        double getTotalPotentialTime();
        double getTotalIdleTime();
        double getPercentTotalIdleTime();
        double getTotalBusyTime();
        double getPercentTotalBusyTime();
        double getTotalLostTime();
        double getPercentTotalLostTime();
        double getPercentAverageHostUtilisation();
        double getPercentMaxHostUtilisation();
        double getPercentMinHostUtilisation();
        double getPercentSdHostUtilisation();
        double getAverageNumberHosts();
        double getMaxNumberHosts();
        double getMinNumberHosts();
        double getSdNumberHosts();
        double getAverageFreeHosts();
        double getMaxFreeHosts();
        double getMinFreeHosts();
        double getSdFreeHosts();
        double getAverageGlobalQueueLength();
        double getMaxGlobalQueueLength();
        double getMinGlobalQueueLength();
        double getSdGlobalQueueLength();
        double getAverageWaitTime();
        double getMaxWaitTime();
        double getMinWaitTime();
        double getSdWaitTime();
        double getAverageSchedulesPerTask();
        double getMaxSchedulesPerTask();
        double getMinSchedulesPerTask();
        double getSdSchedulesPerTask();
        double getNumberSchedulerCalls();
        double getNumberSchedulerActivations();
        double getAverageActivationInterval();
        double getMaxActivationInterval();
        double getMinActivationInterval();
        double getSdActivationInterval();
        double getMaxEnergy();
        double getEnergyI();
        double getEnergyII();
        double getEparam();


        /* Output methods */
        void outputStatistics(ostream& out = std::cout);
        //void outputQueueDynamics(ostream& out);
        //void outputTotalHostsDynamics(ostream& out);
        //void outputFreeHostsDynamics(ostream& out);
        //void outputUtilisationDynamics(ostream& out);
        //void outputSchedulingDynamics(ostream& out);
        //void outputGlobalHistory(ostream& out);
        //void setOutputFormat (string& format); //xml, html, txt...

    private:
        /* Set methods */
        void addTaskFinishTime (double time);
        void increaseTotalTime (double time);
        void increaseBusyTime  (double time);
        void increaseIdleTime  (double time);
        void increaseLostTime  (double time);
        void addUtilisation    (double utilisation, double time);
        void addNumberHosts    (double numberHosts, double time);
        void addNumberFreeHosts(double numberFreeHosts, double time);
        void addQueueLength    (double queueLength, double time);
        void addWaitTime       (double waitTime);
        void addSchedulesPerTask    (double schedules);
        void increaseSchedulerCalls ();
        void addSchedulerActivationInterval (double interval);
        void setEnergy(double _MaxEnergy, double _EnergyI, double _EnergyII, double _Eparam);

        double       totalTime;           /* Total computing time */
        double       totalBusyTime;       /* Total useful computing time */
        double       totalIdleTime;       /* Total idle computing time */
        double       totalLostTime;       /* Total lost computing time */
        double       schedulerCalls;      /* Total calls to the scheduler*/

        double		 MaxEnergy;
        double		 EnergyI;
        double		 EnergyII;
        double		 Eparam;

        Accumulator  makespan_flowtime;   /* Makespan, flowtime */
        Accumulator  utilisation;         /* Continued utilisation */
        Accumulator  totalHosts;          /* Continued total hosts */
        Accumulator  freeHosts;           /* Continued idle hosts */
        Accumulator  queueLength;         /* Continued queue lenght */
        Accumulator  waitTime;            /* Wait time accumulator */
        Accumulator  schedulesPerTask;    /* Schedules per task */
        Accumulator  activationInterval;  /* Scheduling interval */

        //History    globalHistory;
        //Dynamics   queueDynamics;
        //Dynamics   totalHostDynamics;
        //Dynamics   freeHostDynamics;
        //Dynamics   schedulingDynamics;  // {<time,num_tasks>}
        //Dynamics   utilisationDynamics;
};
#endif
