#ifndef __INDEPENDENT_RUNS_STATISTICS_H__
#define __INDEPENDENT_RUNS_STATISTICS_H__
class IndependentRunsStatistics;

#include "Accumulator.hh"
#include <stdio.h>
#include <iostream>
#include <fstream>
using namespace std;

class IndependentRunsStatistics {
    public:
        IndependentRunsStatistics ();
        virtual ~IndependentRunsStatistics();

        /* Set Methods */
        void addMakespan (double makespan);
        void addFlowTime (double flowtime);
        void addUtilisation (double utilisation);

        /* Get Methods */
        double getNumberIndependentRuns ();
        double getAverageMakespan ();
        double getMinMakespan();
        double getMaxMakespan();
        double getSdMakespan();
        double getConfidenceMakespan (double confidence = 0.95);
        double getAverageFlowTime ();
        double getMinFlowTime();
        double getMaxFlowTime();
        double getSdFlowTime();
        double getConfidenceFlowTime (double confidence = 0.95);
        double getAverageUtilisation();
        double getMinUtilisation();
        double getMaxUtilisation();
        double getSdUtilisation();
        double getConfidenceUtilisation (double confidence = 0.95);

        /* Output Methods */
        void outputStatistics(ostream& out = std::cout);

    private:
        Accumulator  makespan;      /* Makespan accumulator    */
        Accumulator  flowtime;      /* Flowtime accumulator    */
        Accumulator  utilisation;   /* Utilisation accumulator */
};
#endif
