#ifndef __SPACE_SHARE_POLICY_H__
#define __SPACE_SHARE_POLICY_H__
class SpaceSharePolicy;

#include "AllocPolicy.hh"
#include "Task.hh"
#include "Host.hh"
#include "GridSimulation.hh"
#include <vector>
using namespace std;

class SpaceSharePolicy : public AllocPolicy {
    public:
        SpaceSharePolicy();
        virtual ~SpaceSharePolicy();

        /* Implementation of Alloc Policy get methods that perform  */
        /* Host get methods                                         */
        virtual double         getExecutionTime (Task* task);
        virtual double         getEstimatedExecutionTime (Task* task);
        virtual vector<Task*>  getExecutingTasks();
        virtual vector<Task*>  getAllTasks ();
        virtual int            getNumberTasks (bool exec);
        virtual int            getQueueLength ();
        virtual Task*          getTaskById (int tid);
        virtual double         getNextFinishTime ();

    protected:
        /* Implementation of Alloc Policy configuration methods */
        virtual void  setSource (Host* host);

        /* Implementation of Alloc Policy methods that perform Host */
        /* specific methods                                         */
        virtual void  transferTasks (vector<Task*>& tasks);
        virtual void  dispatchNext();
        virtual void  getRidOfTasks (bool include_executing= false);
        virtual bool  runTask (Task* task);
        virtual bool  finishTask (Task* task);
        virtual bool  stopTask   (Task* task);
        virtual bool  pauseTask  (Task* task);
        virtual bool  cancelTask (Task* task);
        virtual bool  resumeTask (Task* task);
        virtual void  setNextFinishTime (double time);

        /* Other methods */
        virtual void  specificSchedule () = 0;

        Task*              executing;          /* Running task           */
        vector<Task*>      scheduled;          /* Tasks in queue         */
        double             nextFinishTime;     /* Current task end time  */
};
#endif
