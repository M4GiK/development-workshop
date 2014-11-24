#include "SimpleMetaPolicy.hh"

SimpleMetaPolicy::SimpleMetaPolicy() {
}

SimpleMetaPolicy::~SimpleMetaPolicy() {
}

void SimpleMetaPolicy::schedule (hostsSet& hosts, tasksSet& tasks) {

    int numHosts = hosts.size();
    int numTasks = tasks.size();

    /* Init the schedule_ structure */
    for (int i = 0; i < numHosts; ++i){
        plan[hosts[i]] = vector<Task*> ();
    }

    /* Performs the schedule */
    int m = 0;
    for (int i = 0; i < numTasks; ++i) {
        plan[hosts[m]].push_back (tasks[i]);
        assignHost(tasks[i], hosts[m]);
        m++; m = m % numHosts;
    }
}
