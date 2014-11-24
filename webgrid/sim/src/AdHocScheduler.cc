#include "AdHocScheduler.hh"
//#include "Energy.hh"

AdHocScheduler::AdHocScheduler(string adhoc_heuristic) {
    if (!adhoc_heuristic.compare("min_min")) {
        heuristic = AdHocHeuristics::MinMin_h;
    } else if (!adhoc_heuristic.compare("max_min")) {
        heuristic = AdHocHeuristics::MaxMin_h;
    } else if (!adhoc_heuristic.compare("sufferage")) {
        heuristic = AdHocHeuristics::Sufferage_h;
    } else if (!adhoc_heuristic.compare("relative_cost")) {
        heuristic = AdHocHeuristics::RelativeCost_h;
    } else if (!adhoc_heuristic.compare("ljfr_sjfr")) {
        heuristic = AdHocHeuristics::LJFRSJFR_h;
    } else if (!adhoc_heuristic.compare("mct")) {
        heuristic = AdHocHeuristics::MCT_h;
    } else if (!adhoc_heuristic.compare("met")) {
        heuristic = AdHocHeuristics::MET_h;
    } else if (!adhoc_heuristic.compare("sa")) {
        heuristic = AdHocHeuristics::SA_h;
    } else if (!adhoc_heuristic.compare("kpb")) {
        heuristic = AdHocHeuristics::KPB_h;
    } else if (!adhoc_heuristic.compare("olb")) {
        heuristic = AdHocHeuristics::OLB_h;
    } else if (!adhoc_heuristic.compare("random")) {
        heuristic = AdHocHeuristics::Random_h;        
    } else {
        error("AdHocScheduler::unknown ad-hoc heuristic!");
        heuristic = AdHocHeuristics::Random_h;
    }
}

AdHocScheduler::~AdHocScheduler() {
}

void AdHocScheduler::schedule (hostsSet& hosts, tasksSet& tasks) {    
    using namespace AdHocHeuristics;
    /*-------------------------------------------------------------------*/
    /* The purpouse of this code template is to adapt interfaces         */
    /*-------------------------------------------------------------------*/
    int nbr_tasks = tasks.size();   if (nbr_tasks == 0) return;
    int nbr_hosts = hosts.size();   if (nbr_hosts == 0) return;
    
    matrix<double>  etc          (nbr_tasks, nbr_hosts);
    vector<double>  workload     (nbr_tasks);
    vector<double>  mips         (nbr_hosts);
    vector<double>  ready        (nbr_hosts);
            
    for (int t = 0; t < nbr_tasks; ++t) {
        for (int m = 0; m < nbr_hosts; ++m) {
            etc[t][m] = hosts[m]->getEstimatedExecutionTime(tasks[t]);
    }   }
    
    for (int m = 0; m < nbr_hosts; ++m) {
        ready[m] = hosts[m]->getExpectedReadyTime();
        mips [m] = hosts[m]->getMips();
    } 
    /* TODO: To avoid makespan and flowtimes overflow, substract minimum */
    /*       ready time for every ready time ...                         */
    
    for (int t = 0; t < nbr_tasks; ++t) {
        workload[t] = tasks[t]->getWorkLoad();
    }
     	 	
 	AdHocHeuristics::InitialSolutions is;
    is.set_problem (workload, mips, ready);
    is.set_problem (etc, ready);
        
    /*-------------------------------------------------------------------*/
    /* Now we execute the AdHocHeuristic                                 */
    /*-------------------------------------------------------------------*/
     vector<int> sch = is.heuristic (heuristic);
        
    /*-------------------------------------------------------------------*/
    /* Transform and apply the schedule from AdHocHeuristics             */
    /*-------------------------------------------------------------------*/
    for (int t = 0; t < nbr_tasks; ++t) {
        plan[hosts[sch[t]]].push_back(tasks[t]);
        assignHost(tasks[t], hosts[sch[t]]);
    }

    //Energy* energy = new Energy(nbr_hosts, nbr_tasks);
	//energy->assignEnergy(hosts,solution.local_makespan,solution.completion,problem.ready_times);
	MaxEnergy = 0;
	EnergyI = 0;
	EnergyII = 0;
	Eparam = 0;
}
