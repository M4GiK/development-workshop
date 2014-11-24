#ifndef __HOST_H__
#define __HOST_H__
class Host;

#include "EventListener.hh"
#include "AllocPolicy.hh"
#include "HostCharacteristics.hh"
#include "Task.hh"
#include "GridSimulation.hh"
#include "Grid.hh"
#include <vector>
using namespace std;

typedef HostCharacteristics HCharacteristics;

enum HostState {
    STATE_BUSY,         /* working       */
    STATE_IDLE,         /* idle, free    */
    STATE_STOP,         /* being stopped */
};

class Host : public EventListener {
    friend class AllocPolicy;
    friend class Task;
    friend class Grid;
    friend class GridSimulation;
    friend class Scheduler;
    public:
        Host (double mips, GridSimulation* sim);
        Host (double mips, AllocPolicy* allocPolicy, GridSimulation* sim);
        virtual ~Host();

        /*  Get methods  */
        int       getHid   ();
        double    getMips  ();
        int       getState ();
        double 	  getTl    ();

        /* Time statistics methods */
        double    getExpectedReadyTime ();
        double    getWakeUpTime   ();
        double    getShutDownTime ();
        double    getTotalTime ();
        double    getBusyTime ();
        double    getIdleTime ();
        double    getLostTime ();

        /* Get methods (most of them related task information) */
        HCharacteristics* getCharacteristics();
        double         getExecutionTime (Task* task);
        double         getEstimatedExecutionTime (Task* task);
        vector<Task*>  getExecutingTasks ();
        Grid*          getParent ();
        Task*          getTaskById (int tid);
        vector<Task*>  getAllTasks ();
        int            getNumberTasks (bool exec);
        int            getQueueLength ();

        //void      setName (string& name);
        //string    getName();
        //double    getCostRate (double simTime);
        //double    getBandwidth   (Host* dst);
        //double    getStageInTime (Host* src, Task* task);
        //double    getStageExeTime(Host* src, Task* task);
        //double    getStageOutTime(Host* src, Task* task);

    protected:
        /* Host configuration methods */
        void   setAllocPolicy (AllocPolicy* allocPolicy);
        void   setCharacteristics (HCharacteristics* hCharacter);
        void   setParent (Grid* grid);

        /*  Host  methods  */
        void   transferTasks (vector<Task*>& tasks);
        void   stopHost();
        void   getRidOfTasks (bool include_executing= false);
        bool   runTask (Task* task);
        bool   finishTask (Task* task);
        bool   stopTask   (Task* task);
        bool   pauseTask  (Task* task);
        bool   cancelTask (Task* task);
        bool   resumeTask (Task* task);

        /* Methods to allow friend access modifier be inherited ... */
        void   setState (int state);
        void   setExpectedReadyTime (double readyTime);
        void   transferTasksToSimulator (vector<Task*>& tasks);
        void   transferTaskToSimulator  (Task* tasks);
        bool   taskRun    (Task* task);
        bool   taskFinish (Task* task);
        bool   taskStop   (Task* task);
        bool   taskPause  (Task* task);
        bool   taskCancel (Task* task);
        bool   taskResume (Task* task);

        /*  Other Methods  */
        static  int    getNextHid();
        static  void   resetHid();

        int                hid;                /* Host id                */
        double             mips;               /* Mips                   */
        int                state;              /* State                  */
        double             expectedReadyTime;  /* Ready time             */
        double             wakeupTime;         /* Host start time        */
        double             shutdownTime;       /* Host end time          */
        double             busyTime;           /* Total busy time        */
        double             lostTime;           /* Total lost time        */
        GridSimulation*    sim;                /* GridSimulation ref     */
        Grid*              parent;             /* Parent                 */
        AllocPolicy*       allocPolicy;        /* Allocation Policy      */
        HCharacteristics*  hCharacter;         /* Host characteristics   */
        static int         nextHid;            /* Next host id           */
        double         	   tl;                 /* Trust Level            */

        //double                  load;
        //vector<double>          CPUs
        //vector<bool>            CPUsLoad;
        //vector<int>             CPUsState;
        //string                  name;
        //double                  costRate;
        //map<Host*, Connection*> connections;
};
#endif
