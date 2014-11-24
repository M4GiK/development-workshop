#include "Simulation.hh"
#include "DistributionFactory.hh"
#include "Distribution.hh"
#include "EventNode.hh"
#include <deque>
using namespace std;

#define Q_LIMIT   1000
#define BUF_SIZE  1024
#ifndef HUGE_VAL
#define HUGE_VAL  1.0e+30
#endif

#define BUSY      1
#define IDLE      0
#define ARRIVE    1
#define DEPART    2

class MM1Q: public Simulation {
    public:
        MM1Q();
        MM1Q(Distribution* mi,Distribution* ms, int ndr);
        ~MM1Q();

    private:
        virtual void performInit();
        virtual void performProcess(EventNode* event);
        virtual bool isFinish();
        virtual void updateStat(EventNode* event);
        virtual void report();

        void arrive();
        void depart();

        int server_status;
        double time_last_event;
        int num_custs_delayed;
        double total_of_delays;
        double area_num_in_q;
        double area_server_status;
        deque<double> time_arrival;

        Distribution* mean_interarrival;
        Distribution* mean_service;
        int num_delays_required;

        bool no_more;
        bool was_not_working;
};

MM1Q::MM1Q(): Simulation() {
    string st;
    DistributionFactory db;

    cout << "Mean Interarrival Time: ";
    cin >> st;  mean_interarrival = db.CreateDistribution(st);
    cout << "Mean Service Time: ";
    cin >> st;  mean_service = db.CreateDistribution(st);
    cout << "Number of Delays Required: "; cin >> num_delays_required;

    registerEvent(ARRIVE,"ARRIVE");
    registerEvent(DEPART,"DEPART");

    no_more = false;
    was_not_working = false;
}

MM1Q::MM1Q(Distribution* mi,Distribution* ms,int ndr)
    :Simulation() {
    mean_interarrival = mi;
    mean_service = ms;
    num_delays_required = ndr;
}

MM1Q::~MM1Q() {
}

void MM1Q::performInit() {
    /*  initialize the state variable. */
    server_status = IDLE;
    time_last_event = 0;

    /*  initialize the statistical counters. */
    num_custs_delayed = 0;
    total_of_delays = 0.0;
    area_num_in_q = 0.0;
    area_server_status = 0.0;

    /* Initialize event list. Since no customers are present,
     * the departure (service completion) even is eliminated
     * from consideration. */
    scheduleAt(0.0, ARRIVE, 0);
    scheduleAt(HUGE_VAL, DEPART, -1);

    time_arrival.clear();
}

void MM1Q::performProcess(EventNode* event) {
    int event_type = event->eventType;

    switch (event_type) {
        case ARRIVE:
            arrive();
            break;
        case DEPART:
            depart();
            break;
        default:
            break;
    }
}

bool MM1Q::isFinish() {
    return !(num_custs_delayed<num_delays_required);
}

void MM1Q::updateStat(EventNode* event) {
    double time_since_last_event;

    /* Compute the since last event, and update last_event-time maker. */
    time_since_last_event = simTime-time_last_event;
    time_last_event = simTime;

    /* Update area under number-in-queue function. */
    area_num_in_q += time_arrival.size()*time_since_last_event;


    if (was_not_working) { //updateStat indicates an arrive event
        area_server_status += time_since_last_event;
        was_not_working = false; no_more = false;
    }
    if (no_more) { was_not_working = true;}
    no_more = false;


    /* Update area under server-busy idicator function. */
    //area_server_status += server_status*time_since_last_event;
}

void MM1Q::report() {
    /* Compute and write estimates of desired measures of performance. */
    printf("Average delay in queue: %0.2f\n",
            total_of_delays/num_custs_delayed);
    printf("Average number in queue: %0.2f\n", area_num_in_q/simTime);
    printf("Server utilization: %0.2f\n",
                    (simTime-area_server_status)/simTime);
    printf("Time simulation ended: %0.2f\n", simTime);
}

void MM1Q::arrive() {
    double delay;

    /* Schedule next arrival. */
    scheduleAfter(mean_interarrival->generate(simTime), ARRIVE);

    /* Check to see whether server is busy. */
    if(server_status == BUSY){
        /* There is still room in the queue, so store the time of
         * arrival of the arriving customer at the (new) end of
         * time_arrival. */
        time_arrival.push_back(simTime);
    } else {
        /* Server is idle, so arriving customer has a delay of zero.
         * (The following two statements are for program clarity
         * and do not affect the results of the simulation.) */
        delay = 0;
        total_of_delays += delay;

        /* Make server busy. */
        server_status = BUSY;

        /* Schedule a departure (server complete). */
        scheduleAfter(mean_service->generate(simTime), DEPART, -1);
    }
}

void MM1Q::depart() {
    double delay;


    ++num_custs_delayed;

    /* Check to see whether the queue is empty. */
    if (time_arrival.empty()){
        /* The queue is empty so make the server idle and
         * eliminate the departure (service completion) event
         * from consideration. */
        server_status = IDLE;
        no_more = true;
    } else {
        /* Compute the delay of the customer who is beginning
         * service and update the total delay accumulator. */
        delay  = simTime - time_arrival[0];
        total_of_delays += delay;

        /* Schedule departure. */
        scheduleAfter(mean_service->generate(simTime),DEPART, -1);

        /* Move each customer in queue (if any) up one place. */
        time_arrival.pop_front();
    }
}

//int main(int argc,char *argv[]) {
//    MM1Q sim;
//    sim.run();
//    return 0;
//}
