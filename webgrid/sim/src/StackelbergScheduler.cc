/*
 * StackelbergScheduler.cc
 *
 *  Created on: Mar 15, 2010
 *      Author: gumer
 */


#include "StackelbergScheduler.hh"
#include "StackelbergSearch.hh"
#include "InitialSolutions.hh"
#include "Energy.hh"

#define log_2(x) (log(x)/log(2.0))

void set_setup (int evolution_steps, int pop_size,
                int intermediate_size, double prob_cross, double prob_mutate,
                int start_choice, int select_choice, double select_extra,
                int cross_choice, double cross_extra, int mutate_choice,
                double mutate_extra,  int mutation_size, bool replace_if_better,
                bool replace_generational, int env_length, int env_choice,
                bool enable_game, double leader_factor,
                int number_followers, int min_fun_choice,
                bool independent_GA, int energy_case,
                StackelbergSearch::Setup& setup)
{

    setup.set_nb_evolution_steps     (evolution_steps);
    setup.set_population_size        (pop_size);
    setup.set_intermediate_population_size (intermediate_size);
    setup.set_cross_probability      (prob_cross);
    setup.set_mutate_probability     (prob_mutate);
    setup.set_start_choice           (start_choice);
    setup.set_select_choice          (select_choice);
    setup.set_select_extra_parameter (select_extra);
    setup.set_cross_choice           (cross_choice);
    setup.set_cross_extra_parameter  (cross_extra);
    setup.set_mutate_choice          (mutate_choice);
    setup.set_mutate_extra_parameter (mutate_extra);
    setup.set_mutation_size			 (mutation_size);
    setup.set_replace_only_if_better (replace_if_better);
    setup.set_replace_generational   (replace_generational);
    setup.set_enviroment_length		 (env_length);
    setup.set_enviroment_choice		 (env_choice);
    setup.set_enable_game            (enable_game);
    setup.set_leader_factor			 (leader_factor);
    setup.set_number_followers		 (number_followers);
    setup.set_min_fun_choice		 (min_fun_choice);
    setup.set_independent_GA         (independent_GA);
    setup.set_energy_case            (energy_case);
}

StackelbergScheduler::StackelbergScheduler(double secs, bool h_s,
		GAParams params) {
    if (secs <= 0) {
        error("HGSScheduler: wrong max_time_to_spend value!");
        secs = 12;

    }
    this->params = params;

    seconds = secs;
    h_or_s = h_s;
}

StackelbergScheduler::~StackelbergScheduler() {
	// TODO Auto-generated destructor stub
}

int g_number_crashed = 0;


