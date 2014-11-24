#include "MAScheduler.hh"
#include "Security.hh"
#include "Energy.hh"

#define log_2(x) (log(x)/log(2.0))

MAScheduler::MAScheduler(double secs, int ls, MAParams params) {
    if (secs <= 0) {
        error("MAScheduler: wrong max_time_to_spend value!");                                
        secs = 12;
    }
    this->params = params;
    seconds = secs;
    lsearch = ls;
}

MAScheduler::~MAScheduler() {
}

void MAScheduler::schedule (hostsSet& hosts, tasksSet& tasks) {    
    using namespace Memetic;
    /*-------------------------------------------------------------------*/
    /* The purpouse of this code template is to adapt skeleton interfaces*/
    /* with the interface and behaviour of SchedulingPolicy              */                   
    /*-------------------------------------------------------------------*/
    int nbr_tasks = tasks.size();   if (nbr_tasks == 0) return;
    int nbr_hosts = hosts.size();   if (nbr_hosts == 0) return;
    std::cout<<"MAScheduler working"<<endl;
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
		setup.set_nb_generations				(8*nbr_tasks);
		int pop = int (abs( 2*(ceil(log_2(nbr_tasks))-1) ) ); pop = max(pop,2);
		setup.set_population_size				(pop);
		setup.set_nb_solutions_to_recombine		(3);
		setup.set_nb_recombinations				(max(int (ceil(0.2*pop)),1));	
		setup.set_nb_mutations					(max(int (ceil(0.8*pop)),1));
		setup.set_start_choice					(Memetic::StartAll);
		setup.set_select_choice					(Memetic::SelectRandom);
		setup.set_select_extra_parameter		(0.5);
		setup.set_cross_choice					(Memetic::RecombineOnePoint);
		setup.set_recombine_selection			(Memetic::SelectBinaryTournament);
		setup.set_rec_selection_extra			(0.9);
		setup.set_mutate_choice					(Memetic::MutateRebalancing);
		setup.set_mutate_extra_parameter		(0.6);
		setup.set_mutate_selection				(Memetic::SelectBest);
		setup.set_mut_selection_extra			(0.5);
		setup.set_local_search_choice			(lsearch);
		set_lsearch_parameters					(nbr_tasks, setup);
		setup.set_nb_ls_not_improving_iterations(max(int (ceil(0.6*setup.nb_local_search_iterations())),1));
		setup.set_add_only_if_better			(false);
		//---------------------------------------------------------------------
	} else {
		//--------------------------- LocalTabuHop Configuration -----------------------------
		setup.set_nb_generations				(nbr_tasks);
		setup.set_population_size				(4);
		setup.set_nb_solutions_to_recombine		(2);
		setup.set_nb_recombinations				(2);	
		setup.set_nb_mutations					(2);
		setup.set_start_choice					(Memetic::StartAll);
		setup.set_select_choice					(Memetic::SelectLinearRanking);
		setup.set_select_extra_parameter		(0.5);
		setup.set_cross_choice					(Memetic::RecombineOnePoint);
		setup.set_recombine_selection			(Memetic::SelectBinaryTournament);
		setup.set_rec_selection_extra			(0.9);
		setup.set_mutate_choice					(Memetic::MutateRebalancing);
		setup.set_mutate_extra_parameter		(0.6);
		setup.set_mutate_selection				(Memetic::SelectBest);
		setup.set_mut_selection_extra			(0.5);
		setup.set_local_search_choice			(lsearch);
		set_lsearch_parameters					(nbr_tasks, setup);
		setup.set_nb_ls_not_improving_iterations(plus_infinity);
		setup.set_add_only_if_better			(false);
		//---------------------------------------------------------------------
	}*/

        if (lsearch != Memetic::LocalTabuHop) {
		//--------------------------- Default Configuration -----------------------------
		setup.set_nb_generations				(params._nb_generations);
		//int pop = int (abs( 2*(ceil(log_2(nbr_tasks))-1) ) ); pop = max(pop,2);
		setup.set_population_size				(params._population_size);
		setup.set_nb_solutions_to_recombine		(params._nb_solutions_to_recombine);
		setup.set_nb_recombinations				(params._nb_recombinations);
		setup.set_nb_mutations					(params._nb_mutations);
		setup.set_start_choice					(params._start_choice);
		setup.set_select_choice					(params._select_choice);
		setup.set_select_extra_parameter		(params._select_extra_parameter);
		setup.set_cross_choice					(params._cross_choice);
		setup.set_recombine_selection			(params._recombine_selection);
		setup.set_rec_selection_extra			(params._rec_selection_extra);
		setup.set_mutate_choice					(params._mutate_choice);
		setup.set_mutate_extra_parameter		(params._mutate_extra_parameter);
		setup.set_mutate_selection				(params._mutate_selection);
		setup.set_mut_selection_extra			(params._mut_selection_extra);
		setup.set_local_search_choice			(lsearch);
		set_lsearch_parameters					(nbr_tasks, setup);
		setup.set_nb_ls_not_improving_iterations(params._nb_ls_not_improving_iterations);
		setup.set_add_only_if_better			(params._add_only_if_better);
		//---------------------------------------------------------------------
	} else {
		//--------------------------- LocalTabuHop Configuration -----------------------------
		setup.set_nb_generations				(params._nb_generations);
		setup.set_population_size				(params._population_size);
		setup.set_nb_solutions_to_recombine		(params._nb_solutions_to_recombine);
		setup.set_nb_recombinations				(params._nb_recombinations);
		setup.set_nb_mutations					(params._nb_mutations);
		setup.set_start_choice					(params._start_choice);
		setup.set_select_choice					(params._select_choice);
		setup.set_select_extra_parameter		(params._select_extra_parameter);
		setup.set_cross_choice					(params._cross_choice);
		setup.set_recombine_selection			(params._recombine_selection);
		setup.set_rec_selection_extra			(params._rec_selection_extra);
		setup.set_mutate_choice					(params._mutate_choice);
		setup.set_mutate_extra_parameter		(params._mutate_extra_parameter);
		setup.set_mutate_selection				(params._mutate_selection);
		setup.set_mut_selection_extra			(params._mut_selection_extra);
		setup.set_local_search_choice			(lsearch);
		set_lsearch_parameters					(nbr_tasks, setup);
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

void MAScheduler::set_lsearch_parameters (int nbr_tasks, Memetic::Setup& setup)	{
	switch (lsearch) {
		case Memetic::LocalMove:
			setup.set_nb_local_search_iterations	(max(ceil(log_2(nbr_tasks)),10.0));
			break;
		case Memetic::SteepestLocalMove:
			setup.set_nb_local_search_iterations	(1);
			break;
		case Memetic::LocalSwap:
			setup.set_nb_local_search_iterations	(max(ceil(log_2(nbr_tasks)),10.0));
			break;
		case Memetic::SteepestLocalSwap:
			setup.set_nb_local_search_iterations	(1);
			break;
		case Memetic::LocalRebalance:
			setup.set_nb_local_search_iterations	(max(ceil(log_2(nbr_tasks)),10.0));
			setup.set_lsearch_extra_parameter		(0.75);
			break;
		case Memetic::DeepLocalRebalance:
			setup.set_nb_local_search_iterations	(1);
			setup.set_lsearch_extra_parameter		(0.5);
			break;
		case Memetic::LocalMCTMove:
			setup.set_nb_local_search_iterations	(max(ceil(log_2(nbr_tasks)),10.0));
			break;
		case Memetic::LocalMCTSwap:
			setup.set_nb_local_search_iterations	(max(ceil(log_2(nbr_tasks)),10.0));
			break;
		case Memetic::LocalMFTMove:
			setup.set_nb_local_search_iterations	(max(ceil(log_2(nbr_tasks)/2),2.0));
			break;
		case Memetic::LocalMFTSwap:
			setup.set_nb_local_search_iterations	(1);				
			break;
		case Memetic::LocalFlowtimeRebalance:
			setup.set_nb_local_search_iterations	(max(ceil(log_2(nbr_tasks)/2),2.0));
			setup.set_lsearch_extra_parameter		(0.5);
			break;
		case Memetic::EmptiestRessourceRebalance:
			setup.set_nb_local_search_iterations	(max(ceil(log_2(nbr_tasks)),10.0));
			setup.set_lsearch_extra_parameter		(0.5);
			break;
		case Memetic::EmptiestRessourceFlRebalance:
			setup.set_nb_local_search_iterations	(max(ceil(log_2(nbr_tasks)/2),2.0));
			setup.set_lsearch_extra_parameter		(0.25);
			break;
		case Memetic::LocalShortHop:
			setup.set_nb_local_search_iterations	(max(ceil(log_2(nbr_tasks)),10.0));
			setup.set_lsearch_extra_parameter		(500);
			break;
		case Memetic::VNS:
			setup.set_nb_local_search_iterations	(max(ceil(log_2(nbr_tasks)),10.0));
			setup.set_lsearch_extra_parameter		(3);
			break;
		case Memetic::LocalTabuHop:
			setup.set_nb_local_search_iterations	(1);
			setup.set_lsearch_extra_parameter		(max(abs(5*ceil(log_2(nbr_tasks))), 1.0));
			break;		
	}
}

