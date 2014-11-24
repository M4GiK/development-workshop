#ifndef __TASK_H__
#define __TASK_H__
class Task;

#include "EventListener.hh"
#include "TaskRequirements.hh"
#include "Host.hh"
#include "UtilsPackage.hh"

typedef TaskRequirements Requirements;

enum TaskState {
    STATE_UNASSIGN,     /* waiting for assignment  */
    STATE_SCHEDULED,    /* being scheduled         */
    STATE_WAIT,         /* waiting for dispatching */
    STATE_RUN,          /* running                 */
    STATE_FINISH,       /* finish                  */
};

class Task : public EventListener{
    friend class Host;
    friend class GridSimulation;
    friend class Scheduler;

    public:
        Task (double workLoad, double submitTime);
        virtual ~Task();

        /*  Get methods  */
        int        getTid();
        double     getWorkLoad();
        int        getState();
        Host*      getHost();
        double	   getSd();

        /* Statistics methods (time related) and other get methods */
        double     getSubmitTime();
        double     getStartTime();
        double     getFinishTime();
        double     getExecutionTime ();
        double     getEstimatedExecutionTime();
        int        getNumSchedules();
        Requirements*  getRequirements ();

        //double   getInputSize();
        //double   getExeSize();
        //double   getOutputSize();
        //double   getAccumulatedCost();
        //double   getStageTime    (Host* src, Host* dst);
        //double   getStageInTime  (Host* src, Host* dst);
        //double   getStageExeTime (Host* src, Host* dst);
        //double   getStageOutTime (Host* src, Host* dst);
        //void*    getData();
        //double   getRemainTime();
        //double   getRemainWorkLoad();

    protected:
        /* Task methods */
        bool   runTask    ();
        bool   startTask  ();
        bool   finishTask ();
        bool   endTask    ();
        bool   stopTask   ();
        bool   pauseTask  ();
        bool   cancelTask ();
        bool   resumeTask ();

        /* Task configuration methods */
        void   setRequirements (Requirements* requirements);
        void   setState  (int state);
        void   setHost   (Host* host);
        void   unsetHost ();

        /* Other methods */
        static  int  getNextTid ();
        static  void resetTid   ();

        int               tid;              /* Task id                  */
        double            totalWorkLoad;    /* Work load                */
        int               state;            /* Task state               */
        Host*             host;             /* Target host              */
        double            submitTime;       /* Submit time              */
        double            startTime;        /* Start time               */
        double            finishTime;       /* Finish time              */
        double            exeTime;          /* Execution time           */
        int               numSchedules;     /* Number times scheduled   */
        Requirements*     requirements;     /* Task requirements        */
        static int        nextTid;          /* Next task id             */
        double         	  sd;               /* Security Demand          */

        //unsigned long   inputSize;
        //unsigned long   exeSize;
        //unsigned long   outputSize;
        //void*           data;
        //double          accumulatedCost;
};
#endif