void StackelbergScheduler::schedule (hostsSet& hosts, tasksSet& tasks) {
    using namespace StackelbergSearch;
    /*-------------------------------------------------------------------*/
    /* The purpouse of this code template is to adapt skeleton interfaces*/
    /* with the interface and behaviour of SchedulingPolicy              */
    /*-------------------------------------------------------------------*/
    int nbr_tasks = tasks.size();   if (nbr_tasks == 0) return;
    int nbr_hosts = hosts.size();   if (nbr_hosts == 0) return;



    matrix<double>  etc          (nbr_tasks, nbr_hosts);
    matrix<double>  tfp          (nbr_tasks, nbr_hosts);
    vector<double>  workload     (nbr_tasks);
    vector<double>  mips         (nbr_hosts);
    vector<double>  ready        (nbr_hosts);
    double          now        = getSimulationTime();

    for (int t = 0; t < nbr_tasks; ++t) {
        for (int m = 0; m < nbr_hosts; ++m) {
            etc[t][m] = hosts[m]->getEstimatedExecutionTime(tasks[t]);
    }   }

    /*
	 * Calculate TFP matrix.
	 * Normally rand[0.6, 0.9]
	 */

    double SD_min = 0.6;
	double SD_max = 0.9;
	double TL_min = 0.3;
	double TL_max = 1.0;
	double AB_min = 0.85;
	double AB_max = 1.0;

	double sd_arr[nbr_tasks];
	for ( int i = 0; i < nbr_tasks; i++){
		sd_arr[i] = (SD_min+(SD_max-SD_min)*rand()/double(RAND_MAX));
	}

	/*
	 * Default  rand[0.3, 1.0]
	 */
	vector<double> abort_vec(nbr_hosts);
	double tl_arr[nbr_hosts];
	for ( int i = 0; i < nbr_hosts; i++){
		abort_vec[i] = (AB_min+(AB_max-AB_min)*rand()/double(RAND_MAX));
		tl_arr[i] = (TL_min+(TL_max-TL_min)*rand()/double(RAND_MAX));
	}

	for ( int m = 0; m < nbr_hosts; m++ ){
		for ( int t = 0; t < nbr_tasks; t++ ){

			double sd = sd_arr[t];
			double tl = tl_arr[m];
			double alpha = 3;
			double p;
			if (sd <= tl){
				p=0;
			}
			else{
				p = 1-exp(-alpha*(sd - tl));
			}

			tfp[t][m] = p;

		}
	}

    for (int m = 0; m < nbr_hosts; ++m) {
        ready[m] = hosts[m]->getExpectedReadyTime();
        mips [m] = hosts[m]->getMips();
    }
    /* TODO: To avoid makespan and flowtimes overflow, substract minimum */
    /*       ready time for every ready time ...                         */

    for (int t = 0; t < nbr_tasks; ++t) {
        workload[t] = tasks[t]->getWorkLoad();
    }

 	StackelbergSearch::Problem problem;
    problem.set_problem (workload, mips, ready, now);
    problem.set_problem (etc, tfp, abort_vec, ready, now);



    //ustawienie galezi, podany paremetr m�wi�cy ile tych galezi ma byc
    vector <StackelbergSearch::Setup> setup(1);

    // cout << "params.env_length " << params.env_length << endl;

    set_setup (
    		params.evolution_steps, //2000, nb_evolution_steps
    		params.pop_size,   //60, // population_size
    		params.intermediate_size,//48,   // intermediate_population_size

    		params.prob_cross,//0.80, // cross_probability
    		params.prob_mutate,//0.30, // mutate_probability
    		params.start_choice,//StackelbergSearch::StartRandom,          // start_choice

    		params.select_choice,//StackelbergSearch::SelectLinearRanking,  // select_choice
    		params.select_extra,//0.60,                                    // select_extra_parameter

    		params.cross_choice,//StackelbergSearch::CrossCX,              // cross_choice
    		params.cross_extra,//0.60,                                    // cross_extra_parameter

    		params.mutate_choice,//StackelbergSearch::MutateRebalancing,  // mutate_choice
    		params.mutate_extra,//0.50,                                  // mutate_extra_parameter

    		params.mutation_size,//0,                                     // mutation_size
    		params.replace_if_better,//false,                                 // replace_only_if_better
    		params.replace_generational,//false,                                 // replace_generational

    		params.env_length * nbr_tasks,//1,              // enviroment_length
    		params.env_choice,//AdHocHeuristics::env_basic,            // enviroment_choice

    		params.enable_game,//true,        // enable_game
    		params.leader_factor,//0.0625,        // leader_factor
    		params.number_followers,//15,          // number_followers
    		params.min_fun_choice,//4,           // min_fun_choice
    		params.independent_GA,//false,       // independent_GA
    		params.energy,//0                    // energy case

    		setup[0]);



    Setup::set_fitness_type(3);
    Setup::set_enable_qls(params.security);
    Setup::set_enable_qlu(params.enable_qlu);
    Setup::set_enable_qle(params.enable_qle);
    Setup::set_enable_qlab(params.enable_qlab);
    Setup::set_security_extra_param(params.security_extra);
    Setup::set_abort_extra_param(params.abort_extra);


    /*-------------------------------------------------------------------*/
    /* Now we construct and execute the skeleton solver (provided class) */
    /*-------------------------------------------------------------------*/
    StackelbergSearch::Solver_Seq solver (problem, setup[0]);
    solver.run ();

    /*-------------------------------------------------------------------*/
    /* Gather the results (schedule) from solver,and transform+apply them*/
    /*-------------------------------------------------------------------*/
    StackelbergSearch::Solution solution = solver.best_solution ();
    vector<int> sch = solution.schedule;


    for (int t = 0; t < nbr_tasks; ++t) {
        plan[hosts[sch[t]]].push_back(tasks[t]);
        assignHost(tasks[t], hosts[sch[t]]);
    }

	Energy* energy = new Energy(nbr_hosts, nbr_tasks);
	energy->assignEnergy(mips,solution.local_makespan,solution.completion_l,problem.ready_times);
	MaxEnergy = energy->MaxEnergy;
	EnergyI = energy->EnergyI;
	EnergyII = energy->EnergyII;
	Eparam = energy->Eparam;
}
