#include "NGAScheduler.hh"
#include "NGeneticSearch.hh"
#include "Security.hh"
#include "Energy.hh"

#define log_2(x) (log(x)/log(2.0))

NGAScheduler::NGAScheduler(double secs, bool sga, GAParams params) {
    if (secs <= 0) {
        error("GAScheduler: wrong max_time_to_spend value!");                                
        secs = 12;
    }
    this->params = params;
    seconds = secs;
    struggle = sga;
}

NGAScheduler::~NGAScheduler() {
}

void NGAScheduler::schedule (hostsSet& hosts, tasksSet& tasks) {
    using namespace NGeneticSearch;
    /*-------------------------------------------------------------------*/
    /* The purpouse of this code template is to adapt skeleton interfaces*/
    /* with the interface and behaviour of SchedulingPolicy              */                   
    /*-------------------------------------------------------------------*/
    int nbr_tasks = tasks.size();   if (nbr_tasks == 0) return;
    int nbr_hosts = hosts.size();   if (nbr_hosts == 0) return;
    cout<<"NGA Scheduler working"<<endl;
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
    
 	NGeneticSearch::Problem problem;
    problem.set_problem (workload, mips, ready, now);
    problem.set_problem (etc, ready, now);
        
    /*-------------------------------------------------------------------*/
    /* Setup tunning is specific for each skeleton type (parameters and  */
    /* their tunning may be different)...                                */
    /*-------------------------------------------------------------------*/    
    
	NGeneticSearch::Setup setup;
	//--------------------------- Struggle / Steady-State -----------------------------
	setup.set_nb_independent_runs          (1);
	setup.set_max_time_to_spend            (seconds);
	//if (struggle) setup.set_nb_evolution_steps (10 * nbr_tasks);
	//else setup.set_nb_evolution_steps          (5 * nbr_tasks);
	//int pop = int( abs( 2*(ceil(log_2(nbr_tasks))-1) ) ); pop = max(pop,2);
	setup.set_population_size              (params.pop_size);
	//int ipop = int(ceil(0.3*pop));         ipop=max(ipop, 1);
	setup.set_intermediate_population_size (params.intermediate_size);
	setup.set_cross_probability            (params.prob_cross);
	setup.set_mutate_probability           (params.prob_mutate);
	setup.set_start_choice                 (params.start_choice);
	setup.set_select_choice                (params.select_choice);
	setup.set_select_extra_parameter       (params.select_extra);
	setup.set_cross_choice                 (params.cross_choice);
	setup.set_cross_extra_parameter        (params.cross_extra);
	setup.set_mutate_choice                (params.mutate_choice);
	setup.set_mutate_extra_parameter       (params.mutate_extra);
	setup.set_replace_only_if_better       (params.replace_if_better);
	setup.set_replace_generational         (params.replace_generational);
	setup.set_struggle_replace	           (params.struggle_replace);
	setup.set_struggle_extra_parameter     (params.struggle_extra_parameter);
	setup.set_energy_case                  (params.energy);
	//---------------------------------------------------------------------
    
    /*-------------------------------------------------------------------*/
    /* Now we construct and execute the skeleton solver (provided class) */
    /*-------------------------------------------------------------------*/
    NGeneticSearch::Solver_Seq solver (problem, setup);
    solver.run ();
        
    /*-------------------------------------------------------------------*/
    /* Gather the results (schedule) from solver,and transform+apply them*/
    /*-------------------------------------------------------------------*/
    NGeneticSearch::Solution solution = solver.best_Mksp_solution ();
    vector<int> sch = solution.schedule;

    for (int t = 0; t < nbr_tasks; ++t) {
        plan[hosts[sch[t]]].push_back(tasks[t]);
        assignHost(tasks[t], hosts[sch[t]]);
    }

	Energy* energy = new Energy(nbr_hosts, nbr_tasks);
	energy->assignEnergy(mips,solution.local_makespan,solution.completion,problem.ready_times);
	MaxEnergy = energy->MaxEnergy;
	EnergyI = energy->EnergyI;
	EnergyII = energy->EnergyII;
	Eparam = energy->Eparam;
}
