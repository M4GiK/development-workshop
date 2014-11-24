/*
 * HgsScheduler.cc
 *
 *  Created on: 2010-01-14
 *      Authors: Lukasz and Joanna
 */

#include "HgsScheduler.hh"
#include "HGSSearch.hh" //hgs
#include "InitialSolutions.hh"
#include "Security.hh"
#include "Energy.hh"

#define log_2(x) (log(x)/log(2.0))

void set_setup (int evolution_steps, int pop_size,
                int intermediate_size, double prob_cross, double prob_mutate,
                int start_choice, int select_choice, double select_extra,
                int cross_choice, double cross_extra, int mutate_choice,
                double mutate_extra,  int mutation_size, bool replace_if_better,
                bool replace_generational, int env_length, int env_choice, int energy_case,
                HGSSearch::Setup& setup)


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
    setup.set_energy_case            (energy_case);
}

HgsScheduler::HgsScheduler(double secs, bool h_s, GAParams params, GAParams (&hgs_params)[4]) {
    if (secs <= 0) {
        error("HGSScheduler: wrong max_time_to_spend value!");
        secs = 12;
    }
    for ( int i = 0; i < 4; i++){
    	this->hgs_params[i] = hgs_params[i];
    }
    this->params = params;
    seconds = secs;
    h_or_s = h_s;
}

HgsScheduler::~HgsScheduler() {
	// TODO Auto-generated destructor stub
}

void HgsScheduler::schedule (hostsSet& hosts, tasksSet& tasks) {
    using namespace HGSSearch;
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

 	HGSSearch::Problem problem;
    problem.set_problem (workload, mips, ready, now);
    problem.set_problem (etc, ready, now);

//---------------------------STEADY STATE------------------------

int pop_1 = int( abs( 4*(ceil(log_2(nbr_tasks))-1) ) ); pop_1 = abs(max(pop_1,1)/(11.8));
int ipop_1 = int(ceil(pop_1/6.0));         ipop_1=abs(max(ipop_1, 1)/(11.8));


//--------------------------ELITIST-------------------------------

//int pop_1=int(abs(ceil(log_2(nbr_tasks))-1)); pop_1=abs(max(((pop_1*pop_1)+pop_1),1)/(10.4));
//int ipop_1 = int(ceil((pop_1-2)/2.0));     ipop_1=abs(max(ipop_1, 1)/(10.4));

	assert(params.number_branches >= 1 );
	assert(params.number_branches <= 5 );

    //ustawienie galezi, podany paremetr m�wi�cy ile tych galezi ma byc
    vector <HGSSearch::Setup> setup( params.number_branches );
    //int n_mut = 0;
    //double env_length = 0.5;
    //int env_lenght_1 = nbr_tasks*env_length;//neighbourhood parameter
    //int env_lenght_2 = (nbr_tasks -env_lenght_1)*env_length + env_lenght_1;


//  , ,
//	, , ,
//	, , ,
//	, , ,
//	,  , ,
//	, ,

    //cout << "pop_size[1] == " << params.pop_size << endl;
    // cout << "params.env_length " << params.env_length << endl;

    set_setup (

    		params.evolution_steps, // int evolution_steps
    		params.pop_size, // int pop_size
    		params.intermediate_size, // int intermediate_size

    		params.prob_cross, // double prob_cross
    		params.prob_mutate, // double prob_mutate

    		params.start_choice, // int start_choice
    		params.select_choice, // int select_choice
    		params.select_extra,              // double select_extra

            params.cross_choice,       // int cross_choice
            params.cross_extra,          // double cross_extra
            params.mutate_choice, // int mutate_choice
            params.mutate_extra,          // double mutate_extra

            params.mutation_size,       // int mutation_size

            params.replace_if_better,   // bool replace_if_better
            params.replace_generational,   // bool replace_generational

            params.env_length * nbr_tasks,       // int env_length
            params.env_choice,       // int env_choice
            params.energy,           // int energy_case
            setup[0]);

    for ( int i = 1 ; i < params.number_branches; i++ ){

    	//cout << "pop_size[" << i + 2 << "] == " << hgs_params[i].pop_size << endl;

		set_setup (

				hgs_params[i].evolution_steps, // int evolution_steps
				hgs_params[i].pop_size,  // int pop_size
				hgs_params[i].intermediate_size, // int intermediate_size

				hgs_params[i].prob_cross, // double prob_cross
				hgs_params[i].prob_mutate, // double prob_mutate

				hgs_params[i].start_choice, // int start_choice
				hgs_params[i].select_choice, // int select_choice
				hgs_params[i].select_extra,              // double select_extra

				hgs_params[i].cross_choice,         // int cross_choice
				hgs_params[i].cross_extra,            // double cross_extra
				hgs_params[i].mutate_choice, // int mutate_choice
				hgs_params[i].mutate_extra,        // double mutate_extra
				hgs_params[i].mutation_size,    // int mutation_size
				hgs_params[i].replace_if_better,    // bool replace_if_better
				hgs_params[i].replace_generational,   // bool replace_generational
				hgs_params[i].env_length * nbr_tasks, // int env_length
				hgs_params[i].env_choice, // int env_choice
				hgs_params[i].energy,     // int energy_case
				setup[i]);

    }


 //   int pop = int( abs( 4*(ceil(log_2(nbr_tasks))-1) ) ); pop = max(pop,1);
    for(int i=0; i<(int)setup.size();i++){
    	setup[i].set_optimization_strategy(h_or_s);
    	setup[i].set_max_time_to_spend(seconds);
//    	setup[i].set_population_size(pop_1);
    }

    int evolution_steps = params.number_metaepochs;
    // cout << "Number metaepochs " << evolution_steps << endl;
    /*-------------------------------------------------------------------*/
    /* Now we construct and execute the skeleton solver (provided class) */
    /*-------------------------------------------------------------------*/
    HGSSearch::Solver_HGS solver (problem, setup, evolution_steps);
    solver.run ();

    /*-------------------------------------------------------------------*/
    /* Gather the results (schedule) from solver,and transform+apply them*/
    /*-------------------------------------------------------------------*/
    HGSSearch::Solution solution = solver.best_solution ();
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
