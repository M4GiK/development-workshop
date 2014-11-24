/*
 * GAParams.hh
 *
 *  Created on: Apr 15, 2010
 *      Author: gumer
 */

#ifndef GAPARAMS_HH_
#define GAPARAMS_HH_

#include "mallba.hh"

//#include "InitialSolutions.hh"

enum SelectChoice {  SelectRandom, SelectBest, SelectLinearRanking,
					 SelectExpRanking, SelectBinaryTournament,
					 SelectNTournament
				  };
enum CrossChoice  {  CrossOnePoint, CrossTwoPoints, CrossUniform,
					 CrossFitnessBased, CrossPMX, CrossOX, CrossCX
				  };
enum MutateChoice {  MutateMove, MutateSwap, MutateBoth,
					 MutateRebalancing
				  };

enum StartMethod  { StartLJFRSJFR, StartMCT, StartRandom, StartAll,
					StartMinMin, StartRelativeCost, StartKPB, StartAll2
				  };

enum FitnessType  { FitnessMakespan, FitnessFlowtime, FitnessBoth
				  };

namespace AdHocHeuristics{

	enum heuristic_type { MinMin_h, MaxMin_h, Sufferage_h, RelativeCost_h,
						  LJFRSJFR_h, MCT_h, MET_h, SA_h, KPB_h, OLB_h,
						  Random_h
						};

	enum enviroment_type
	{
		  env_basic, env_minimalizing
	};

}

class GAParams {
public:
	int evolution_steps;
	int pop_size;
	int intermediate_size;
	double prob_cross;
	double prob_mutate;
	int start_choice;
	int select_choice;
	double select_extra;
	int cross_choice;
	double cross_extra;
	int mutate_choice;
	double mutate_extra;
	int mutation_size;
	bool replace_if_better;
	bool replace_generational;
        bool struggle_replace;
        int struggle_extra_parameter;
	double env_length;
	int env_choice;


	bool enable_game;
	double leader_factor;
	int number_followers;
	int min_fun_choice;
	bool independent_GA;

	bool security;
	double security_param;
	double security_extra;

	bool calc_penalty;

	bool enable_qle;
	bool enable_qlu;
	bool enable_qlab;

	double abort_extra;

	int number_branches;

	int number_metaepochs;


	/*
	 * Params needed for HGS (so we know what not to override with root branch
	 */
	bool is_set_evolution_steps;
	bool is_set_pop_size;
	bool is_set_intermediate_size;
	bool is_set_prob_cross;
	bool is_set_prob_mutate;
	bool is_set_start_choice;
	bool is_set_select_choice;
	bool is_set_select_extra;
	bool is_set_cross_choice;
	bool is_set_cross_extra;

	bool is_set_mutate_choice;
	bool is_set_mutate_extra;
	bool is_set_mutation_size;
	bool is_set_replace_if_better;
	bool is_set_replace_generational;
	bool is_set_env_length;
	bool is_set_env_choice;

	int energy;

public:
	GAParams():
		evolution_steps(2000), // nb_evolution_steps
		pop_size(60),   // population_size
		intermediate_size(48),   // intermediate_population_size

		prob_cross(0.80), // cross_probability
		prob_mutate(0.30), // mutate_probability

		start_choice(StartRandom),         // start_choice
		select_choice(SelectLinearRanking),  // select_choice
		select_extra(0.60),                                    // select_extra_parameter

		cross_choice(CrossCX),              // cross_choice
		cross_extra (0.60),                                    // cross_extra_parameter

		mutate_choice(MutateRebalancing),  // mutate_choice
		mutate_extra  (0.50),                                  // mutate_extra_parameter
		mutation_size  (0),                                     // mutation_size
		replace_if_better(false),                                 // replace_only_if_better
		replace_generational(false),                                 // replace_generational
                struggle_replace(false),
                struggle_extra_parameter(2),

		env_length(1),                                     // enviroment_length
		env_choice(AdHocHeuristics::env_basic),            // enviroment_choice

		enable_game(true),        // enable_game
		leader_factor(0.0625),        // leader_factor
		number_followers(15),          // number_followers
		min_fun_choice(0),           // min_fun_choice
		independent_GA(false),       // independent_GA

		security(true),
		security_param(3),
		security_extra(20),
		calc_penalty(false),

		enable_qle(true),
		enable_qlu(true),
		enable_qlab(true),

		abort_extra(50),

		number_branches(1),

		number_metaepochs(10),





