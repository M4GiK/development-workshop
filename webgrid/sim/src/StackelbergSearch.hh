/*************************************************************************/
/* FILE: GeneticSearch.hh                                                */
/*************************************************************************/

#ifndef INC_GeneticSearch
#define INC_GeneticSearch

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <assert.h>
#include "mallba.hh"
#include "no_leda.hh"
#include "InitialSolutions.hh"
using namespace std;
using namespace AdHocHeuristics;

// This attribute tells the skeleton whether to trace or not for each of
// the improved solutions, its iteration number, its makespan, utilization
// and flowtime
//#define TRACEAR

// This attribute tells the skeleton the way it is going to optimize.
// There are two possible options for MULTIOBJECTIVE:
// false = Hierarchical: Hierarchically optimizes makespan and flowtime
// true  = Simultaneous: Simultaneously optimizes makespan and flowtime

//#define MULTIOBJECTIVE

skeleton StackelbergSearch {

    requires class Problem;
    requires class Solution;
    requires class Population;

    provides class Setup;
    provides class Solver;
    provides class Solver_Seq;

    provides class Solver_HGS;

    /*-------------------REQUIRED CLASSES AND METHODS--------------------*/

    /**
     * Storage class for problem definition: number of machines, mips,
     * number of tasks to schedule. Not much logic here. Class just holds
     * problem parameters.
     */
    requires class Problem {
     public :
		Problem ();
        ~Problem();

        friend ostream& operator<< (ostream& os, const Problem& pbm);
        friend istream& operator>> (istream& is, Problem& pbm);

        Direction direction () const;

        void set_problem (const matrix<double>& etc_info,
						  const vector<double>& ready_time_info,
						  const double          current_time);

        void set_problem (const matrix<double>& etc_info,
						  const matrix<double>& tfp_info,
						  const vector<double>& abort_info,
						  const vector<double>& ready_time_info,
						  const double          current_time);

        void set_problem (const vector<double>& task_info ,
                          const vector<double>& machine_info ,
                          const vector<double>& ready_time_info,
                          const double          current_time);


	 protected:
		void compute_etc ();
		void compute_completion();
		void fix_ready_times_to_now ();

     public:
        int             number_tasks;
        int             number_machines;
        vector<double>  workload;
        vector<double>  mips;
        double          now;
        vector<double>  abort_probability;
        double          abort_max;
        vector<double>  ready_times;
        matrix<double>  etc;
        matrix<double>  tfp;
        matrix<double>  completion;
    };

    /**
     * Represents single solution vector, contains logic for genetic operators,
     * allows transformation between representations.
     * Genetic operators are implemented as friend functions.
     */
    requires class Solution {
     public :
        Solution  (const Problem&  pbm);
        Solution  (const Solution& sol);
        ~Solution ();
        //***********************************************************
        vector<int> solution();
        void set_initial(const Solution& best_solution, double env_length, int env_type);


        //***********************************************************
        Solution& operator=    (const Solution& sol);

        bool   is_better_than  (const Solution& sol) const;
        friend bool operator== (const Solution& sol1,const Solution& sol2);
        friend bool operator!= (const Solution& sol1,const Solution& sol2);

        friend ostream& operator<< (ostream& os, const Solution& sol);
        friend istream& operator>> (istream& is, Solution& sol);

        string print_normal();
        string print_load();
        string print_transformed();

        void   set_initial     ();
        double game_fitness    ();
        double Q_l__fitness(int cmp_start, int cmp_end,
        		double max_completion_single_task__global,
        		double& qle_out, double& qlu_out, double& qls_out, double& qlab_out );
        double fitness         () const;
        void   update_fitness  ();
        bool   feasible        () const;
        void   update_feasible ();
        void   mutate          (const Solver& solver, int first_task, int after_last_task, bool delay_fitness);

        void   set_fitness_delayed(bool new_value);
        bool   need_update_fitness();

     public:
        double get_Qle();
        double get_Qlu();
        double get_Qls();
        double get_Qlab();

        /**
         * TODO: _____ We probably want to refactor this. True programmer doesn't
         * have any friends.
         */
        friend void cross_one_point     (const Solver& solver, int first_task, int after_last_task, Solution& sol1, Solution& sol2);
        friend void cross_two_points    (const Solver& solver, int first_task, int after_last_task, Solution& sol1, Solution& sol2);
        friend void cross_uniform       (const Solver& solver, int first_task, int after_last_task, Solution& sol1, Solution& sol2);
        friend void cross_fitness_based (const Solver& solver, int first_task, int after_last_task, Solution& sol1, Solution& sol2);
        friend void crossover_pmx       (const Solver& solver, int first_task, int after_last_task, Solution& sol1, Solution& sol2);
        friend void crossover_ox        (const Solver& solver, int first_task, int after_last_task, Solution& sol1, Solution& sol2);
		friend void crossover_cx        (const Solver& solver, int first_task, int after_last_task, Solution& sol1, Solution& sol2);
        friend void mutate_move         (const Solver& solver, int first_task, int after_last_task, Solution& sol);
        friend void mutate_swap         (const Solver& solver, int first_task, int after_last_task, Solution& sol);
        friend void mutate_both         (const Solver& solver, int first_task, int after_last_task, Solution& sol);
        friend void mutate_rebalancing  (const Solver& solver, int first_task, int after_last_task, Solution& sol);
        friend void mutate_swap_hgs 	(const Solver& solver, int first_task, int after_last_task, Solution& sol);

     public:
        const Problem&  problem;
        double          _fitness;
        double          _feasible;

        double          _Qle;
        double          _Qlu;
        double          _Qls;
        double          _Qlab;
        bool            _need_update_fitness;

	 public:
        void heuristic (heuristic_type choice);
		void perturbate_randomly  ();

	 protected:
		void compute_most_loaded_machines       (matrix<int>& sched, vector<int>& machines);
		void transform_schedule_representation  (int first_task, int after_last_task, matrix<int>& sched);
		void transform_schedule_representation  (int first_task, int after_last_task, vector<int>& perm, vector<int>& limits);
		void apply_schedule_representation      (const matrix<int>& sched);
		void apply_schedule_representation      (const vector<int>& perm, const vector<int>& limits);
		int  select_task_from_another_machine   (int machine, const matrix<int>& sched);
		void get_shuffled_tasks                 (vector<int>& tasks);
		void get_shuffled_machines 				(vector<int>& machines);

	 public:
        vector<int>     schedule;
        vector<double>  completion_l;
        double			local_makespan;    /* Makespan of used machines  */
        double          local_avg_utilization;
        double          flowtime;
        double			energy;


        //double        total_makespan;
        //double        total_avg_utilization;
        //***********************************************
        void enviroment(const Solution& best, int env_lenght, enviroment_type choise);
    };

    /**
     * Represents population. TODO: doc
     */
    requires class Population {
     public:
        Population (const Solver& solver);
        Population (const Solver& solver, const int size);
        Population (const Population& pop);
        ~Population();

        Solution& best_solution () const;
        double    best_fitness  () const;
        double    worst_fitness () const;

        void start  ();
        void update ();
        void sort   ();
        int  size   () const;

        void quit_worst (const int n);
        void keep_best  (const int n);

        Population select () const;
        void cross        ();
        void cross		  (int first_task_to_cross, int last_plus_one_task_to_cross, bool delay_fitness_calculation);
        void mutate       ();
        void mutate		  ( int first_task, int after_last_task, bool delay_fitness );
        void replace      (Population& pop);

        double find_minimum( Solution* solution, int player_nr );
        double follower_module();

        void calculate_delayed_fitness();

        Population& operator=  (const Population& pop);
        Population& operator+= (const Population& pop);

        friend ostream& operator<< (ostream& os, const Population& pop);
        friend istream& operator>> (istream& is, Population& pop);
        //****************************************************************
        void start_epoch(const Solution& best_solution);
        void mutate_hgs();
        void mutate_hgs( int first_task, int after_last_task, bool delay_fitness );

     protected:

        const Solver&  _solver;
		const Problem& _problem;
		const Setup&   _setup;
        vector<Solution*> _solutions;


        Solution*         _best_solution;
        double _best_fitness;
        double _worst_fitness;

        friend void start_ljfrsjfr (Population& pop);
        friend void start_mct      (Population& pop);
        friend void start_random   (Population& pop);
        friend void start_all      (Population& pop);
        friend void start_minmin   (Population& pop);
        friend void start_rc       (Population& pop);
        friend void start_kpb      (Population& pop);
        friend void start_all2     (Population& pop);

        friend Population select_random            (const Population& pop);
        friend Population select_best              (const Population& pop);
        friend Population select_linear_ranking    (const Population& pop);
        friend Population select_exp_ranking       (const Population& pop);
        friend Population select_binary_tournament (const Population& pop);
        friend Population select_n_tournament      (const Population& pop);

        //-------------------------------------------------------------------
        friend void select_enviroment_basic(Population& pop, const Solution& best, int env_lenght);
        friend void select_enviroment_min(Population& pop, const Solution& best, int env_lenght);

        friend Population connect_populations(const Population& pop1, const Population& pop2);
        friend class Solver_HGS;
    };

    requires void crossover (const Solver& solver,
    		Solution& sol1, Solution& sol2,
    		int first_task_to_cross, int last_plus_one_task_to_cross,
    		bool delay_fitness_calculation );
    requires bool terminateQ (const Solver& solver);

    /*-------------------PROVIDED CLASSES AND METHODS--------------------*/

//    enum SelectChoice {  SelectRandom, SelectBest, SelectLinearRanking,
//                         SelectExpRanking, SelectBinaryTournament,
//                         SelectNTournament
//                      };
//    enum CrossChoice  {  CrossOnePoint, CrossTwoPoints, CrossUniform,
//    					 CrossFitnessBased, CrossPMX, CrossOX, CrossCX
//    				  };
//    enum MutateChoice {  MutateMove, MutateSwap, MutateBoth,
//    					 MutateRebalancing
//    				  };
//
//    enum StartMethod  { StartLJFRSJFR, StartMCT, StartRandom, StartAll,
//                        StartMinMin, StartRelativeCost, StartKPB, StartAll2
//                      };
//
//    enum FitnessType  { FitnessMakespan, FitnessFlowtime, FitnessBoth
//					  };

    //******************************************************************
    /**
     * Storage class for Solver initial parameters.
     */
    provides class Setup {
     public:
        Setup ();

        friend ostream& operator<< (ostream& os, const Setup& setup);
        friend istream& operator>> (istream& is, Setup& setup);

        int    nb_independent_runs () const;
        int    nb_evolution_steps () const;
        float  max_time_to_spend () const;
        int    nb_repetitions_to_end () const;
        int    population_size () const;
        int    intermediate_population_size () const;
        float  cross_probability () const;
        float  mutate_probability () const;
        int    select_choice () const;
        float  select_extra_parameter () const;
        int    cross_choice () const;
        float  cross_extra_parameter () const;
        int    mutate_choice () const;
        float  mutate_extra_parameter () const;
        bool   replace_only_if_better () const;
        bool   replace_generational () const;
        int    start_choice () const;
        const  string& user_information () const;
        int    energy_case () const;

        void  set_nb_independent_runs (int i);
        void  set_nb_evolution_steps (int i);
        void  set_max_time_to_spend (float f);
        void  set_nb_repetitions_to_end (int i);
        void  set_population_size (int i);
        void  set_intermediate_population_size (int i);
        void  set_cross_probability (float f);
        void  set_mutate_probability (float f);
        void  set_select_choice (int i);
        void  set_select_extra_parameter (float f);
        void  set_cross_choice (int i);
        void  set_cross_extra_parameter (float f);
        void  set_mutate_choice (int i);
        void  set_mutate_extra_parameter (float f);
        void  set_replace_only_if_better (bool b);
        void  set_replace_generational (bool b);
        void  set_start_choice (int i);
        void  set_user_information (const string& s);
        void  set_energy_case (int i);
        //***********************************************************
        void set_enviroment_length(int env_length);
        int enviroment_length() const;
        void set_enviroment_choice(int i);
        int enviroment_choice() const;
        void set_number_followers(int i);
        int number_followers() const;
        void set_leader_factor(float f);
        float leader_factor() const;
        void set_mutation_size(int i);
        int mutation_size() const;
        void set_enable_leader( bool enable_leader );
        bool enable_leader( ) const;

        void set_enable_game( bool b );
        bool enable_game() const;

        void set_min_fun_choice( int i );
        int min_fun_choice() const;

        void set_independent_GA( bool set_independent_GA );
        bool independent_GA( ) const;

        static void set_security_extra_param( double enable_security );
        static double security_extra_param();

        static void set_abort_extra_param( double enable_security );
		static double abort_extra_param();

        static void set_enable_qle( bool enable_qle );
		static bool enable_qle();

		static void set_enable_qlu( bool enable_qlu );
		static bool enable_qlu();

        static void set_enable_qls( bool enable_qls );
        static bool enable_qls();

        static void set_enable_qlab( bool enable_qlab );
        static bool enable_qlab();

        static void set_fitness_type(int fitness_type);
        static int fitness_type();
        //***********************************************************

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
        int     _start_choice;
        string  _user_information;
        //*************************************
        int 	_enviroment_length;
        int 	_enviroment_choice;
        int		_number_followers;
        float	_leader_factor;
        int 	_mutation_size;

        bool    _enable_game;
        int     _min_fun_choice;
        bool    _independent_GA;

        static bool    _enable_qle;
        static bool    _enable_qlu;
        static bool    _enable_qls;
        static bool    _enable_qlab;
        static double  _security_extra_param;
        static double  _abort_extra_param;
        static int _fitness_type;
     public:
        static vector<int>_pc;
    };

    provides class Solver {
     public:
    	Solver();
        Solver (const Problem& pbm, const Setup& setup);
        virtual ~Solver ();

        const Problem& problem () const;
        const Setup&   setup () const;

        // Execution
        virtual void run () =0;
        virtual void perform_one_independent_run () =0;
        virtual void perform_one_phase () =0;
        virtual void set_current_solution (const Solution& sol);

        // Global information
        virtual int       independent_run () const;
        virtual float     time_spent () const;
        virtual Solution  best_solution () const;
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
        virtual int  	  all_iterations() const;
        // Phase information
        virtual int  current_iteration () const;

     protected:

        const Problem& _problem;
        const Setup&   _setup;
        const bool     _minimizing;

        int      _independent_run;
        float    _time_spent;
        Solution _best_solution;
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
        //int 	 _all_iterations;
        Population _parents;
        Population _offsprings;

		#ifdef TRACEAR
			ofstream  _tracefile;
		#endif

        virtual void  update_state (const Solution& sol);

     public:
        int 	 _all_iterations;
    };

    provides class Solver_Seq: public Solver {
     public:
    	Solver_Seq(Solver_Seq& other);
        Solver_Seq (const Problem& pbm, const Setup& setup);
        ~Solver_Seq ();

        // Execution
        void run ();
        void perform_one_independent_run ();
        void perform_one_phase ();

        void perform_independent_run();
        void perform_one_epoch(const Solution& best_solution);
        void set_1st_phase_belongs_independent_run(bool state);
        void prepare_population(const Solution& best_solution);
        void replace_parents(Population& pop);

        Population get_population() const;

        void set_parent_branch_number(int branch_number);
        int parent_branch_number() const;

        /**
		 * Start positions for each player compartment. This vector size is
		 * equal to number of players +1, so we can store end of last
		 * compartment in same vector and not threat it as special case.
		 *
		 * Example, 512 tasks, first 256 for leader, rest for 4 players:
		 *
		 * [ 0,     256,     320,    384,    448,     512 ]
		 */
		vector<int>  _player_compartments;
		int leader_size() const;

     private:

		int phase_counter;

        // Auxiliar attributes
        float       _gtime,_ltime;
        bool        _1st_phase_belongs_independent_run;

        bool		_enable_game_temp;

        int _parent_branch_number;
        void calculate_player_compartments();

    };
    //*********************************************************************
        provides class Solver_HGS
        {
    		public:
    			Solver_HGS(Problem problem, vector<Setup> setup, int evelution_steps=1);
    			~Solver_HGS();

    			void run();
    			void run_branch(int branch_id);
    			int nb_evolution_steps() const;//number of repeating an algorythm
    			void set_evolution_steps(int nb_evolution_steps);

    			Solution best_solution() const;
    			int best_found_metaepoch() const;
    			int best_found_branch() const;
    			int number_branches(int branch_id);

    		protected:
    			void start();
    			void add_branch(const Solution& solution, int branch_nb, int parent_branch);
    			bool compare_branches(int branch_nb, int branch_id);
    			void stick_branches(int branch_id);
    			void connect_branches(int branch_id, int item1, int item2);

    			vector< vector<Solver_Seq*> > _branches;//macierz żeby zbudować drzewo, w kolejnych epokach jest kilka algorytmów przetwarzanych równolegle

    			vector<Setup> _setup;//settings for each branch
    			Problem _problem;

    			Solution _best_solution;
    			int _nb_evolutions_steps;
    			int _actual_evolution_step;
    			float _time_spent;
    			int _best_found_metaepoch;
    			int _best_found_branch;
        };
    //*********************************************************************
        extern bool ENERGY_CASE;
}

#endif
