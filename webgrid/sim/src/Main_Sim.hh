#include "getopt.h"     // Windows
#include <unistd.h>   // *nix

#include "GlobalStatistics.hh"
#include "GlobalStatisticsSummator.hh"
#include "IndependentRunsStatistics.hh"
#include "GridSimulation.hh"
#include "DistributionFactory.hh"
#include "Distribution.hh"
#include "SchedulerFactory.hh"
#include "GAParams.hh"
#include "GAScheduler.hh"
#include "UtilsPackage.hh"
#include <limits.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

GlobalStatistics run  ( int totalTasks, int initialTasks, string iaTime,
                        string workLoad, int initialHosts, string mips,
                        string addHost, string delHost, int minHosts,
                        int maxHosts, bool reschedule, string strategy,
                        string activation, string hostSelection,
                        string taskSelection, string allocPolicy,
                        long int& seed, bool trace, string traceFile,
                        GAParams ga_params,  GAParams (&hgs_params)[4],
                        TabuParams tabuParams, CMAParams cmaParams, MAParams maParams);

void usage();

int parseArg (int argc, char* argv[], int& totalTasks, int& initialTasks,
              string& iaTime, string& workLoad, int& initialHosts,
              string& mips, string& addHost, string& delHost,int& minHosts,
              int& maxHosts, bool& reschedule, string& strategy,
              string& activation, string& hostSelection,
              string& taskSelection, string& allocPolicy, int& nRuns,
              long int& seed, bool& trace, string& traceFile,
              GAParams& ga_params, GAParams (&hgs_par)[4], TabuParams& tabuParams,
              CMAParams cmaParams, MAParams maParams);

void reportParameters  (int totalTasks, int initialTasks, string iaTime,
                        string workLoad, int initialHosts, string mips,
                        string addHost, string delHost, int minHosts,
                        int maxHosts, bool reschedule, string strategy,
                        string activation, string hostSelection,
                        string taskSelection, string allocPolicy,int nRuns,
                        long int seed, bool trace, string traceFile,
                        GAParams params, GAParams (&hgs_params)[4],
                        TabuParams tabuParams, CMAParams cmaParams, MAParams maParams,
                        ostream& out);

void processMultipleRuns (GlobalStatistics& gs,
                         IndependentRunsStatistics& irs);
