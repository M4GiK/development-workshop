/*************************************************************************/
/* FILE: GeneticSearch.hh                                                */
/*************************************************************************/
#ifndef INC_GeneticSearch
#define INC_GeneticSearch

#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

#include <mpi.h>
#include "mallba.hh"
#include <mpicxx.h>
#include "iopacket2_nl.hh"
#include "no_leda.hh"

#include "Distribution.hh"
#include "ConstantDistribution.hh"
#include "NormalDistribution.hh"
#include "UniformDistribution.hh"
#include "ExponentialDistribution.hh"
#include "ZipfDistribution.hh"
#include "TriangleDistribution.hh"
#include "InitialSolutions.hh"

using namespace std;
using namespace AdHocHeuristics;

//#define TRACE_FIT		/*to keep trace of best fitness values (a trace ofstream must be set in Setup)*/
//#define TRACE_MKSP	/*to keep trace of best makespan values (a trace ofstream must be set in Setup)*/
#define MULTIOBJECTIVE	/*to perform a multiobjective serarch considering makespan and mean flowtime simultaneously*/

skeleton NGeneticSearch {

    requires class Problem;
    requires class Solution;
    requires class Population;

    provides class Setup;
	provides class Setup_IR;
    provides class Solver;
    provides class Solver_Seq;
	provides class Solver_Lan_IR;

    /*-------------------REQUIRED CLASSES AND METHODS--------------------*/
    requires class Problem {
		public :
			Problem ();
			~Problem();
			
			friend ostream& operator<< (ostream& os, const Problem& pbm);
			friend istream& operator>> (istream& is, Problem& pbm);
			friend opacket2& operator<< (opacket2& op, const Problem& pbm);
			friend ipacket2& operator>> (ipacket2& ip, Problem& pbm);
			
			Direction direction () const;
			
			void set_problem (const vector<double>& task_info, 
                          const vector<double>& machine_info,
                          const vector<double>& ready_time_info,
                          const double current_time);
			
			void set_problem (const matrix<double>& etc, const vector<double>& ready_time_info, 
				const double current_time);
		
		protected:
			void compute_etc ();
			void fix_ready_times_to_now ();
			void fill_workload_mips ();
		
		public:     
			int             number_tasks;
			int             number_machines;
			vector<double>  workload;
			vector<double>  mips;
			double          now;
			vector<double>  ready_times;
			matrix<double>  etc;
	};

    requires class Solution {
		public :
			Solution (const Problem&  pbm);
			Solution (const Solution& sol);
			~Solution();
			Solution& operator= (const Solution& sol);
			
			bool   is_better_than  (const Solution& sol) const;
			friend bool operator== (const Solution& sol1,const Solution& sol2);
			friend bool operator!= (const Solution& sol1,const Solution& sol2);
			friend ostream& operator<< (ostream& os, const Solution& sol);
			friend istream& operator>> (istream& is, Solution& sol);
			friend opacket2& operator<< (opacket2& op, const Solution& sol);
			friend ipacket2& operator>> (ipacket2& ip, Solution& sol);
			
			void   set_initial ();
			double fitness () const;
			void   update_fitness ();
			bool   feasible() const;
			void   update_feasible ();
			void   mutate (const Solver& solver);
			double similarity (const Solution& sol) const;
			double key () const;
			
			friend void cross_one_point (const Solver& solver, Solution& sol1, Solution& sol2);
			friend void cross_two_points (const Solver& solver,Solution& sol1, Solution& sol2);
			friend void cross_uniform (const Solver& solver, Solution& sol1, Solution& sol2);
			friend void cross_fitness_based (const Solver& solver, Solution& sol1, Solution& sol2);
			friend void crossover_pmx (const Solver& solver, Solution& sol1, Solution& sol2);
			friend void crossover_ox (const Solver& solver, Solution& sol1, Solution& sol2);
			friend void crossover_cx (const Solver& solver, Solution& sol1, Solution& sol2);
			friend void mutate_move (const Solver& solver, Solution& sol);
			friend void mutate_swap (const Solver& solver, Solution& sol);
			friend void mutate_both (const Solver& solver, Solution& sol);
			friend void mutate_rebalancing (const Solver& solver, Solution& sol);
			friend bool improvement (const Solution& s1, const Solution& s2);
		
		protected:
			const Problem&  problem;
			double          _fitness;
			double          _feasible;
			double			_key;		/*contains the key to acces to tha hash table when using struggle policy*/
		
		public:    
			void heuristic (heuristic_type choice);
			void perturbate_randomly  ();
		
		protected:              
			void compute_most_loaded_machines       (matrix<int>& sched, vector<int>& machines);
			void transform_schedule_representation  (matrix<int>& sched);
			void transform_schedule_representation  (vector<int>& perm, vector<int>& limits);
			void apply_schedule_representation      (const matrix<int>& sched);
			void apply_schedule_representation      (const vector<int>& perm, const vector<int>& limits);
			int  select_task_from_another_machine   (int machine, const matrix<int>& sched);
			void get_shuffled_tasks    				(vector<int>& tasks);
			void get_shuffled_machines 				(vector<int>& machines);
		
		public:	
			vector<int>     schedule;
			vector<double>  completion;        
			double			local_makespan;    /* Makespan of used machines  */        
			double          local_avg_utilization;
			double          flowtime;	
			double			energy;
	};

    requires class Population {
		public:
			Population (const Solver& solver);
			Population (const Solver& solver, const int size);
			Population (const Population& pop);
			~Population();
			
			Solution& best_solution () const;		/*Solution with best fitness value*/
			Solution& best_Mksp_solution () const;	/*Solution with best makespan value*/
			double    best_fitness  () const;
			double    worst_fitness () const;
			
			void start ();
			void update ();
			void sort ();
			int  size () const;
			void quit_worst (const int n);
			void keep_best (const int n);
			Population select () const;
			void cross ();
			void mutate ();
			void replace (Population& pop);
			
			Population& operator=  (const Population& pop);
			Population& operator+= (const Population& pop);
			friend ostream& operator<< (ostream& os, const Population& pop);
			friend istream& operator>> (istream& is, Population& pop);
		
		protected:
			const Solver&  _solver;
			const Problem& _problem;
			const Setup&   _setup;
			
			vector<Solution*>	_solutions;
			Solution*			_best_solution;
			Solution*			_best_Mksp_solution;
			double				_best_fitness;
			double				_worst_fitness;        
			double				_hmin;				/*Minimum key value (to build the hash table)*/
			double				_hmax;				/*Maximum key value (to build the hash table)*/
			
			friend void start_ljfrsjfr (Population& pop);
			friend void start_mct      (Population& pop);
			friend void start_random   (Population& pop);
			friend void start_all      (Population& pop);
			friend void start_minmin   (Population& pop);
			friend void start_rc       (Population& pop);
			friend void start_kpb      (Population& pop);
			friend void start_all2     (Population& pop);
			
			friend Population select_random (const Population& pop);
			friend Population select_best (const Population& pop);
			friend Population select_linear_ranking (const Population& pop);
			friend Population select_exp_ranking (const Population& pop);
			friend Population select_binary_tournament (const Population& pop);
			friend Population select_n_tournament (const Population& pop);
		
		private:
			void struggle_replace (Population& pop);
    };
	
	class HashTable {
		public:
			HashTable (int size, double hmin, double hmax);
			~HashTable ();
			
			bool get (Solution* s1, Solution*& s2);
			void put (Solution* s);
			void remove (Solution* s);
		
		private:
			vector<double> _t;
			matrix<Solution*> _v;
	};

    requires void crossover (const Solver& solver, Solution& sol1, 
    						 Solution& sol2);
    requires bool terminateQ (const Solver& solver);

    /*-------------------PROVIDED CLASSES AND METHODS--------------------*/
    enum SelectChoice { SelectRandom, SelectBest, SelectLinearRanking,
                        SelectExpRanking, SelectBinaryTournament,
                        SelectNTournament 
                      };
    enum CrossChoice  { CrossOnePoint, CrossTwoPoints, CrossUniform,
    					CrossFitnessBased, CrossPMX, CrossOX, CrossCX
    				  };
    enum MutateChoice { MutateMove, MutateSwap, MutateBoth, 
    					MutateRebalancing
    				  };    
    enum StartMethod  { StartLJFRSJFR, StartMCT, StartRandom, StartAll,
						StartMinMin, StartRelativeCost, StartKPB, StartAll2
                      };   
	/*StartAll = 1xStartLJFRSJFR & 1xStartMCT & (n-2)xStartRandom*/
	/*StartAll2 = 1xStartLJFRSJFR & 1xStartMCT & 1xStartMinMin & 1xStartRelativeCost & 1xStartKPB & (n-5)xStartRandom*/
    
    provides class Setup {
		public:
			Setup ();
			
			friend ostream& operator<< (ostream& os, const Setup& setup);
			friend istream& operator>> (istream& is, Setup& setup);
			friend opacket2& operator<< (opacket2& op, const Setup& setup);
			friend ipacket2& operator>> (ipacket2& ip, Setup& setup);
			
			int   nb_independent_runs () const;
			int   nb_evolution_steps () const;
			float max_time_to_spend () const;
			int   nb_repetitions_to_end () const;
			int   population_size () const;
			int   intermediate_population_size () const;
			float cross_probability () const;
			float mutate_probability () const;
			int   select_choice () const;
			float select_extra_parameter () const;
			int   cross_choice () const;
			float cross_extra_parameter () const;
			int   mutate_choice () const;
			float mutate_extra_parameter () const;
			bool  replace_only_if_better () const;
			bool  replace_generational () const;
			bool  struggle_replace () const;
			int	  struggle_extra_parameter () const;
			int   start_choice () const;
			const string& user_information () const;
			ofstream* trace () const;
			int   energy_case () const;
			
			void set_nb_independent_runs (int i);
			void set_nb_evolution_steps (int i);
			void set_max_time_to_spend (float f);
			void set_nb_repetitions_to_end (int i);
			void set_population_size (int i);
			void set_intermediate_population_size (int i);
			void set_cross_probability (float f);
			void set_mutate_probability (float f);
			void set_select_choice (int i);
			void set_select_extra_parameter (float f);
			void set_cross_choice (int i);
			void set_cross_extra_parameter (float f);
			void set_mutate_choice (int i);
			void set_mutate_extra_parameter (float f);
			void set_replace_only_if_better (bool b);
			void set_replace_generational (bool b);
			void set_struggle_replace (bool b);
			void set_struggle_extra_parameter (int i);
			void set_start_choice (int i);
			void set_user_information (const string& s);
			void set_trace (ofstream* of);
			void set_energy_case (int i);
		
		protected:
			int     _nb_independent_runs;
			int     _nb_evolution_steps;
			float   _max_time_to_spend;
			int     _nb_repetitions_to_end;
			int     _population_size;        
			int     _intermediate_population_size;
			float   _cross_probability;
			float   _mutate_probability;
			int     _select_choice;
			float   _select_extra_parameter;
			int     _cross_choice;
			float   _cross_extra_parameter;
			int     _mutate_choice;
			float   _mutate_extra_parameter;
			bool    _replace_only_if_better;
			bool    _replace_generational;
			bool    _struggle_replace;
			int		_struggle_extra_parameter;
			int     _start_choice;
			string  _user_information;
			ofstream* _trace;
    };

	provides class Setup_IR: public Setup {
		public:
			Setup_IR ();
			
			void set_nb_repetitions_to_end			(int i);
			void set_population_size				(int i);
			void set_intermediate_population_size	(int i);
			void set_cross_probability				(float f);
			void set_mutate_probability				(float f);
			void set_select_choice					(int i);
			void set_select_extra_parameter			(float f);
			void set_cross_choice					(int i);
			void set_cross_extra_parameter			(float f);
			void set_mutate_choice					(int i);
			void set_mutate_extra_parameter			(float f);
			void set_replace_only_if_better			(bool b);
			void set_replace_generational			(bool b);
			void set_struggle_replace				(bool b);
			void set_struggle_extra_parameter		(int i);
			void set_start_choice					(int i);
			
			void set_nb_repetitions_to_end			(int lim_inf, int lim_sup, char distr);
			void set_population_size				(int lim_inf, int lim_sup, char distr);
			void set_intermediate_population_size	(int lim_inf, int lim_sup, char distr);
			void set_cross_probability				(float lim_inf, float lim_sup, char distr);
			void set_mutate_probability				(float lim_inf, float lim_sup, char distr);
			void set_select_choice					(int lim_inf, int lim_sup, char distr);
			void set_select_extra_parameter			(float lim_inf, float lim_sup, char distr);
			void set_cross_choice					(int lim_inf, int lim_sup, char distr);
			void set_cross_extra_parameter			(float lim_inf, float lim_sup, char distr);
			void set_mutate_choice					(int lim_inf, int lim_sup, char distr);
			void set_mutate_extra_parameter			(float lim_inf, float lim_sup, char distr);
			void set_replace_only_if_better			(double p);
			void set_replace_generational			(double p);
			void set_struggle_replace				(double p);
			void set_struggle_extra_parameter		(int lim_inf, int lim_sup, char distr);
			void set_start_choice					(int lim_inf, int lim_sup, char distr);
			
			/*Simple setup constructor*/
			void fill_setup(Setup& setup, int n) const;
		
		protected:
			template <typename type>
			struct param {
				type lim_inf;
				type lim_sup;
				char distr;
			};
			
			param<int> _nb_repetitions_to_end_param;
			param<int> _population_size_param;        
			param<int> _intermediate_population_size_param;
			param<float> _cross_probability_param;
			param<float> _mutate_probability_param;
			param<int> _select_choice_param;
			param<float> _select_extra_parameter_param;
			param<int> _cross_choice_param;
			param<float> _cross_extra_parameter_param;
			param<int> _mutate_choice_param;
			param<float> _mutate_extra_parameter_param;
			double _replace_only_if_better_probl;
			double _replace_generational_probl;
			double _struggle_replace_probl;
			param<int> _struggle_extra_parameter_param;
			param<int> _start_choice_param;
			
			double generate_value (double inf, double sup, char distr, int n) const;
			int generate_value (int inf, int sup, char distr, int n) const;
    };

    provides class Solver {
		public:
			Solver (const Problem& pbm, const Setup& setup);
			virtual ~Solver ();
			
			const Problem& problem () const;
			const Setup&   setup () const;
			
			// Execution
			virtual void run () =0;
			virtual void perform_one_independent_run () =0;
			virtual void perform_one_phase () =0;
			virtual void set_current_solution (const Solution& sol);
			virtual void set_current_solution (const Solution& sol,const double cost);
			
			// Global information
			virtual int       independent_run () const;
			virtual float     time_spent () const;
			virtual Solution  best_solution () const;
			virtual Solution  best_Mksp_solution () const;
			/*Although it is problem dependent, solution with best makespan value*/
			/*must be kept as best makespan valued individual it is no allways the best fitted.*/
			/*This represents a lost of generecity. A future version of MALLBA Library should permit*/
			/*dealing with various best solutions considering more than one criteria generically*/
			
			virtual int       independent_run_best_found () const;
			virtual int       iteration_best_found () const;
			virtual float     time_best_found () const;
			virtual double    best_cost () const;
			virtual double    worst_cost () const;
			virtual int       nb_repetitions_best_solution () const;
			
			// Independent run information
			virtual Solution  current_solution () const;
			virtual double    current_cost () const;
			virtual Solution  current_initial_solution () const;
			virtual double    current_initial_cost () const;
			virtual float     current_time_spent () const;
			virtual Solution  current_best_solution () const;
			virtual int       current_best_solution_iteration () const;
			virtual float     current_best_solution_time () const;
			virtual double    current_best_cost () const;
			virtual double    current_worst_cost () const;
			
			// Phase information
			virtual int  current_iteration () const;
		
		protected:
			const Problem& _problem;
			const Setup&   _setup;
			const bool     _minimizing;
			
			int      _independent_run;
			float    _time_spent;
			Solution _best_solution;
			Solution _best_Mksp_solution;
			int      _independent_run_best_found;
			int      _iteration_best_found;
			float    _time_best_found;
			double   _best_cost;
			double   _worst_cost;
			int      _nb_repetitions_best_solution;
			Solution _current_solution;
			double   _current_cost;
			int      _current_iteration;
			Solution _current_initial_solution;
			double   _current_initial_cost;
			float    _current_time_spent;
			Solution _current_best_solution;		
			int      _current_best_solution_iteration;
			float    _current_best_solution_time;
			double   _current_best_cost;
			double   _current_worst_cost;
			
			Population _parents;
			Population _offsprings;     
			
			virtual void  update_state (const Solution& sol);
			virtual void  update_Mksp_state (const Solution& sol);
	};

    provides class Solver_Seq: public Solver {
		public:
			Solver_Seq (const Problem& pbm, const Setup& setup);
			~Solver_Seq ();
			
			// Execution
			void run ();
			void perform_one_independent_run ();
			void perform_one_phase ();
		
		private:
			// Auxiliar attributes
			float       _gtime,_ltime;
			bool        _1st_phase_belongs_independent_run;       
	};

	provides class Solver_Lan_IR: public Solver {
		public:
			Solver_Lan_IR (const Problem& pbm, const Setup& setup);
			~Solver_Lan_IR();
			
			Setup	best_configuration () const;	/*configuration that found best solution*/
			int	last_iteration () const;
			
			// Execution
			void run ();
			void perform_one_independent_run ();
			void perform_one_phase ();
		
		private:
			// Auxiliar attributes
			int			_comm_size; 
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
			void perform_one_independent_run ();
			void perform_one_phase ();
		
		private:
			const Setup_IR&   _setup_IR;
			
			// Auxiliar attributes
			int			_comm_size; 
			Setup		_best_config;
			int			_last_it;

			void master();
			void slave();
	};
	extern bool ENERGY_CASE;
}
#endif
