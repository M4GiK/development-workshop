#include "CMAScheduler.hh"
#include "Security.hh"
#include "Energy.hh"

#define log_2(x) (log(x)/log(2.0))

CMAScheduler::CMAScheduler(double secs, int ls,
		CMAParams params) {
    if (secs <= 0) {
        error("CMAScheduler: wrong max_time_to_spend value!");                                
        secs = 12;
    }
    this->params = params;

    seconds = secs;
    lsearch = ls;
}

CMAScheduler::~CMAScheduler() {
}

void CMAScheduler::schedule (hostsSet& hosts, tasksSet& tasks) {    
    using namespace Memetic;
    /*-------------------------------------------------------------------*/
    /* The purpouse of this code template is to adapt skeleton interfaces*/
    /* with the interface and behaviour of SchedulingPolicy              */                   
    /*-------------------------------------------------------------------*/
    int nbr_tasks = tasks.size();   if (nbr_tasks == 0) return;
    int nbr_hosts = hosts.size();   if (nbr_hosts == 0) return;
    cout << "CMA Starting" << endl;;
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
    
 	Memetic::Problem problem;
    problem.set_problem (workload, mips, ready, now);
    problem.set_problem (etc, ready, now);
        
    /*-------------------------------------------------------------------*/
    /* Setup tunning is specific for each skeleton type (parameters and  */
    /* their tunning may be different)...                                */
    /*-------------------------------------------------------------------*/    
    
	Memetic::Setup setup;
	setup.set_nb_independent_runs			(1);
	setup.set_max_time_to_spend				(seconds);
	setup.set_energy_case                   (params.energy);

	/*if (lsearch != Memetic::LocalTabuHop) {
		//--------------------------- Default Configuration -----------------------------
			setup.set_nb_generations				(9*nbr_tasks);
			int size = int (ceil(sqrt(log_2(nbr_tasks) ) ) ) + 2;
			setup.set_population_height				(size);
			setup.set_population_width				(size);			
			setup.set_nb_solutions_to_recombine		(3);
			setup.set_nb_recombinations				(setup.population_size());	
			setup.set_nb_mutations					(setup.population_size()/2);
			setup.set_start_choice					(Memetic::StartAll);
			setup.set_neighborhood_pattern			(Memetic::L5);
			setup.set_recombination_order			(Memetic::FixedLineSweep);
			setup.set_mutation_order				(Memetic::NewRandomSweep);
			setup.set_recombine_choice				(Memetic::RecombineOnePoint);
			setup.set_recombine_selection			(Memetic::SelectNTournament);
			setup.set_rec_selection_extra			(3);
			setup.set_mutate_choice					(Memetic::MutateRebalancing);
			setup.set_mutate_extra_parameter		(0.6);
			setup.set_local_search_choice			(lsearch);
			set_lsearch_parameters (nbr_tasks, setup);
			setup.set_nb_ls_not_improving_iterations(plus_infinity);
			setup.set_add_only_if_better			(true);
		//---------------------------------------------------------------------
	} else {
		//--------------------------- LocalTabuHop Configuration -----------------------------
		setup.set_nb_generations				(2*nbr_tasks);
		setup.set_population_height				(3);
		setup.set_population_width				(3);			
		setup.set_nb_solutions_to_recombine		(3);
		setup.set_nb_recombinations				(setup.population_size());	
		setup.set_nb_mutations					(setup.population_size()/2);
		setup.set_start_choice					(Memetic::StartAll);
		setup.set_neighborhood_pattern			(Memetic::L5);
		setup.set_recombination_order			(Memetic::FixedLineSweep);
		setup.set_mutation_order				(Memetic::NewRandomSweep);
		setup.set_recombine_choice				(Memetic::RecombineOnePoint);
		setup.set_recombine_selection			(Memetic::SelectNTournament);
		setup.set_rec_selection_extra			(4);
		setup.set_mutate_choice					(Memetic::MutateRebalancing);
		setup.set_mutate_extra_parameter		(0.6);
		setup.set_local_search_choice			(lsearch);
		set_lsearch_parameters (nbr_tasks, setup);
		setup.set_nb_ls_not_improving_iterations(plus_infinity);
		setup.set_add_only_if_better			(true);
		//---------------------------------------------------------------------
	}*/
        if (lsearch != Memetic::LocalTabuHop) {
		//--------------------------- Default Configuration -----------------------------
			setup.set_nb_generations				(params._nb_generations);
			setup.set_population_height				(params._population_height);
			setup.set_population_width				(params._population_width);
			setup.set_nb_solutions_to_recombine		(params._nb_solutions_to_recombine);
			setup.set_nb_recombinations				(params._nb_recombinations);
			setup.set_nb_mutations					(params._nb_mutations);
			setup.set_start_choice					(params._start_choice);
			setup.set_neighborhood_pattern			(params._neighborhood_pattern);
			setup.set_recombination_order			(params._recombination_order);
			setup.set_mutation_order				(params._mutation_order);
			setup.set_recombine_choice				(params._recombine_choice);
			setup.set_recombine_selection			(params._recombine_selection);
			setup.set_rec_selection_extra			(params._rec_selection_extra);
			setup.set_mutate_choice					(params._mutate_choice);
			setup.set_mutate_extra_parameter		(params._mutate_extra_parameter);
			setup.set_local_search_choice			(lsearch);
			set_lsearch_parameters (nbr_tasks, setup);
			setup.set_nb_ls_not_improving_iterations(params._nb_ls_not_improving_iterations);
			setup.set_add_only_if_better			(params._add_only_if_better);
		//---------------------------------------------------------------------
	} else {
		//--------------------------- LocalTabuHop Configuration -----------------------------
		setup.set_nb_generations				(params._nb_generations);
		setup.set_population_height				(params._population_height);
		setup.set_population_width				(params._population_width);
		setup.set_nb_solutions_to_recombine		(params._nb_solutions_to_recombine);
		setup.set_nb_recombinations				(params._nb_recombinations);
		setup.set_nb_mutations					(params._nb_mutations);
		setup.set_start_choice					(params._start_choice);
		setup.set_neighborhood_pattern			(params._neighborhood_pattern);
		setup.set_recombination_order			(params._recombination_order);
		setup.set_mutation_order				(params._mutation_order);
		setup.set_recombine_choice				(params._recombine_choice);
		setup.set_recombine_selection			(params._recombine_selection);
		setup.set_rec_selection_extra			(params._rec_selection_extra);
		setup.set_mutate_choice					(params._mutate_choice);
		setup.set_mutate_extra_parameter		(params._mutate_extra_parameter);
		setup.set_local_search_choice			(lsearch);
		set_lsearch_parameters (nbr_tasks, setup);
		setup.set_nb_ls_not_improving_iterations(params._nb_ls_not_improving_iterations);
		setup.set_add_only_if_better			(params._add_only_if_better);
		//---------------------------------------------------------------------
	}
    
    /*-------------------------------------------------------------------*/
    /* Now we construct and execute the skeleton solver (provided class) */
    /*-------------------------------------------------------------------*/
    Memetic::Solver_Seq solver (problem, setup);
    solver.run ();
        
    /*-------------------------------------------------------------------*/
    /* Gather the results (schedule) from solver,and transform+apply them*/
    /*-------------------------------------------------------------------*/
    Memetic::Solution solution = solver.best_Mksp_solution ();
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

void CMAScheduler::set_lsearch_parameters (int nbr_tasks, Memetic::Setup& setup)	{
	switch (lsearch) {
		case Memetic::LocalMove:
			setup.set_nb_local_search_iterations	(5);
			break;
		case Memetic::SteepestLocalMove:
			setup.set_nb_local_search_iterations	(1);
			break;
		case Memetic::LocalSwap:
			setup.set_nb_local_search_iterations	(5);
			break;
		case Memetic::SteepestLocalSwap:
			setup.set_nb_local_search_iterations	(1);
			break;
		case Memetic::LocalRebalance:
			setup.set_nb_local_search_iterations	(5);
			setup.set_lsearch_extra_parameter		(0.75);
			break;
		case Memetic::DeepLocalRebalance:
			setup.set_nb_local_search_iterations	(1);
			setup.set_lsearch_extra_parameter		(0.5);
			break;
		case Memetic::LocalMCTMove:
			setup.set_nb_local_search_iterations	(5);
			break;
		case Memetic::LocalMCTSwap:
			setup.set_nb_local_search_iterations	(5);
			break;
		case Memetic::LocalMFTMove:
			setup.set_nb_local_search_iterations	(5);
			break;
		case Memetic::LocalMFTSwap:
			setup.set_nb_local_search_iterations	(1);				
			break;
		case Memetic::LocalFlowtimeRebalance:
			setup.set_nb_local_search_iterations	(5);
			setup.set_lsearch_extra_parameter		(0.5);
			break;
		case Memetic::EmptiestRessourceRebalance:
			setup.set_nb_local_search_iterations	(5);
			setup.set_lsearch_extra_parameter		(0.5);
			break;
		case Memetic::EmptiestRessourceFlRebalance:
			setup.set_nb_local_search_iterations	(5);
			setup.set_lsearch_extra_parameter		(0.25);
			break;
		case Memetic::LocalShortHop:
			setup.set_nb_local_search_iterations	(5);
			setup.set_lsearch_extra_parameter		(500);
			break;
		case Memetic::VNS:
			setup.set_nb_local_search_iterations	(5);
			setup.set_lsearch_extra_parameter		(3);
			break;
		case Memetic::LocalTabuHop:
			setup.set_nb_local_search_iterations	(1);
			setup.set_lsearch_extra_parameter		(10);
			break;		
	}
}