		/*
		 * For HGS parsing
		 */
	    is_set_evolution_steps(false),
		is_set_pop_size(false),
		is_set_intermediate_size(false),
		is_set_prob_cross(false),
		is_set_prob_mutate(false),
		is_set_start_choice(false),
		is_set_select_choice(false),
		is_set_select_extra(false),
		is_set_cross_choice(false),
		is_set_cross_extra(false),
		is_set_mutate_choice(false),
		is_set_mutate_extra(false),
		is_set_mutation_size(false),
		is_set_replace_if_better(false),
		is_set_replace_generational(false),
		is_set_env_length(false),
		is_set_env_choice(false),

		energy(0)
	{}
};

class TabuParams {
public:
	int nb_iterations;
	int start_choice;    // <START>
	bool use_delta_function;  // not used

	int tabu_size;
	int max_tabu_status;
	int min_tabu_status; // not used

	int max_repetitions;
	int nb_intensifications;
	int nb_diversifications;
	int elite_size;
	int aspiration_value;

	float max_nb_swaps;
	float max_nb_transfers;

	float max_load_interval;
	float percent_min_load;

	bool security;
	double security_param;
	int energy;


public:
	TabuParams():

		 nb_iterations(2000),
		 start_choice(AdHocHeuristics::MCT_h),    // <ADHEUR>
		 use_delta_function(false),  // not used

		 tabu_size(918133),
		 max_tabu_status(32),
		 min_tabu_status(32), // not used

		 max_repetitions(1),
		 nb_intensifications(9),
		 nb_diversifications(9),
		 elite_size(10),
		 aspiration_value(1),

		 max_nb_swaps(plus_infinity),
		 max_nb_transfers(plus_infinity),

		 max_load_interval(1.00),
		 percent_min_load(1.00),

		 security(false),
		 security_param(3),
		 energy(0)
	{}
};

namespace CMAParamsEnums
{
    enum StartMethod  { StartLJFRSJFR, StartMCT, StartRandom, StartAll,
                                                    StartMinMin, StartRelativeCost, StartKPB, StartAll2 };
    /*StartAll = 1xStartLJFRSJFR & 1xStartMCT & (n-2)xStartRandom*/
    /*StartAll2 = 1xStartLJFRSJFR & 1xStartMCT & 1xStartMinMin & 1xStartRelativeCost & 1xStartKPB & (n-5)xStartRandom*/

    enum SelectChoice { SelectRandom, SelectBest, SelectLinearRanking,
                        SelectExpRanking, SelectBinaryTournament,
                        SelectNTournament };

    enum RecombineChoice  { RecombineOnePoint, RecombineUniform, RecombineFitnessBased };

    enum MutateChoice { MutateMove, MutateSwap, MutateBoth, MutateRebalancing };

    enum LocalSearchChoice { LocalMove, SteepestLocalMove, LocalSwap, SteepestLocalSwap,
                             LocalRebalance, DeepLocalRebalance, LocalMCTMove, LocalMCTSwap,
                             LocalMFTMove, LocalMFTSwap, LocalFlowtimeRebalance, EmptiestRessourceRebalance,
                             EmptiestRessourceFlRebalance, LocalShortHop, VNS, LocalTabuHop };

    enum NeighborhoodShape { Panmictic, L5, L9, C9, C13};

    enum AsynchronousUpdate { FixedLineSweep, FixedRandomSweep, NewRandomSweep };
}

class CMAParams {
public:
	int _nb_independent_runs;
        int _nb_generations;
        float _max_time_to_spend;
        int _population_height;
        int _population_width;
        int _max_repetitions_to_diversify;
        int _nb_solutions_to_recombine;
        int _nb_recombinations;
        int _nb_mutations;
        int _start_choice;
        bool _add_only_if_better;
        int _neighborhood_pattern;
        int _recombination_order;
        int _mutation_order;
        int _recombine_choice;
        int	_recombine_selection;
        float _rec_selection_extra;
        int _mutate_choice;
        float _mutate_extra_parameter;
        int _local_search_choice;
        int _nb_local_search_iterations;
        double _nb_ls_not_improving_iterations;
        float _lsearch_extra_parameter;
        bool security;
        double security_param;
        int energy;


public:
	CMAParams():

          _nb_independent_runs(1),
	  _nb_generations(40),
	  _max_time_to_spend(plus_infinity),
	  _population_height(6),
	  _population_width(6),
	  _max_repetitions_to_diversify(13),
	  _nb_solutions_to_recombine(2),
	  _nb_recombinations(13),
	  _nb_mutations(13),
	  _start_choice(StartAll),
	  _add_only_if_better(false),
	  _neighborhood_pattern(1),
	  _recombination_order(CMAParamsEnums::FixedLineSweep),
	  _mutation_order(CMAParamsEnums::FixedLineSweep),
	  _recombine_choice(CMAParamsEnums::RecombineOnePoint),
	  _recombine_selection(CMAParamsEnums::SelectRandom),
	  _rec_selection_extra(0),
	  _mutate_choice(CMAParamsEnums::MutateMove),
	  _mutate_extra_parameter(0),
	  _local_search_choice(0),
	  _nb_local_search_iterations(0),
	  _nb_ls_not_improving_iterations(plus_infinity),
	  _lsearch_extra_parameter(0.0),
	  security(false),
	  security_param(3),
	  energy(0)

