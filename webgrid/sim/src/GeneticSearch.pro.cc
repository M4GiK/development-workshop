/*************************************************************************/
/* FILE: GeneticSearch.pro.cc                                            */
/*************************************************************************/
#include "GeneticSearch.hh"
#include "GAParams.hh"
#include <math.h>

bool compare_solutions (const GeneticSearch::Solution* i,
			            const GeneticSearch::Solution* j);

skeleton GeneticSearch {

    /*********************************************************************/
    /* Setup                                                             */
    /*********************************************************************/
    Setup::Setup ()
    :   _nb_independent_runs(1),
        _nb_evolution_steps(2500),
		_max_time_to_spend(plus_infinity),
		_nb_repetitions_to_end(-1),
        _population_size(70),
        _intermediate_population_size(34),
        _cross_probability(0.80),
        _mutate_probability(0.40),
        _select_choice(SelectLinearRanking),
        _select_extra_parameter(0.00),
        _cross_choice(CrossCX),
        _cross_extra_parameter(0.00),
        _mutate_choice(MutateRebalancing),
        _mutate_extra_parameter(0.60),
        _replace_only_if_better(false),
        _replace_generational(false),
        _start_choice(StartAll2),
        _user_information("")
    {
    }

    ostream& operator<< (ostream& os, const Setup& stp) {
	    os << "------------------GENERAL PARAMETERS------------------"        << endl;
		os << "Nb of independent runs: " << stp._nb_independent_runs          << endl;
		os << "Nb of evolution steps: "  << stp._nb_evolution_steps           << endl;
		os << "Max time to spend: "      << stp._max_time_to_spend            << endl;
		os << "Nb repetitions to end: "  << stp._nb_repetitions_to_end        << endl;
		os << "Population size: "        << stp._population_size              << endl;
		os << "Intermediate pop size: "  << stp._intermediate_population_size << endl;
		os << "Cross probability: "      << stp._cross_probability            << endl;
		os << "Mutate probability: "     << stp._mutate_probability           << endl;
		os << "Select choice : "         << stp._select_choice                << endl;
		os << "Select extra parameter: " << stp._select_extra_parameter       << endl;
		os << "Cross choice: "           << stp._cross_choice                 << endl;
		os << "Cross extra parameter: "  << stp._cross_extra_parameter        << endl;
		os << "Mutate choice: "          << stp._mutate_choice                << endl;
		os << "Mutate extra parameter: " << stp._mutate_extra_parameter       << endl;				
		os << "Replace only if better: " << stp._replace_only_if_better       << endl;
		os << "Replace generational: "   << stp._replace_generational         << endl;
		os << "Start choice: "           << stp._start_choice                 << endl;
        os << "------------------SPECIFIC PARAMETERS-----------------"        << endl;
	    os << "Multicriteria (h./s.): "  << MULTIOBJECTIVE                    << endl;
		os << "User information: "       << stp._user_information             << endl;
		os << "Energy Case: "            << ENERGY_CASE                       << endl;

		return os;        
    }

    istream& operator>> (istream& is, Setup& stp) {
        is >> stp._nb_independent_runs;
        is >> stp._nb_evolution_steps;
		is >> stp._max_time_to_spend;
        is >> stp._nb_repetitions_to_end;
        is >> stp._population_size;
        is >> stp._intermediate_population_size;
        is >> stp._cross_probability;
        is >> stp._mutate_probability;
        is >> stp._select_choice;
        is >> stp._select_extra_parameter;
        is >> stp._cross_choice;
        is >> stp._cross_extra_parameter;
        is >> stp._mutate_choice;
        is >> stp._mutate_extra_parameter;
        is >> stp._replace_only_if_better;
        is >> stp._replace_generational;
        is >> stp._start_choice;
        is >> MULTIOBJECTIVE;
        is >> stp._user_information;
        is >> ENERGY_CASE;

        return is;
    }

    bool Setup::optimization_strategy () const {
        return MULTIOBJECTIVE;
    }    

    int Setup::nb_independent_runs () const {
        return _nb_independent_runs;
    }

    int Setup::nb_evolution_steps () const {
        return _nb_evolution_steps;
    }
	
	float Setup::max_time_to_spend () const {
		return _max_time_to_spend;
	}
	
    int Setup::nb_repetitions_to_end () const {
    	return _nb_repetitions_to_end;
    }
    
    int Setup::population_size () const {
        return _population_size;
    }

    int Setup::intermediate_population_size () const {
        return _intermediate_population_size;
    }

    float Setup::cross_probability () const {
        return _cross_probability;
    }

    float Setup::mutate_probability () const {
        return _mutate_probability;
    }

    int Setup::select_choice () const {
        return _select_choice;
    }

    float Setup::select_extra_parameter () const {
        return _select_extra_parameter;
    }

    int Setup::cross_choice () const {
        return _cross_choice;
    }

    float Setup::cross_extra_parameter () const {
        return _cross_extra_parameter;
    }

    int Setup::mutate_choice () const {
        return _mutate_choice;
    }

    float Setup::mutate_extra_parameter () const {
        return _mutate_extra_parameter;
    }

    bool Setup::replace_only_if_better () const {
        return _replace_only_if_better;
    }

    bool Setup::replace_generational () const {
        return _replace_generational;
    }

	int Setup::start_choice () const {
		return _start_choice;
	}	

    const string& Setup::user_information () const {
        return _user_information;
    }

    int Setup::energy_case () const {
    	return ENERGY_CASE;
    }

    void Setup::set_optimization_strategy (bool b) {
        MULTIOBJECTIVE = b;
    }    
    
    void Setup::set_nb_independent_runs (int i) {
        assert(i>=1);
        _nb_independent_runs = i;
    }

    void Setup::set_nb_evolution_steps (int i) {
        assert(i>=1);
        _nb_evolution_steps = i;
    }
	
	void Setup::set_max_time_to_spend (float f) {
		assert(f>=0.0);
		_max_time_to_spend = f;
	}
	
    void Setup::set_nb_repetitions_to_end (int i) {
    	_nb_repetitions_to_end = i;
    }
    
    void Setup::set_population_size (int i) {
        assert(i>=1);
        _population_size = i;
    }

    void Setup::set_intermediate_population_size (int i) {
        assert(i>=1);
        _intermediate_population_size = i;
    }

    void Setup::set_cross_probability (float f) {
        assert((f>=0) && (f<=1));
        _cross_probability = f;
    }

    void Setup::set_mutate_probability (float f) {
        assert((f>=0) && (f<=1));
        _mutate_probability = f;
    }

    void Setup::set_select_choice (int i) {
        _select_choice = i;
    }

    void Setup::set_select_extra_parameter (float f) {
        _select_extra_parameter = f;
    }

    void Setup::set_cross_choice (int i) {
        _cross_choice = i;
    }

    void Setup::set_cross_extra_parameter (float f) {
        _cross_extra_parameter = f;
    }

    void Setup::set_mutate_choice (int i) {
        _mutate_choice = i;
    }

    void Setup::set_mutate_extra_parameter (float f) {
        _mutate_extra_parameter = f;
    }

    void Setup::set_replace_only_if_better (bool b) {
        _replace_only_if_better = b;
    }

    void Setup::set_replace_generational (bool b) {
        _replace_generational = b;
    }

	void Setup::set_start_choice (int i) {
		_start_choice = i;
	}
	
    void Setup::set_user_information (const string& s) {
        _user_information = s;
    }

    void Setup::set_energy_case (int i) {
    	ENERGY_CASE = i;
    }


    /*********************************************************************/
    /* Solver                                                            */
    /*********************************************************************/
    Solver::Solver (const Problem& pbm, const Setup& setup)
    :   _problem(pbm),
        _setup(setup),
        _minimizing(pbm.direction()==Minimize),
        _independent_run(0),
        _time_spent(0.0),
        _best_solution(pbm),
        _independent_run_best_found(0),
        _iteration_best_found(0),
        _time_best_found(0.0),
        _best_cost(((_minimizing)?(plus_infinity):(minus_infinity))),
        _worst_cost(((_minimizing)?(minus_infinity):(plus_infinity))),
		_nb_repetitions_best_solution(0),
        _current_solution(_best_solution),
        _current_cost(_best_cost),
        _current_iteration(0),
        _current_initial_solution(_current_solution),
        _current_initial_cost(_current_cost),
        _current_time_spent(0.0),
        _current_best_solution(_current_solution),
        _current_best_solution_iteration(0),
        _current_best_solution_time(0.0),
        _current_best_cost(_best_cost),
        _current_worst_cost(_worst_cost),
        _parents(*this),
        _offsprings(*this)
    {
    	#ifdef TRACEAR
    		_tracefile.open ("trace", ios::out);
    	#endif
    }

    Solver::~Solver() {
    	#ifdef TRACEAR
    		_tracefile.close();
    	#endif
    }

    const Problem& Solver::problem () const {
        return _problem;
    }

    const Setup& Solver::setup () const {
        return _setup;
    }

    void Solver::set_current_solution (const Solution& sol) {
        _current_solution = sol;
        _current_cost = sol.fitness();

        update_state(sol);
    }

    int Solver::independent_run () const {
        return _independent_run;
    }

    float Solver::time_spent () const {
        return _time_spent;
    }

    Solution Solver::best_solution () const  {
        return _best_solution;
    }

    int Solver::independent_run_best_found () const {
        return _independent_run_best_found;
    }

    int Solver::iteration_best_found () const {
        return _iteration_best_found;
    }

    float Solver::time_best_found () const {
        return _time_best_found;
    }

    double Solver::best_cost () const {
        return _best_cost;
    }

    double Solver::worst_cost () const {
        return _worst_cost;
    }

	int Solver::nb_repetitions_best_solution () const {
		return _nb_repetitions_best_solution;
	}	

    Solution Solver::current_solution () const {
        return _current_solution;
    }

    double Solver::current_cost () const {
        return _current_cost;
    }

    int Solver::current_iteration () const {
        return _current_iteration;
    }

    Solution Solver::current_initial_solution () const {
        return _current_initial_solution;
    }

    double Solver::current_initial_cost () const {
        return _current_initial_cost;
    }

    float Solver::current_time_spent () const {
        return _current_time_spent;
    }

    Solution Solver::current_best_solution () const {
        return _current_best_solution;
    }

    int Solver::current_best_solution_iteration () const {
        return _current_best_solution_iteration;
    }

    float Solver::current_best_solution_time () const {
        return _current_best_solution_time;
    }

    double Solver::current_best_cost () const {
        return _current_best_cost;
    }

    double Solver::current_worst_cost () const {
        return _current_worst_cost;
    }

    void Solver::update_state (const Solution& sol) {             
        _current_solution = sol;
        _current_cost     = sol.fitness();
		int sign = ((_minimizing)?(1):(-1));

        if ( sol.is_better_than(_current_best_solution) ) {
            _current_best_solution = _current_solution;
            _current_best_cost = _current_cost;
            _current_best_solution_iteration = _current_iteration;
            _current_best_solution_time = _current_time_spent;
        }

        if ( sol.is_better_than(_best_solution) ) {        	
            _best_solution = _current_solution;
            _best_cost     = _current_cost;
            _independent_run_best_found = _independent_run;
            _iteration_best_found = _current_iteration;
            _time_best_found = _time_spent;
			#ifdef TRACEAR				
				_tracefile.precision(13);
				_tracefile << _iteration_best_found << " " 
					 	   << _best_solution.local_makespan << " " 
					 	   << _best_solution.local_avg_utilization << " "
					 	   << _best_solution.flowtime << endl;
				cout.precision(13);
				cout	   << _iteration_best_found << " " 
					 	   << _best_solution.local_makespan << " " 
					 	   << _best_solution.local_avg_utilization << " "
					 	   << _best_solution.flowtime << endl;				
			#endif
			_nb_repetitions_best_solution = 0;
        } else _nb_repetitions_best_solution++;

        if ((sign * _current_cost) > (sign * _current_worst_cost))
            _current_worst_cost = _current_cost;

        if ((sign * _current_cost) > (sign * _worst_cost))
            _worst_cost = _current_cost;
    }

    /*********************************************************************/
    /* Solver_Seq                                                        */
    /*********************************************************************/
    Solver_Seq::Solver_Seq (const Problem& pbm, const Setup& setup)
    :   Solver(pbm,setup),
        _gtime(0.0),
        _ltime(0.0),
        _1st_phase_belongs_independent_run(false)
    {
    }

    Solver_Seq::~Solver_Seq () {
    }

     void Solver_Seq::run () {
        _independent_run = 1;
        _gtime = used_time();
        _time_spent = 0;  //�� _gtime ??
        _best_solution = Solution(_problem);
        _independent_run_best_found = 0;
        _iteration_best_found = 0;
        _best_cost  = ((_minimizing)?(plus_infinity):(minus_infinity));
        _worst_cost = ((_minimizing)?(minus_infinity):(plus_infinity));

        int nb_independent_runs = _setup.nb_independent_runs();
        
        while (_independent_run <= nb_independent_runs) {
            perform_one_independent_run();
            _independent_run++;
        }
    }

    void Solver_Seq::perform_one_independent_run () {
        _parents.start ();
        _parents.sort ();
        _parents.update();
        update_state(_parents.best_solution());
        
        _1st_phase_belongs_independent_run = true;
        perform_one_phase();
        while( !terminateQ(*this) ) {
            perform_one_phase();
        }
    }

    void Solver_Seq::perform_one_phase () {
        // Hybridization may run phases out of independent runs
        // so initialization has to be supported

        if (_1st_phase_belongs_independent_run) {
            // A starting solution is created
            _current_initial_solution.set_initial();
            _current_initial_cost = _current_initial_solution.fitness();
            
            _current_solution = _current_initial_solution;
            _current_cost     = _current_initial_cost;
            
            // Initialization
            _current_best_solution = _current_solution;
            _current_best_cost     = _current_cost;
            _current_worst_cost    = _current_cost;

            _current_iteration = 0;
            _ltime = used_time();
            _current_time_spent = 0;   //�� _ltime; ??
            _current_best_solution_iteration = 0;
            _current_best_solution_time = _current_time_spent;

			_nb_repetitions_best_solution = 0;
            update_state(_current_solution);
            _1st_phase_belongs_independent_run = false;
        }

        // Generational loop (phase execution)        
        _offsprings = _parents.select();
        _offsprings.cross();
        update_state(_offsprings.best_solution());
        _offsprings.mutate();
        update_state(_offsprings.best_solution());
        _parents.replace(_offsprings);
        
        // Update the global and local state
        _current_iteration++;
        _current_time_spent += used_time(_ltime);
        _time_spent += used_time(_gtime);        
    }
    
    /*-------------------AUXILIAR ATTRIBUTES DECLARATIONS----------------*/
       bool MULTIOBJECTIVE; // Stores optimization strategy (hier./simult.)
       bool ENERGY_CASE; // Stores energy optimization case (0-disabl./1/2)
    /*-------------------------------------------------------------------*/
}
