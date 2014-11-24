#include "TSScheduler.hh"
#include "TabuSearch.hh"
#include "Security.hh"
#include "Energy.hh"

#define E        (2.7182818284590452354)
#define log_2(x) (log(x)/log(2.0))
#define ln(x)    (log(x)/log(E))

TSScheduler::TSScheduler(double secs, bool h_s, TabuParams params) {
    if (secs <= 0) {
        error("TSScheduler: wrong max_time_to_spend value!");                                
        secs = 12;
    }
    this->params = params;
    seconds = secs;
    h_or_s = h_s;
}

TSScheduler::~TSScheduler() {
}

void TSScheduler::schedule (hostsSet& hosts, tasksSet& tasks) {    
    using namespace TabuSearch;
    /*-------------------------------------------------------------------*/
    /* The purpouse of this code template is to adapt skeleton interfaces*/
    /* with the interface and behaviour of SchedulingPolicy              */                   
    /*-------------------------------------------------------------------*/
    int nbr_tasks = tasks.size();   if (nbr_tasks == 0) return;
    int nbr_hosts = hosts.size();   if (nbr_hosts == 0) return;
    
    matrix<double>  etc          (nbr_tasks, nbr_hosts);
    vector<double>  workload     (nbr_tasks);
    vector<double>  mips         (nbr_hosts);
    vector<double>  ready        (nbr_hosts);
    double          now        = getSimulationTime();
            
    for (int t = 0; t < nbr_tasks; ++t) {
        for (int m = 0; m < nbr_hosts; ++m) {
            etc[t][m] = hosts[m]->getEstimatedExecutionTime(tasks[t]);
    }   }
    
    /* <security> */

	if(params.security)
	{
		Security* security = new Security(nbr_hosts, nbr_tasks);
		security->getTl(hosts);
		security->getSd(tasks);
		security->ApplySecurity(etc,params.security_param);
	}

	/* </security> */

    for (int m = 0; m < nbr_hosts; ++m) {
        ready[m] = hosts[m]->getExpectedReadyTime();
        mips [m] = hosts[m]->getMips();
    } 
    /* TODO: To avoid makespan and flowtimes overflow, substract minimum */
    /*       ready time for every ready time ...                         */
    
    for (int t = 0; t < nbr_tasks; ++t) {
        workload[t] = tasks[t]->getWorkLoad();
    }
    
 	TabuSearch::Problem problem;
    problem.set_problem (workload, mips, ready, now);
    problem.set_problem (etc, ready, now);

    cout << "params.max_nb_swaps " << params.max_nb_swaps << endl;
    cout << "params.max_nb_transfers " << params.max_nb_transfers << endl;
    cout << "params.max_load_interval " << params.max_load_interval << endl;
    cout << "params.percent_min_load " << params.percent_min_load << endl;
        
    /*-------------------------------------------------------------------*/
    /* Setup tunning is specific for each skeleton type (parameters and  */
    /* their tunning may be different)...                                */
    /*-------------------------------------------------------------------*/    
    // Default fine tunning ... 
      TabuSearch::Setup setup;
      setup.set_optimization_strategy        (h_or_s);
      setup.set_independent_runs             (1);
      setup.set_max_time_to_spend            (seconds);

      setup.set_nb_iterations                (params.nb_iterations);
      setup.set_start_choice                 (params.start_choice);

      setup.set_use_delta_function           (params.use_delta_function);   //not used (?)
      setup.set_tabu_size                    (params.tabu_size);
      setup.set_max_tabu_status              (params.max_tabu_status);
      setup.set_min_tabu_status              (params.min_tabu_status);  //not used

      setup.set_max_repetitions              (params.max_repetitions); // max_reps
      setup.set_nb_intensifications          (params.nb_intensifications);
      setup.set_nb_diversifications          (params.nb_diversifications);
      setup.set_elite_size                   (params.elite_size);

      setup.set_aspiration_value             (params.aspiration_value);

      setup.set_max_nb_swaps                 (params.max_nb_swaps);
      setup.set_max_nb_transfers             (params.max_nb_transfers);
      setup.set_max_load_interval            (params.max_load_interval);
      setup.set_percent_min_load             (params.percent_min_load);
    
    /*-------------------------------------------------------------------*/
    /* Now we construct and execute the skeleton solver (provided class) */
    /*-------------------------------------------------------------------*/
    TabuSearch::Solver_Seq solver (problem, setup);
    solver.run ();
        
    /*-------------------------------------------------------------------*/
    /* Gather the results (schedule) from solver,and transform+apply them*/
    /*-------------------------------------------------------------------*/
    TabuSearch::Solution solution = solver.best_solution ();
    vector<int> sch = solution.schedule;

    for (int t = 0; t < nbr_tasks; ++t) {
        plan[hosts[sch[t]]].push_back(tasks[t]);
        assignHost(tasks[t], hosts[sch[t]]);
    }

	Energy* energy = new Energy(nbr_hosts, nbr_tasks);
	energy->assignEnergy(mips,solution.local_makespan,solution.completions,problem.ready_times);
	MaxEnergy = energy->MaxEnergy;
	EnergyI = energy->EnergyI;
	EnergyII = energy->EnergyII;
	Eparam = energy->Eparam;
}