	{}
};

namespace MAParamsEnums 
{
    enum StartMethod  { StartLJFRSJFR, StartMCT, StartRandom, StartAll,
						StartMinMin, StartRelativeCost, StartKPB, StartAll2};
	/*StartAll = 1xStartLJFRSJFR & 1xStartMCT & (n-2)xStartRandom*/
	/*StartAll2 = 1xStartLJFRSJFR & 1xStartMCT & 1xStartMinMin & 1xStartRelativeCost & 1xStartKPB & (n-5)xStartRandom*/

	enum SelectChoice { SelectRandom, SelectBest, SelectLinearRanking,
			    SelectExpRanking, SelectBinaryTournament,
			    SelectNTournament };

	enum RecombineChoice  { RecombineOnePoint, RecombineUniform, RecombineFitnessBased };

	enum MutateChoice { MutateMove, MutateSwap, MutateBoth, MutateRebalancing };    

	enum LocalSearchChoice { LocalMove, SteepestLocalMove, LocalSwap, SteepestLocalSwap, 
				 LocalRebalance, DeepLocalRebalance, LocalMCTMove, LocalMCTSwap,
				 LocalMFTMove, LocalMFTSwap, LocalFlowtimeRebalance, EmptiestRessourceRebalance,
				 EmptiestRessourceFlRebalance, LocalShortHop, VNS, LocalTabuHop};

        enum NeighborhoodShape { Panmictic, L5, L9, C9, C13};

	enum AsynchronousUpdate { FixedLineSweep, FixedRandomSweep, NewRandomSweep };
}


typedef struct {
		int lsearch_id;
		int nb_lsearch_iterations;
		double nb_ls_not_improving_it;
		float lsearch_extra_parameter;
	} LS_config;

class MAParams {
public:
	int _nb_independent_runs;
        int _nb_generations;
        float _max_time_to_spend;
        int _population_size;
        int _population_height;
        int _population_width;
        int _max_repetitions_to_diversify;
        int _nb_solutions_to_recombine;
        int _nb_recombinations;
        int _nb_mutations;
        int _start_choice;
        bool _add_only_if_better;
        int _neighborhood_pattern;
        int _recombination_order;
        int _mutation_order;
        int _recombine_choice;
        int _select_choice;
        float _select_extra_parameter;
        int   _cross_choice;
        int	_recombine_selection;
        float _rec_selection_extra;
        int _mutate_choice;
        float _mutate_extra_parameter;
        int _mutate_selection;
        float _mut_selection_extra;
        int _local_search_choice;
        int _nb_local_search_iterations;
        double _nb_ls_not_improving_iterations;
        float _lsearch_extra_parameter;
        std::vector<LS_config> _slave_local_search;
        bool security;
        double security_param;
        int energy;


public:
	MAParams():

          _nb_independent_runs(10),
	  _nb_generations(40),
	  _max_time_to_spend(plus_infinity),
	  _population_size(26),
	  _max_repetitions_to_diversify(13),
	  _nb_solutions_to_recombine(2),
	  _nb_recombinations(13),
	  _nb_mutations(13),
	  _start_choice(StartAll),
	  _add_only_if_better(false),
	  _neighborhood_pattern(1),
	  _recombination_order(MAParamsEnums::FixedLineSweep),
	  _mutation_order(MAParamsEnums::FixedLineSweep),
	  _recombine_choice(MAParamsEnums::RecombineOnePoint),
	  _select_choice(SelectRandom),
	  _select_extra_parameter(0),
	  _cross_choice(MAParamsEnums::RecombineOnePoint),
	  _recombine_selection(SelectRandom),
	  _rec_selection_extra(0),
	  _mutate_choice(MAParamsEnums::MutateMove),
	  _mutate_extra_parameter(0),
	  _mutate_selection(SelectRandom),
	  _mut_selection_extra(0),
	  _local_search_choice(0),
	  _nb_local_search_iterations(0),
	  _nb_ls_not_improving_iterations(plus_infinity),
	  _lsearch_extra_parameter(0),
	  _slave_local_search(std::vector<LS_config>(0)),
	  security(false),
	  security_param(3),
	  energy(0)

	{}
};


#endif /* GAPARAMS_HH_ */
