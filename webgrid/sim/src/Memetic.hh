// FILE: Memetic.hh
#ifndef INC_Memetic
#define INC_Memetic
#include <iostream>
#include <assert.h>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>

#include "mallba.hh"
#include <mpi.h>
#include "iopacket2_nl.hh"
#include "no_leda.hh"
#include "TabuSearch.hh"

#include "Distribution.hh"
#include "ConstantDistribution.hh"
#include "NormalDistribution.hh"
#include "UniformDistribution.hh"
#include "ExponentialDistribution.hh"
#include "ZipfDistribution.hh"
#include "TriangleDistribution.hh"
#include "InitialSolutions.hh"

using namespace AdHocHeuristics;

//#define TRACE_FIT		/*to keep trace of best fitness values (a trace ofstream must be set in Setup)*/
//#define TRACE_MKSP	/*to keep trace of best makespan values (a trace ofstream must be set in Setup)*/
#define MULTIOBJECTIVE	/*to perform a multiobjective serarch considering makespan and mean flowtime simultaneously*/

//#define NDEBUG
//#define LS_TEST
//#define MA_VERBOSE
//#define RECOMBINATION_VERBOSE
//#define MUTATION_VERBOSE
//#define RESTART_VERBOSE

skeleton Memetic {
	
	requires class Problem;
	requires class Solution;
	requires class Population;
	requires class Local_Search_Engine;
	requires class Solution_Changes;
	
	provides class Setup;
	provides class Setup_IR;
	provides class Solver;
	provides class Solver_Seq;
	provides class Solver_Lan_IR;
	provides class Solver_Lan_IR_DS;
	provides class Solver_Lan_MS;


	//	REQUIRED CLASSES AND METHODS ---------------------------------------

	requires class Problem {
		public:
			Problem ();
			~Problem ();
			
			friend ostream& operator<< (ostream& os, const Problem& pbm);
			friend istream& operator>> (istream& is, Problem& pbm);
			friend opacket2& operator<< (opacket2& op, const Problem& pbm);
			friend ipacket2& operator>> (ipacket2& ip, Problem& pbm);
			
			Direction direction () const;
			
			void set_problem (const vector<double>& task_info,
				const vector<double>& machine_info,
			    const vector<double>& ready_time_info,
			    const double current_time);
			
			void set_problem (const matrix<double>& etc, const vector<double>& ready_time_info, const double current_time);
			
			int             number_tasks;
			int             number_machines;
			vector<double>  workload;
			vector<double>  mips;
			double          now;
			vector<double>  ready_times;
			matrix<double>  etc;

		protected:
			void compute_etc ();
			void fix_ready_times_to_now ();
			void fill_workload_mips ();
	};
	
	requires class Solution {
		public:	
			Solution (const Problem& pbm);
			Solution (const Solution& sol);
			~Solution ();
			
			Solution&        operator= (const Solution& sol);
			friend bool      operator== (const Solution& sol1, const Solution& sol2);
			friend bool      operator!= (const Solution& sol1, const Solution& sol2);
			friend ostream&  operator<< (ostream& os, const Solution& sol);
			friend istream&  operator>> (istream& is, Solution& sol);
			friend opacket2& operator<< (opacket2& op, const Solution& sol);
			friend ipacket2& operator>> (ipacket2& ip, Solution& sol);
			
			double fitness () const;
			bool   is_better_than  (const Solution& sol) const;
			void   mutate (const Solver& solver);
			void   update_fitness ();
			bool   feasible() const;
			void   update_feasible ();
			
			friend Solution recombine (vector<Solution>& sols);
			friend Solution recombine_one_point (const Solver& solver, vector<Solution>& sols);	  
			friend Solution recombine_uniform (const Solver& solver, vector<Solution>& sols);
			friend Solution recombine_fitness_based (const Solver& solver, vector<Solution>& sols);	  
			
			friend void mutate_move (const Solver& solver, Solution& sol);
			friend void mutate_swap (const Solver& solver, Solution& sol);
			friend void mutate_both (const Solver& solver, Solution& sol);
			friend void mutate_rebalancing (const Solver& solver, Solution& sol);     
			
			friend void local_move (const Solver& solver, Solution& sol);
			friend void steepest_local_move (const Solver& solver, Solution& sol);
			friend void local_swap (const Solver& solver, Solution& sol);
			friend void steepest_local_swap (const Solver& solver, Solution& sol);
			friend void local_rebalance (const Solver& solver, Solution& sol);
			friend void deep_local_rebalance (const Solver& solver, Solution& sol);
			friend void local_mct_move (const Solver& solver, Solution& sol);
			friend void local_mct_swap (const Solver& solver, Solution& sol);
			friend void local_mft_move (const Solver& solver, Solution& sol);
			friend void local_mft_swap (const Solver& solver, Solution& sol);
			friend void local_f_rebalance (const Solver& solver, Solution& sol);
			friend void local_empt_ressource_rebalance (const Solver& solver, Solution& sol);
			friend void local_empt_ressource_fl_rebalance (const Solver& solver, Solution& sol);
			friend void local_short_hop (const Solver& solver, Solution& sol);
			friend void variable_nbh_search (const Solver& solver, Solution& sol);
			friend void local_tabu_hop (const Solver& solver, Solution& sol);
			friend bool improvement (const Solution& s1, const Solution& s2);
			friend bool LS_improvement (Solution& s1, const Solution& s2);
			
			friend class Solution_Changes;
			
			void heuristic (heuristic_type choice);   		
			void perturbate_randomly  ();
			
			vector<int>     schedule;
			vector<double>  completion;        
			double			local_makespan;    /* Makespan of used machines  */        
			double			local_avg_utilization;
			double			flowtime;
			double			energy;
			
			bool	host_participates (int host);
			double	get_flowtime (int machine);
			void	compute_makespan ();
			void	compute_flowtime ();
			void	perform_move (int t, int m);
			void	perform_swap (int t1, int t2);
		
		protected:
			void compute_most_loaded_machines       (matrix<int>& sched, vector<int>& machines);
			void transform_schedule_representation  (matrix<int>& sched);
			void transform_schedule_representation  (vector<int>& perm, vector<int>& limits);
			void apply_schedule_representation      (const matrix<int>& sched);
			void apply_schedule_representation      (const vector<int>& perm, const vector<int>& limits);
			int  select_task_from_another_machine   (int machine, const matrix<int>& sched);
			int  select_best_task_from_another_machine (int machine, const matrix<int>& sched);
			int  select_best_task_for_this_machine  (int machine, const vector<int>& tasks);
			void fill_machines_permutation (vector<int>& machine_perm1, vector<int>& machine_perm2);
			
			const Problem&  _problem;
			double          _fitness;
			bool			_feasible;
	}; 
	
	requires class Population {	
		public:	
			Population (const Solver& solver);	  
			~Population ();
			
			void 	 initialize ();
			void 	 add (const Solution& sol);
			void	 add (const vector<Solution>& sols);
			void 	 select ();
			void 	 restart ();
			int  	 size () const;

			Solution best () const;
			Solution best_Mksp_solution() const;	  

			bool   meets_conv_criteria () const;
			void   update ();
			void   sort ();
			
			vector<Solution> select_to_merge (bool at_random=false);
			Solution select_to_mutate ();
			Solution& operator[] (int i_th);
			
			Population& operator=  (const Population& pop);
			friend ostream& operator<< (ostream& os, const Population& pop);
		
		protected:
			const Solver&   _solver;
			
			vector<Solution*>	_solutions;				
			Solution*			_best_solution;
			Solution*		    _worst_solution;
			Solution*			_best_Mksp_solution;
			double				_best_fitness;
			double				_worst_fitness;
			
			friend void start_ljfrsjfr (Population& pop);
			friend void start_mct      (Population& pop);
			friend void start_random   (Population& pop);
			friend void start_all      (Population& pop);
			friend void start_minmin   (Population& pop);
			friend void start_rc       (Population& pop);
			friend void start_kpb      (Population& pop);
			friend void start_all2     (Population& pop);
			
			friend void select_random (Population& pop);
			friend void select_best (Population& pop);
			friend void select_linear_ranking (Population& pop);
			friend void select_exp_ranking (Population& pop);
			friend void select_binary_tournament (Population& pop);
			friend void select_n_tournament (Population& pop);
			
			friend vector<Solution> random_sel (const Population& pop, int n, float extra_p);
			friend vector<Solution> best_sel (const Population& pop, int n, float extra_p);
			friend vector<Solution> linear_ranking_sel (const Population& pop, int n, float extra_p);
			friend vector<Solution> exp_ranking_sel (const Population& pop, int n, float extra_p);
			friend vector<Solution> binary_tournament_sel (const Population& pop, int n, float extra_p);
			friend vector<Solution> n_tournament_sel (const Population& pop, int n, float extra_p);
	};

	requires class Local_Search_Engine {
		public:
			Local_Search_Engine (const Solver& solver);
			~Local_Search_Engine ();
			
			void apply_to (Solution& sol);
		
		protected:
			const Solver& _solver;
	};

	typedef struct 
	{
		int pos;
		int new_val;
	} vector_changes;

	requires class Solution_Changes {
		public:
			Solution_Changes (const Problem& pbm);
			~Solution_Changes ();
			
			friend opacket2& operator<< (opacket2& op, const Solution_Changes& sol_cha);
			friend ipacket2& operator>> (ipacket2& ip, Solution_Changes& sol_cha);
			Solution_Changes& operator= (const Solution_Changes& sol_cha);
			
			void differences(const Solution& old_sol, const Solution& new_sol);
			void accumulate(const Solution& old_sol, const Solution& new_sol);
			void apply_to(Solution &sol);
		
		protected:
			const Problem& _pbm;

			double		 _new_local_makespan;
			double       _new_flowtime;
			double       _new_local_avg_utilization;	 
			double		 _new_fitness;
			bool		 _new_feasible;
			vector<vector_changes> _schedule_changes;
			int			 _last_machine;
	};
			
	requires bool terminateQ (const Solver& solver);
	requires Solution recombine (const Solver& solver, vector<Solution>& sols);


	//	PROVIDED CLASSES AND METHODS ---------------------------------------
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
	
	typedef struct {
		int lsearch_id;
		int nb_lsearch_iterations;
		double nb_ls_not_improving_it;
		float lsearch_extra_parameter;
	} LS_config;

	provides class Setup {
		public:
			Setup ();
			
			friend ostream& operator<< (ostream& os, const Setup& setup);
			friend istream& operator>> (istream& is, Setup& setup);
			friend opacket2& operator<< (opacket2& op, const Setup& setup);
			friend ipacket2& operator>> (ipacket2& ip, Setup& setup);
			
			// About execution
			int  nb_independent_runs () const;
			int  nb_generations () const;
			float max_time_to_spend () const;
			int  population_size () const;
			int population_height () const;
			int population_width () const;
			int  max_repetitions_to_diversify () const;
			int  nb_solutions_to_recombine () const;
			int  nb_recombinations () const;
			int  nb_mutations () const;
			
			void set_nb_independent_runs (int i);
			void set_nb_generations (int i);
			void set_max_time_to_spend (float f);
			void set_population_height (int i);
			void set_population_width (int i);	
			void set_population_size (int i);
			void set_max_repetitions_to_diversify (int i);
			void set_nb_solutions_to_recombine (int i);
			void set_nb_recombinations (int i);	
			void set_nb_mutations (int i);
			void set_parallel_mode (int i);
			
			/*Algorithm options*/
			int   start_choice () const;
			bool  add_only_if_better () const;
                        int   neighborhood_pattern () const;
			int   recombination_order () const;
			int   mutation_order () const;
			int   recombine_choice () const;
			int   select_choice () const;
			float select_extra_parameter () const;
			int   cross_choice () const;
			int   recombine_selection () const;
			float rec_selection_extra () const;
			int   mutate_choice () const;
			float mutate_extra_parameter () const;
			int   mutate_selection () const;
			float mut_selection_extra () const;
			int   local_search_choice() const;
			int   nb_local_search_iterations() const;
			double   nb_ls_not_improving_iterations() const;
			float lsearch_extra_parameter() const;
			vector<LS_config> slave_local_search() const;
			
			ofstream* trace () const;
			int  energy_case () const;
			
			void set_start_choice (int i);
			void set_add_only_if_better (bool b);
                        void set_neighborhood_pattern (int i);
			void set_recombination_order (int i);
			void set_mutation_order (int i);
			void set_recombine_choice (int i);
			void set_select_choice (int i);
			void set_select_extra_parameter (float f);
			void set_cross_choice (int i);
			void set_recombine_selection (int i);
			void set_rec_selection_extra (float f);
			void set_mutate_choice (int i);
			void set_mutate_extra_parameter (float f);
			void set_mutate_selection (int i);
			void set_mut_selection_extra (float f);
			void set_local_search_choice(int i);
			void set_nb_local_search_iterations (int i);
			void set_nb_ls_not_improving_iterations (double d);
			void set_lsearch_extra_parameter (float f);
			void set_slave_local_search(const vector<LS_config>& v);
			void set_trace (ofstream* of);
			void set_energy_case (int i);
		
		protected:
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
			int _select_choice;
            int _neighborhood_pattern;
			int _recombination_order;
			int _mutation_order;
			int _recombine_choice;
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
			vector<LS_config> _slave_local_search;
			ofstream* _trace;
	};

	provides class Setup_IR: public Setup {
		public:
			Setup_IR ();
			
			void set_population_size					(int i);
                        void set_population_height				(int i);
			void set_population_width				(int i);
			void set_max_repetitions_to_diversify		(int i);
			void set_nb_solutions_to_recombine			(int i);
			void set_nb_recombinations					(int i);	
			void set_nb_mutations						(int i);  
			void set_start_choice						(int i);
			void set_add_only_if_better					(bool b);
                        void set_neighborhood_pattern			(int i);
			void set_recombination_order			(int i);
			void set_mutation_order					(int i);
			void set_recombine_choice				(int i);
			void set_select_choice						(int i);
			void set_select_extra_parameter				(float f);
			void set_cross_choice						(int i);
			void set_recombine_selection				(int i);
			void set_rec_selection_extra				(float f);
			void set_mutate_choice						(int i);
			void set_mutate_extra_parameter				(float f);
			void set_mutate_selection					(int i);
			void set_mut_selection_extra				(float f);
			void set_local_search_choice				(int i);
			void set_nb_local_search_iterations			(int i);
			void set_nb_ls_not_improving_iterations		(double d);
			void set_lsearch_extra_parameter			(float f);

			void set_population_size					(int lim_inf, int lim_sup, char distr);
                        void set_population_height				(int lim_inf, int lim_sup, char distr);
			void set_population_width				(int lim_inf, int lim_sup, char distr);
			void set_max_repetitions_to_diversify		(int lim_inf, int lim_sup, char distr);
			void set_nb_solutions_to_recombine			(int lim_inf, int lim_sup, char distr);
			void set_nb_recombinations					(int lim_inf, int lim_sup, char distr);	
			void set_nb_mutations						(int lim_inf, int lim_sup, char distr);  
			void set_start_choice						(int lim_inf, int lim_sup, char distr);
			void set_add_only_if_better					(double p);
                        void set_neighborhood_pattern			(int lim_inf, int lim_sup, char distr);
			void set_recombination_order			(int lim_inf, int lim_sup, char distr);
			void set_mutation_order					(int lim_inf, int lim_sup, char distr);
			void set_recombine_choice				(int lim_inf, int lim_sup, char distr);
			void set_select_choice						(int lim_inf, int lim_sup, char distr);
			void set_select_extra_parameter				(float lim_inf, float lim_sup, char distr);
			void set_cross_choice						(int lim_inf, int lim_sup, char distr);
			void set_recombine_selection				(int lim_inf, int lim_sup, char distr);
			void set_rec_selection_extra				(float lim_inf, float lim_sup, char distr);
			void set_mutate_choice						(int lim_inf, int lim_sup, char distr);
			void set_mutate_extra_parameter				(float lim_inf, float lim_sup, char distr);
			void set_mutate_selection					(int lim_inf, int lim_sup, char distr);
			void set_mut_selection_extra				(float lim_inf, float lim_sup, char distr);
			void set_local_search_choice				(int lim_inf, int lim_sup, char distr);
			void set_nb_local_search_iterations			(int lim_inf, int lim_sup, char distr);
			void set_nb_ls_not_improving_iterations		(double lim_inf, double lim_sup, char distr);
			void set_lsearch_extra_parameter			(float lim_inf, float lim_sup, char distr);
			
			/*Simple setup constructor*/
			void fill_setup(Setup& setup, int n) const;
		
		protected:
			template <typename type>
			struct param {
				type lim_inf;
				type lim_sup;
				char distr;
			};
			
			param<int> _population_size_param;
			param<int> _max_repetitions_to_diversify_param;
			param<int> _nb_solutions_to_recombine_param;
			param<int> _nb_recombinations_param;
			param<int> _nb_mutations_param;
			param<int> _start_choice_param;
			double _add_only_if_better_probl;
                        param<int> _neighborhood_pattern_param;
			param<int> _recombination_order_param;
			param<int> _mutation_order_param;
			param<int> _recombine_choice_param;	
			param<int> _select_choice_param;
			param<float> _select_extra_parameter_param;
			param<int> _cross_choice_param;
			param<int> _recombine_selection_param;
			param<float> _rec_selection_extra_param;
			param<int> _mutate_choice_param;
			param<float> _mutate_extra_parameter_param;
			param<int> _mutate_selection_param;
			param<float> _mut_selection_extra_param;
			param<int> _local_search_choice_param;
			param<int> _nb_local_search_iterations_param;
			param<double> _nb_ls_not_improving_iterations_param;
			param<float> _lsearch_extra_parameter_param;
			
			double generate_value (double inf, double sup, char distr, int n) const;
			int generate_value (int inf, int sup, char distr, int n) const;
	};
	
	class Solver {
		public:
			Solver (const Problem& pbm, const Setup& setup);
			virtual ~Solver ();
			
			const Problem& problem () const;
			const Setup&   setup () const;
			
			// Execution
			virtual void run () = 0;
			
			// Global information
			Solution    best_solution () const;
			Solution    best_Mksp_solution () const;
			/*Although it is problem dependent, solution with best makespan value*/
			/*must be kept as best makespan valued individual it is no allways the best fitted.*/
			/*This represents a lost of generecity. A future version of MALLBA Library should permit*/
			/*dealing with various best solutions considering more than one criteria generically*/
			
			double      best_cost () const;
			float       time_spent () const;
			float		time_best  () const;
			
			// Independent run information
			int         independent_run () const;
			int         generation () const;
			Population  current_population () const;
			Solution    current_best_solution () const;
			Solution	current_best_Mksp_solution () const;
			double      current_best_cost () const;
			int         nb_repetitions_best_solution () const;
			float       current_time_spent () const;
		
		protected:
			const Problem& _problem;
			const Setup&   _setup;
			const bool     _minimizing;
			
			Solution   _best_solution;
			Solution   _best_Mksp_solution;
			double     _best_cost;
			float      _time_spent;	
			float	   _time_best;
			int        _independent_run;
			int        _generation;		
			Population _current_population;			
			int        _nb_repetitions_best_solution;
			float      _current_time_spent;
			Local_Search_Engine _local_search_engine;
			
			void  update_best_values ();		
	};

	class Solver_Seq: public Solver {
		public:
			Solver_Seq (const Problem& pbm, const Setup& setup);
			~Solver_Seq ();
			
			// Execution
			void run ();
	};
	
	class Solver_Lan_IR: public Solver {
		public:
			Solver_Lan_IR (const Problem& pbm, const Setup& setup);
			~Solver_Lan_IR ();
			
			Setup	best_configuration () const;
			int		last_iteration () const;
			
			// Execution	
			void run ();
		
		protected:
			int			_comm_size; // communication size (i.e. number of processors)
			Setup		_best_config;
			int			_last_it;
			
			void master();
			void slave();
	};

	provides class Solver_Lan_IR_DS: public Solver {
		public:
			Solver_Lan_IR_DS (const Problem& pbm, const Setup_IR& setup_IR);
			~Solver_Lan_IR_DS();
			
			Setup	best_configuration () const;
			int	last_iteration () const;
			
			// Execution
			void run ();
		
		private:
			const Setup_IR&   _setup_IR;
			
			int			_comm_size; 
			Setup			_best_config;
			int			_last_it;
			
			void master();
			void slave();
	};

	class Solver_Lan_MS: public Solver {
		public:
			Solver_Lan_MS (const Problem& pbm, const Setup& setup);
			~Solver_Lan_MS ();
			
			// Execution
			void run ();
			
			// Global information
			float comm_time_spent () const;
		protected:
			int   _comm_size; // communication size (i.e. number of processors)
			float _comm_time_spent; 
			
			void master();
			void slave();
	};

	/*--------------------AUXILIAR ATTRIBUTES DEFINITIONS----------------*/
	   extern bool ENERGY_CASE;
	/*-------------------------------------------------------------------*/
}
#endif
