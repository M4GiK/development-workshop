#ifndef __GRID_SIMULATION_H__
#define __GRID_SIMULATION_H__
class GridSimulation;

#include "Simulation.hh"
#include "EventNode.hh"
#include "Grid.hh"
#include "Host.hh"
#include "Task.hh"
#include "Distribution.hh"
#include "GlobalStatistics.hh"
#include "Scheduler.hh"
#include "UtilsPackage.hh"
#include <string>
#include <vector>
using namespace std;

#define EVN_ENTER            0
#define EVN_START            1
#define EVN_FINISH           2
#define EVN_SCHEDULE         3
#define EVN_NEW_HOST         4
#define EVN_STOP_HOST        5

/* More restrictive */
//#define EVN_ENTER_PRIO       0
//#define EVN_START_PRIO      -1
//#define EVN_FINISH_PRIO     -5
//#define EVN_SCHEDULE_PRIO   -2
//#define EVN_NEW_HOST_PRIO   -4
//#define EVN_STOP_HOST_PRIO  -3
#define EVN_ENTER_PRIO		-2
#define EVN_START_PRIO		-1
#define EVN_FINISH_PRIO		-5
#define EVN_SCHEDULE_PRIO	 0
#define EVN_NEW_HOST_PRIO	-4
#define EVN_STOP_HOST_PRIO	-3

class GridSimulation: public Simulation {
    friend class Scheduler;
    friend class Host;
    public:
        GridSimulation ();
        virtual ~GridSimulation ();

        /* Grid Simulation configuration methods */
        void addGrid (Distribution* mips, Distribution* delHost,
                      Distribution* addHost,int minHosts, int initialHosts,
                      int maxHosts, string allocPolicy);
        void setTasks (Distribution* iaTime, Distribution* workLoad,
                       int totalTasks, int initialTasks = 0);
        void setScheduler (Scheduler* scheduler);
        //int setTasks (string& taskfile);
        //int setGrids (string& gridfile);

        /* Get methods */
        GlobalStatistics& getGlobalStatistics();
        vector<Grid*>&  getGrids ();
        Grid*           getGridById (int gid);
        Task*           getTaskById (int tid);
        Host*           getHostById (int hid);
        vector<Host*>   getAllHosts();
        vector<Host*>   getAllBusyHosts();
        vector<Host*>   getAllIdleHosts();
        vector<Task*>&  getTasksToSchedule();
        int             getNumberGrids();
        int             getTotalNumberHosts();
        int             getTotalNumberIdleHosts();
        int             getTotalNumberBusyHosts();
        int             getNumberTasksToSchedule();
        int             getTotalNumberTasks(bool exec);
        double          getTotalUtilisation ();

    protected:
        /*Implementation of EventListener and Simulation abstract methods*/
        virtual bool  isFinish ();
        virtual void  updateStat (EventNode* event);
        virtual void  report ();
        virtual void  performInit ();
        virtual void  performProcess (EventNode* event);
        virtual void  performFinalize ();

        /* Auxiliar methods */
        Task*         createNewTask();

        /* GridSimulation methods (broker methods) */
        void          transferTasks(vector<Task*>& tsks);
        void          transferTask (Task* tsk);
        vector<Task*> pickUpTasks ();

        /* Dispatching methods */
        void  processEnter    (EventNode* event);
        void  processStart    (EventNode* event);
        void  processFinish   (EventNode* event);
        void  processSchedule (EventNode* event);
        void  processNewHost  (EventNode* event);
        void  processStopHost (EventNode* event);

        /*  Statistics Methods */
        void  updateStatisticsEnter    (Task* task);
        void  updateStatisticsStart    (Task* task);
        void  updateStatisticsFinish   (Task* task);
        void  updateStatisticsSchedule ();
        void  updateStatisticsNewHost  (Host* host);
        void  updateStatisticsStopHost (Host* host);

        void  updateQueueLength (int includeTasks = 0);
        void  updateNumberFreeHosts();
        void  updateNumberHosts();
        void  updateUtilisation();
        void  updateFinishedTaskStatistics (Task* task);
        void  updateSchedulingStatistics ();
        void  updateHostFinalizationStatistics (Host* host);
        void  updateContinuousStatistics();
        void  updateEnergy();

        /*  Trace Methods */
        void  traceInitSimulation ();
        void  traceEnter    (Task* task);
        void  traceStart    (Task* task);
        void  traceFinish   (Task* task);
        void  traceSchedule ();
        void  traceNewHost  (Host* host);
        void  traceStopHost (Host* host);
        void  traceFinalizeSimulation ();

        int               numTasks;        /* Total number of tasks      */
        int               remainTasks;     /* Remaining number of tasks  */
        int               finishedTasks;   /* Number of finished tasks   */
        Distribution*     iaTime;          /* Task interarrival distrib. */
        Distribution*     workLoad;        /* Task workload distribution */
        vector<Task*>     tasks;           /* Tasks waiting a schedule   */
        vector<Grid*>     grids;           /* Grids in the system        */
        Scheduler*        scheduler;       /* Scheduler                  */
        GlobalStatistics  statistics;      /* Simulation global stats    */

        //string          taskfile;
        //string          gridfile;
};
#endif
