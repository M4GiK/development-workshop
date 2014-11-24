/*************************************************************************/
/* FILE: TabuSearch.hh                                                   */
/*************************************************************************/

#ifndef INC_TabuSearch
#define INC_TabuSearch

#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <deque>
#include <string>
#include <set>
#include <algorithm>
#include "InitialSolutions.hh"
#include "mallba.hh"
#include "no_leda.hh"
using namespace std;
using namespace AdHocHeuristics;

#define _TRACEAR_

skeleton TabuSearch {

	requires class Problem;
	requires class Solution;
	requires class Movement;
	requires class TabuStorage;

	provides class Setup;
	provides class Solver;
	provides class Solver_seq;

    /*-------------------REQUIRED CLASSES AND METHODS--------------------*/
    
    requires class Problem {
     public :
		Problem ();
        ~Problem();

        friend ostream& operator<< (ostream& os, const Problem& pbm);
        friend istream& operator>> (istream& is, Problem& pbm);

        Direction direction () const;

        void set_problem (const vector<double>& task_info,
                          const vector<double>& machine_info,
                          const vector<double>& ready_time_info,
                          const double          current_time);
		void set_problem (const matrix<double>& etc,
						  const vector<double>& ready_time_info,
						  const double          current_time);
	 protected:
		void  compute_etc ();
		void  fix_ready_times_to_now ();
		void  compute_hash_values ();

     public:
        int              number_tasks;
        int              number_machines;
        vector<double>   workload;
        vector<double>   mips;
        double           now;
        vector<double>   ready_times;
        matrix<double>   etc;
        matrix<int> 	 hashing;
    };

 	requires typedef int item;
    requires typedef AdHocHeuristics::heuristic_type heuristic_type;

	requires class Solution {
     public:
		Solution (Problem&  pbm);
		Solution (const Solution& sol);
		~Solution ();

		Solution& operator=    (const Solution& sol);
		bool   is_better_than  (const Solution& sol) const;
		bool   close_to        (const Solution& sol) const;
		friend bool operator== (const Solution& sol1,const Solution& sol2);
		friend bool operator!= (const Solution& sol1,const Solution& sol2);

		friend ostream& operator<< (ostream& os, const Solution& sol);
		friend istream& operator>> (istream& is, Solution& sol);

		double fitness () const;
        void   update_fitness ();
		double delta (const Movement& move) const;

		void   set_initial (TabuStorage& ts, int start_choice = 0);
		void   perturbate_randomly ();

		void   apply   (const Movement& move);
		void   unapply (const Movement& move);

		bool   aspiration (const Movement& move, const TabuStorage& tstore,
		                   const Solver& solver, double& flowtime) const;
		
		void   reward     (TabuStorage& ts);
		void   unreward   (TabuStorage& ts);
		void   penalize   (TabuStorage& ts);
		void   unpenalize (TabuStorage& ts);
		void   escape     (TabuStorage& ts);

		int   size () const;
		item  operator[] (int i_th) const;
		int   hash() const;

	 private:
		Problem&       problem;
		unsigned int   hash_id;

	 public:
		bool  iterated_local_search        (TabuStorage& ts,Solution& sol);
		bool  local_search                 (TabuStorage& ts,Solution& sol);
		bool  local_search_extended        (TabuStorage& ts,Solution& sol);
		
		bool  reward_by_neighborhood       (TabuStorage& ts,Solution& sol);
		bool  unreward_by_neighborhood     (TabuStorage& ts,Solution& sol);
		bool  reward_by_complex_moves      (TabuStorage& ts,Solution& sol);
		bool  unreward_by_complex_moves    (TabuStorage& ts,Solution& sol);
		bool  reward_by_iterated_ls        (TabuStorage& ts,Solution& sol);				
		bool  unreward_by_iterated_ls      (TabuStorage& ts,Solution& sol);
		bool  reward_by_elite_freqs        (TabuStorage& ts,Solution& sol);
		bool  unreward_by_elite_freqs      (TabuStorage& ts,Solution& sol);
		
		bool  penalize_task_distribution   (TabuStorage& ts,Solution& sol);
		bool  unpenalize_task_distribution (TabuStorage& ts,Solution& sol);
        //bool  penalize_by_fitness_bias     (TabuStorage& ts,Solution& sol);
		//bool  unpenalize_by_fitness_bias   (TabuStorage& ts,Solution& sol);
        bool  penalize_by_dynamicity       (TabuStorage& ts,Solution& sol);
		bool  unpenalize_by_dynamicity     (TabuStorage& ts,Solution& sol);
		//bool  penalize_by_host_dynamicity  (TabuStorage& ts,Solution& sol);
		//bool  unpenalize_by_host_dynamicity(TabuStorage& ts,Solution& sol);
		
		void  compute_most_loaded_machines (vector<int>& mach,double itvl);
		void  compute_less_loaded_machines (vector<int>& mach,double pcnt);
		void  compute_random_machines      (vector<int>& mach,double pcnt);
	 	bool  host_participates            (int i);
	 	void  build_matrix_representation  ();
	 	void  compute_completion_times     ();
	 	void  compute_makespan             ();
	 	void  compute_utilization          ();
	 	void  compute_flowtime     	       ();
	 	double compute_flowtime_for        (int machine);
	 	void  compute_hash_id			   ();
	 	void  compute_energy               ();
	 	void  recompute_completion_times   (const Movement& move);
	 	void  recompute_makespan 		   (const Movement& move);
	 	void  recompute_utilization 	   (const Movement& move);
	 	void  recompute_flowtime 		   (const Movement& move);
	 	void  recompute_hash_id		       (const Movement& move);
	 	void  reorder_down 	               (int mach, int pos);
	 	void  reorder_up   	               (int mach, int pos);
	 	void  bring_closer 	               (int mach, int pos);
        void  reorder			           (int mach, int pos, const Movement& move);
	 
	 public:
        vector<int>     schedule;
        vector<int>		positions_;
        matrix<int>		schedule_;
        vector<double>  completions;
        double          sum_local_completions;
        int 			nb_machines_involved;
        double			local_makespan;
        double          local_avg_utilization;
        vector<double>  flowtimes;
        double          flowtime;
        double			energy;
		//double        total_makespan;        /* Makespan of all  machs */
        //double        total_avg_utilization; /*Utilization of all machs*/

    public:
        /* Used for reward, unreward, penalize, unpenalize */
        double            old_max_load_interval;        
        double            old_pcnt_min_loaded;
        //matrix<double>  backup_etc;
	};

	enum move_type {transfer, swap};

	requires class Movement {
	 public:
		Movement (const Problem& pbm, Solution& sol);
		Movement (const Movement& move);
		~Movement ();

		Movement& operator=    (const Movement& move);
		friend bool operator== (const Movement& m1, const Movement& m2);
		friend bool operator!= (const Movement& m1, const Movement& m2);

		friend ostream& operator<< (ostream& os, const Movement& move);
		friend istream& operator>> (istream& is, Movement& move);
		
		void  generate        (move_type type_ = swap);
		void  invert          ();
		int   tabulife        () const;
		void  set_tabulife    (int i);
		void  assign_transfer (int task,  int machine, int tabulife = 0);
		void  assign_swap     (int task1, int task2,   int tabulife = 0);
		bool  is_null         () const;

	 private:
		const Problem&   _problem;
		Solution&        _solution;
		int              _tabulife;

     public:
		move_type type;
		/* Used to store transfer-move information */
		int   task_to_transfer;
		int   machine_to_transfer;
		int   previous_machine;
		/* Used to store swap-move information     */
		int   task1_to_swap;
		int   task2_to_swap;
	};

	requires class TabuStorage {
	 public:
		TabuStorage (Solver& solver);
		~TabuStorage ();

		bool is_in_tabu_storage (const Movement& move, const Solution& sol) const;
		bool is_tabu (const Movement& move, const Solution& sol) const;
		bool aspiration (const Movement& move, const Solution& sol) const;
		void make_tabu (Movement& move, const Solution& sol);
		void make_tabu_inv (Movement& move, const Solution& sol);
		void update ();
		int  size () const;		
				
		void initialize_starting_solution (const Solution& sol);
		void clear  ();
		void clear  (int t);
		void clear  (int t, int m);
		void pin    (int t);
		void update_best_solutions (const Solution& sol, int option = 0);
		void compute_freqs_bests ();
		//void compute_freqs       ();
		//void compute_ifreqs      ();
		void compute_tfreqs      ();
		//void compute_hfreqs      ();

	 public:
	    Solver&   _solver;
		
		/* short term memory (tabu attributes) */
		vector<bool>	 th;		
		matrix<int>    	 tl;
		
		/* long term memory (frequency memory and best solutions) */
		deque<Solution>  best_sols;
		int              nbr_best_sols;		
        int              nbr_moves;
		//matrix<int>    frequency;
		//matrix<int>    ifrequency;
		vector<int>      tfrequency;
		//vector<int>    hfrequency;
        
		matrix<double>  freqs_best;
		//matrix<double>  freqs;
		//matrix<double>  ifreqs;
		vector<double>    tfreqs;	
		//vector<double>  hfreqs;	
	};

	requires bool choose_best_move (const Problem& pbm,const Solution& sol,
	                                const TabuStorage& tstore,
	                                const Solver& solver, Movement& move);
	requires bool choose_best_move_H(const Problem& pb,const Solution& sol,
	                                 const TabuStorage& tstore,
	                                 const Solver& solver, Movement& move);
	requires bool choose_best_move_S(const Problem& pb,const Solution& sol,
	                                 const TabuStorage& tstore,
	                                 const Solver& solver, Movement& move);

	requires bool choose_best_move_from_partition (const Problem& pbm,
	                                const Solution& sol,
	                                const vector<item> partition,
	                                const TabuStorage& tstore,
	                                const Solver& solver, Movement& move);
	
	requires bool terminateQ (const Solver& solver);

    /*-------------------PROVIDED CLASSES AND METHODS--------------------*/
       
	provides class Setup {
   	 public:
		Setup ();

		friend ostream& operator<< (ostream& os, const Setup& setup);
		friend istream& operator>> (istream& is, Setup& setup);

		bool   optimization_strategy () const;
		int    independent_runs () const;
		int    nb_iterations () const;
		float  max_time_to_spend () const;
		int    start_choice () const;
		bool   use_delta_function () const;
		int    tabu_size () const;
		int    min_tabu_status () const;
		int    max_tabu_status () const;
		int    max_repetitions () const;
		int    nb_intensifications () const;
		int    nb_diversifications () const;
        int    elite_size () const;
		int    aspiration_value () const;
		float  max_nb_swaps () const;
		float  max_nb_transfers () const;
		float  max_load_interval () const;
		float  percent_min_load () const;
		const  string& user_information () const;
		int	   energy_case () const;

		void   set_optimization_strategy (bool b);
		void   set_independent_runs (int i);
		void   set_nb_iterations (int i);
		void   set_start_choice (int i);
		void   set_max_time_to_spend (float f);
		void   set_use_delta_function (bool b);
		void   set_tabu_size (int i);
		void   set_min_tabu_status (int i);
		void   set_max_tabu_status (int i);
		void   set_max_repetitions (int i);
		void   set_nb_intensifications (int i);
		void   set_nb_diversifications (int i);
		void   set_elite_size (int i);
		void   set_aspiration_value (int i);
		void   set_max_nb_swaps (float f);
		void   set_max_nb_transfers (float f);
		void   set_max_load_interval (float f);
		void   set_percent_min_load (float f);
		void   set_user_information (const string& s);
		void   set_energy_case (int i);

	 private:
		int    _independent_runs;
		int    _nb_iterations;
		int    _start_choice;
		float  _max_time_to_spend;
		bool   _use_delta_function;
		int    _tabu_size;
		int    _min_tabu_status;
		int    _max_tabu_status;
		int    _max_repetitions;
		int    _nb_diversifications;
		int    _nb_intensifications;
		int    _elite_size;         //problem specific setup variable
		int    _aspiration_value;   //problem specific setup variable
		float  _max_nb_swaps;       //problem specific setup variable
		float  _max_nb_transfers;   //problem specific setup variable
		float  _max_load_interval;  //problem specific setup variable
		float  _percent_min_load;   //problem specific setup variable
        string _user_information;
	};

	provides class Solver {
     public:
		Solver (Problem& pbm, Setup& setup);
		virtual ~Solver ();

		const Problem&     problem () const;
		      Setup&       setup () const;

		// Execution
		virtual void run () =0;
		virtual void perform_one_independent_run () =0;
		virtual void perform_one_phase () =0;
		virtual void set_current_solution (const Solution& sol);
		virtual void set_current_solution (const Solution& sol,
		                                   const double    cost );

		// Global information
		virtual int       independent_run () const;
		virtual float     time_spent () const;
		virtual Solution  best_solution () const;
		virtual int       independent_run_best_found () const;
		virtual int       iteration_best_found () const;
		virtual float     time_best_found () const;
		virtual double    best_cost () const;
		virtual double    worst_cost () const;

		// Independent run information
		virtual Solution  current_solution () const;
		virtual double    current_cost () const;
		virtual int       current_iteration () const;
		virtual Solution  current_initial_solution () const;
		virtual double    current_initial_cost () const;
		virtual float     current_time_spent () const;
		virtual const     Solution& current_best_solution () const;
		virtual int       current_best_solution_iteration () const;
		virtual float     current_best_solution_time () const;
		virtual double    current_best_cost () const;
		virtual double    current_worst_cost () const;

		// Phase information

		virtual int  nb_iterations_performed_in_phase () const;
		virtual bool intensification_in_phase () const;
		virtual bool soft_diversification_in_phase () const;
		virtual bool hard_diversification_in_phase () const;

	 protected:
		Problem&     _problem;
		Setup&       _setup;
		const bool   _minimizing;
		TabuStorage  _tstruct;
		
		int      _independent_run;
		float    _time_spent;
		Solution _best_solution;
		int      _independent_run_best_found;
		int      _iteration_best_found;
		float    _time_best_found;
		double   _best_cost;
		double   _worst_cost;

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
		int      _nb_iterations_performed_in_phase;
		bool     _intensification_in_phase;
		bool     _soft_diversification_in_phase;
		bool     _hard_diversification_in_phase;

		#ifdef _TRACEAR_
			ofstream  _tracefile;
		#endif

		virtual void  update_state ();
	};

	provides class Solver_Seq: public Solver {
     public:
		Solver_Seq (Problem& pbm, Setup& setup);
		~Solver_Seq ();

		// Execution
		void run ();
		void perform_one_independent_run ();
		void perform_one_phase ();

	 private:
		// Auxiliar attributes
		float       _gtime,_ltime;
		Solution    _previous_solution;
		bool        _1st_phase_belongs_independent_run;
		int         _nb_consecutive_repetitions;
	};
    
    /*--------------------AUXILIAR ATTRIBUTES DEFINITIONS----------------*/
    // This attribute tells the skeleton the way it is going to optimize.
    // There are two possible options for MULTIOBJECTIVE:
    // false = Hierarchical: Hierarchically optimizes makespan and flowtime
    // true  = Simultaneous: Simultaneously optimizes makespan and flowtime
       extern bool _MULTIOBJECTIVE_;
       extern bool _ENERGY_CASE_;
    /*-------------------------------------------------------------------*/    	
	
}
#endif
