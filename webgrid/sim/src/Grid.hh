#ifndef __GRID_H__
#define __GRID_H__
class Grid;

#include "EventListener.hh"
#include "GridCharacteristics.hh"
#include "Host.hh"
#include "AllocPolicyFactory.hh"
#include "Task.hh"
#include "GridSimulation.hh"
#include "Distribution.hh"
#include "RandomPackage.hh"
#include "UtilsPackage.hh"
#include <vector>
#include <string>
using namespace std;

typedef GridCharacteristics GCharacteristics;

/* This class groups hosts with similar characteristics. The reason for  */
/* this design, is that it may simplify the scheduler or broker behaviour*/
/* as it will be able to work with a medium-grane object.                */
/* Be aware that is should group hosts with similar characteristics,     */
/* otherwise it may confuse the broker or scheduler...                   */

class Grid : public EventListener {
    friend class GridSimulation;
    public:
        Grid (Distribution* mips, Distribution* delHosts,
              Distribution* addHosts, int minHosts, int initialHosts,
              int maxHosts, string strAllocPolicy, GridSimulation* sim);
        virtual ~Grid();

        /* Get methods */
        int               getGid();
        GCharacteristics* getCharacteristics();

        /* Get methods and statistics methods (host-related) */
        int             getNumberHosts();
        int             getNumberIdleHosts();
        int             getNumberBusyHosts();
        void            getUtilisation (double& actual, double& potential);
        vector<Host*>&  getAllHosts();
        vector<Host*>   getIdleHosts();
        vector<Host*>   getBusyHosts();
        Host*           getHostById(int hid);

        /* Get methods and statistics methods (task-related) */
        int             getNumberTasks (bool exec);
        Task*           getTaskById(int tid);

        //string         getName();
        //void           setName (string& name);
        //double         getBandwidth (Grid* dst);
        //vector<Grids*> getConnectedGrids (bool direct = true);
        //void           addDirectConnection (Grid* dst, Connection* conn);

    protected:
        /* Grid configuration methods */
        void   setCharacteristics(GCharacteristics* gCharacter);

        /* Grid methods */
        Host*  addAHost ();
        bool   addHost  (Host* host);
        Host*  stopAHost();
        bool   stopHost (Host* host);

        /* Other methods */
        virtual void  performInit ();
        virtual void  performFinalize ();
        void          createRandomGrid(int numHosts,string strAllocPolicy);
        static int    getNextGid();
        static void   resetGid();

        int                 gid;             /* Grid id                  */
        vector<Host*>       hosts;           /* Host vector              */
        Distribution*       mips;            /* Mips distribution        */
        Distribution*       addHosts;        /* Add hosts distribution   */
        Distribution*       delHosts;        /* Del hosts distribution   */
        int                 initialHosts;    /* Initial number of hosts  */
        int                 minHosts;        /* Minimum number of hosts  */
        int                 maxHosts;        /* Maximum number of hosts  */
        string              strAllocPolicy;  /* Hosts alloc policy type  */
        GridSimulation*     sim;             /* Grid Simulation reference*/
        GCharacteristics*   gCharacter;      /* Grid dynamic character   */
        static int          nextGid;         /* Next grid id             */

        //string                  name;
        //map<Grid*,Connection*>  connections;
};
#endif
