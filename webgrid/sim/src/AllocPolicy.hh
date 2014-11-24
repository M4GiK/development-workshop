#ifndef __ALLOC_POLICY_H__
#define __ALLOC_POLICY_H__
class AllocPolicy;

#include "EventListener.hh"
#include "Host.hh"
#include "Task.hh"
#include "GridSimulation.hh"
#include <vector>
using namespace std;

class AllocPolicy : public EventListener {
    friend class Host;
    public:
        AllocPolicy ();
        virtual ~AllocPolicy();

        /* Host get methods that will be implemented in a subclass */
        virtual double          getExecutionTime (Task* task) = 0;
        virtual double          getEstimatedExecutionTime (Task* task) = 0;
        virtual vector<Task*>   getExecutingTasks () = 0;
        virtual Task*           getTaskById (int tid) = 0;
        virtual vector<Task*>   getAllTasks () = 0;
        virtual int             getNumberTasks (bool exec) = 0;
        virtual int             getQueueLength () = 0;

    protected:
        /* Alloc Policy configuration methods */
        virtual void    setSource (Host* host) = 0;

        /* Host methods that will be implemented in a subclass */
        virtual void   transferTasks (vector<Task*>& tasks) = 0;
        virtual void   getRidOfTasks (bool include_executing= false) = 0;
        virtual bool   runTask    (Task* task) = 0;
        virtual bool   finishTask (Task* task) = 0;
        virtual bool   stopTask   (Task* task) = 0;
        virtual bool   pauseTask  (Task* task) = 0;
        virtual bool   cancelTask (Task* task) = 0;
        virtual bool   resumeTask (Task* task) = 0;

        /* Methods to allow friend access modifier */
        void             setHostState (int state);
        void             setHostExpectedReadyTime (double time);
        GridSimulation*  getSimulator ();
        void             transferTasksToSimulator (vector<Task*>& task);
        void             transferTaskToSimulator  (Task* task);
        bool             taskRun    (Task* task);
        bool             taskFinish (Task* task);
        bool             taskStop   (Task* task);
        bool             taskPause  (Task* task);
        bool             taskCancel (Task* task);
        bool             taskResume (Task* task);

        Host*              host;        /*     Host reference         */
};
#endif
