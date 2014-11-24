/*************************************************************************/
/* FILE: TabuSearch.pro.cc                                               */
/*************************************************************************/
#include "TabuSearch.hh"

#define ELITE_SOLUTIONS_SIZE     30
#define MAX_LOAD_INTERVAL      1.00
#define PERCENT_MIN_LOAD       1.00
#define MAX_SWAPS              plus_infinity
#define MAX_TRANSFERS          plus_infinity

skeleton TabuSearch {
    
    /*********************************************************************/
    /* Setup                                                             */
    /*********************************************************************/
	Setup::Setup ()
	:	_independent_runs(1),
		_nb_iterations(10000),
		_start_choice(AdHocHeuristics::MinMin_h),
		_max_time_to_spend(plus_infinity),
		_use_delta_function(false),
		_tabu_size(918133),
		_min_tabu_status(32),
		_max_tabu_status(32),
		_max_repetitions(64),
		_nb_diversifications(5),
		_nb_intensifications(5),
		_elite_size(ELITE_SOLUTIONS_SIZE),
		_aspiration_value(16),
        _max_nb_swaps(MAX_SWAPS),
		_max_nb_transfers(MAX_TRANSFERS),
		_max_load_interval(MAX_LOAD_INTERVAL),
		_percent_min_load(PERCENT_MIN_LOAD),
		_user_information("")
	{
	}
		
	ostream& operator<< (ostream& os, const Setup& setup) {
	    os<<"------------------GENERAL PARAMETERS------------------"<<endl;
		os<<"Nb of independent runs: "<< setup._independent_runs    <<endl;
		os<<"Nb of iterations: "      << setup._nb_iterations       <<endl;
		os<<"Starting heuristic: "    << setup._start_choice        <<endl;
		os<<"Max time to spend: "     << setup._max_time_to_spend   <<endl;
		os<<"Use delta function?: "   << setup._use_delta_function  <<endl;
		os<<"Tabu size: "             << setup._tabu_size           <<endl;
		os<<"Min tabu status: "       << setup._min_tabu_status     <<endl;
		os<<"Max tabu status"         << setup._max_tabu_status     <<endl;
		os<<"Max current repetitions: "<<setup._max_repetitions     <<endl;
		os<<"Nb of diversifications: " <<setup._nb_diversifications <<endl;
		os<<"Nb of intensifications: "<< setup._nb_intensifications <<endl;
		os<<"------------------SPECIFIC PARAMETERS-----------------"<<endl;
		os<<"Multicriteria (h./s.): "     <<_MULTIOBJECTIVE_        <<endl;
		os<<"Max nbr elite solutions : "  <<setup._elite_size       <<endl;
		os<<"Aspiration value: "          <<setup._aspiration_value <<endl;
		os<<"Max nb of swaps moves: "     <<setup._max_nb_swaps     <<endl;
		os<<"Max nb of transfer moves: "  <<setup._max_nb_transfers <<endl;
		os<<"Most-loaded machines itval: "<<setup._max_load_interval<<endl;
		os<<"%% Less-loaded machines : "  <<setup._percent_min_load <<endl;
		os<<"User information: "          <<setup._user_information <<endl;
		os<<"Energy case: "               <<_ENERGY_CASE_           <<endl;
		os<<"------------------------------------------------------"<<endl;
		
		return os;
	}
		
	istream& operator>> (istream& is, Setup& setup) {
		is >> setup._independent_runs;
		is >> setup._nb_iterations;
		is >> setup._start_choice;
		is >> setup._max_time_to_spend;
		is >> setup._use_delta_function;
		is >> setup._tabu_size;
		is >> setup._min_tabu_status;
		is >> setup._max_tabu_status;
		is >> setup._max_repetitions;
		is >> setup._nb_diversifications;
		is >> setup._nb_intensifications;
		is >> setup._elite_size;
		is >> setup._aspiration_value;
		is >> setup._max_nb_swaps;
		is >> setup._max_nb_transfers;
		is >> setup._max_load_interval;
		is >> setup._percent_min_load;
		is >> _MULTIOBJECTIVE_;
		is >> setup._user_information;
		is >> _ENERGY_CASE_;
		
		return is;
	}
		
    bool Setup::optimization_strategy () const {
        return _MULTIOBJECTIVE_;
    } 	
	
	int Setup::independent_runs () const {
		return _independent_runs;
	}
		
	int Setup::nb_iterations () const {
		return _nb_iterations;
	}
	
	int Setup::start_choice () const {
	    return _start_choice;
	}	
	
	float Setup::max_time_to_spend () const {
		return _max_time_to_spend;
	}	

	bool Setup::use_delta_function () const {
		return _use_delta_function;
	}

	int Setup::tabu_size () const {
		return _tabu_size;
	}
		
	int Setup::min_tabu_status () const {
		return _min_tabu_status;
	}
		
	int Setup::max_tabu_status () const {
		return _max_tabu_status;
	}
	
	int Setup::max_repetitions () const {
		return _max_repetitions;
	}
		
	int Setup::nb_intensifications () const {
		return _nb_intensifications;
	}
		
	int Setup::nb_diversifications () const {
		return _nb_diversifications;
	}

    int Setup::elite_size () const {
        return _elite_size;
    }    
    
    int Setup::aspiration_value () const {
        return _aspiration_value;
    }    				

    float Setup::max_nb_swaps () const {
        return _max_nb_swaps;
    }
    
    float Setup::max_nb_transfers () const {
        return _max_nb_transfers;
    }    

    float Setup::max_load_interval () const {
        return _max_load_interval;
    }
        
    float Setup::percent_min_load () const {
        return _percent_min_load;
    }            
        
	const string& Setup::user_information () const {
		return _user_information;
	}

	int Setup::energy_case () const {
		return _ENERGY_CASE_;
	}
				
    void Setup::set_optimization_strategy (bool b) {
        _MULTIOBJECTIVE_ = b;
    }    
    	
	void Setup::set_independent_runs (int i)	{
		assert(i>=1);
		_independent_runs = i;
	}
		
	void Setup::set_nb_iterations (int i) {
		assert(i>=1);
		_nb_iterations = i;
	}	
		
    void Setup::set_start_choice (int i) {
        _start_choice = i;
    }    
    		
	void Setup::set_max_time_to_spend (float f) {
		assert (f>0);
		_max_time_to_spend = f;		
	}			
		
	void Setup::set_use_delta_function (bool b) {
		_use_delta_function = b;
	}
				
	void Setup::set_tabu_size (int i) {
		assert(i>=1);
		_tabu_size = i;
	}
		
	void Setup::set_min_tabu_status (int i) {
		assert(i>=1);
		_min_tabu_status = i;
	}
		
	void Setup::set_max_tabu_status (int i) {
		assert(i>=1);
		_max_tabu_status = i;
	}
				
	void Setup::set_max_repetitions (int i) {
		assert(i>=1);
		_max_repetitions = i;
	}
		
	void Setup::set_nb_intensifications (int i) {
		assert(i>=0);
		_nb_intensifications = i;
	}
		
	void Setup::set_nb_diversifications (int i) {
		assert(i>=0);
		_nb_diversifications = i;
	}

    void Setup::set_elite_size (int i) {
        assert(i > 0);
        _elite_size = i;
    }
        
    void Setup::set_aspiration_value (int i) {
        assert (i>=0);
        _aspiration_value = i;
    }    				
	
	void Setup::set_max_nb_swaps (float f) {
	    assert (f >= 0);
	    _max_nb_swaps = f;
	}	
	
	void Setup::set_max_nb_transfers (float f) {
	    assert (f >= 0);
	    _max_nb_transfers = f;
	}	
	
	void Setup::set_max_load_interval (float f) {
	    assert ((f>=0) && (f<=1));
	    _max_load_interval = f;
	}
	
	void Setup::set_percent_min_load (float f) {
	    assert ((f>=0) && (f<=1));
	    _percent_min_load = f;
	}	
		
	void Setup::set_user_information (const string& s) {
		_user_information = s;
	}

	void Setup::set_energy_case (int i) {
		_ENERGY_CASE_ = i;
	}

    /*********************************************************************/
    /* Solver                                                            */
    /*********************************************************************/
	Solver::Solver (Problem& pbm, Setup& setup)
	:	_problem(pbm), 
		_setup(setup),
		_minimizing(pbm.direction()==Minimize),
        _tstruct(*this),
		_independent_run(0),
		_time_spent(0.0),		
		_best_solution(pbm),
		_independent_run_best_found(0),
		_iteration_best_found(0),
		_time_best_found(0.0),
		_best_cost(((_minimizing)?(plus_infinity):(minus_infinity))),
		_worst_cost(((_minimizing)?(minus_infinity):(plus_infinity))),
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
		_nb_iterations_performed_in_phase(0),
		_intensification_in_phase(false),
		_soft_diversification_in_phase(false),
		_hard_diversification_in_phase(false)		
	{
		#ifdef _TRACEAR_
    		_tracefile.open ("trace", ios::out);
    	#endif
	}
		
	Solver::~Solver() {
    	#ifdef _TRACEAR_
    		_tracefile.close();
    	#endif		
	}
		
	const Problem& Solver::problem () const {
		return _problem;
	}
		
	Setup& Solver::setup () const {
		return _setup;
	}
	
	void Solver::set_current_solution (const Solution& sol) {
		_current_solution = sol;
		_current_cost = sol.fitness();
		
		update_state();
	}
   
   void Solver::set_current_solution(const Solution& sol,const double cost)		
    {
		_current_solution = sol;
		_current_cost = cost;
		
		update_state();
	}

	int Solver::independent_run () const {
		return _independent_run;
	}
		
	float Solver::time_spent () const {
		return _time_spent;
	}
		
	Solution Solver::best_solution () const {
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
		
	const Solution& Solver::current_best_solution () const {
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
	
	int Solver::nb_iterations_performed_in_phase () const {
		return _nb_iterations_performed_in_phase;
	}
		
	bool Solver::intensification_in_phase () const {
		return _intensification_in_phase;
	}
		
	bool Solver::soft_diversification_in_phase () const {
		return _soft_diversification_in_phase;
	}
		
	bool Solver::hard_diversification_in_phase () const {
		return _hard_diversification_in_phase;
	}
			
	void Solver::update_state () {             
		int sign = ((_minimizing)?(1):(-1));

        if (_current_solution.is_better_than(_current_best_solution) ) {
            _current_best_solution = _current_solution;
            _current_best_cost = _current_cost;
            _current_best_solution_iteration = _current_iteration;
            _current_best_solution_time = _current_time_spent;
        }

        if (_current_solution.is_better_than(_best_solution)) {
            _best_solution = _current_solution;
            _best_cost     = _current_cost;
            _independent_run_best_found = _independent_run;
            _iteration_best_found = _current_iteration;
            _time_best_found = _time_spent;
			#ifdef _TRACEAR_
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
        }

        if ((sign * _current_cost) > (sign * _current_worst_cost))
            _current_worst_cost = _current_cost;

        if ((sign * _current_cost) > (sign * _worst_cost))
            _worst_cost = _current_cost;
    }    
	
    /*********************************************************************/
    /* Solver_Seq                                                        */
    /*********************************************************************/
	Solver_Seq::Solver_Seq (Problem& pbm, Setup& setup)
	:	Solver(pbm,setup),
		_gtime(0.0),
		_ltime(0.0),
		_previous_solution(_current_solution),
		_1st_phase_belongs_independent_run(false),
		_nb_consecutive_repetitions(0)
	{
	}
		
	Solver_Seq::~Solver_Seq ()	{
	}
		
	void Solver_Seq::run () {
		_independent_run = 1;
		_gtime = used_time();
		_time_spent = 0; // _gtime ??
		_best_solution = Solution(_problem);
		_independent_run_best_found = 0;
		_iteration_best_found = 0;
		_best_cost  = ((_minimizing)?(plus_infinity):(minus_infinity));
		_worst_cost = ((_minimizing)?(minus_infinity):(plus_infinity));

		int nb_independent_runs = _setup.independent_runs();
		while (_independent_run <= nb_independent_runs) {
			perform_one_independent_run();
			_independent_run++;
		}
	}
		
	void Solver_Seq::perform_one_independent_run ()	{				
		_1st_phase_belongs_independent_run = true;
		perform_one_phase();				
		while (!terminateQ(*this)) {
			perform_one_phase();			
		}				
	}
		
	void Solver_Seq::perform_one_phase () {
		// Hybridization may run phases out of independent runs
		// so initialization has to be supported

		if (_1st_phase_belongs_independent_run)	{
			// A starting solution is created
			_current_initial_solution.set_initial( _tstruct,
			                                       setup().start_choice());
			_current_initial_cost = _current_initial_solution.fitness();

			_current_solution = _current_initial_solution;
			_current_cost     = _current_initial_cost;		

			// Initialization		
			_current_best_solution = _current_solution;
			_current_best_cost     = _current_cost;
			_current_worst_cost    = _current_cost;
		
			_current_iteration = 0;
			_ltime = used_time();
			_current_time_spent = 0;  // _ltime ??
			_current_best_solution_iteration = 0;
			_current_best_solution_time = _current_time_spent;
			
			_nb_consecutive_repetitions = 0;
			update_state();
			_1st_phase_belongs_independent_run = false;			
		}
		// Phase execution (atomic)
		bool   found = true, improve = true;
		double delta;

		_previous_solution = _current_solution;
		_nb_iterations_performed_in_phase  = 0;
		_intensification_in_phase      = false;
		_soft_diversification_in_phase = false;
		_hard_diversification_in_phase = false;
						
		Movement move(_problem,_current_solution);
			
		if (_nb_consecutive_repetitions >= _setup.max_repetitions())
		{			
			/* INTENSIFICATION, by rewarding the current solution */
			//double cost_before_intensificate = _current_cost;				
			for (int i=0; i<_setup.nb_intensifications(); i++) {
				_current_solution.reward(_tstruct);
				found = choose_best_move ( _problem, _current_solution, 
				                           _tstruct, *this, move );
				_current_solution.unreward(_tstruct);
					
				if (found) {
					delta = _current_solution.delta (move);
					_current_solution.apply (move);
						
					_tstruct.make_tabu (move, _current_solution);
					_tstruct.update ();
						
					if (_setup.use_delta_function()) _current_cost += delta;
					else _current_cost = _current_solution.fitness();
						
					// Update the global and local state						
					_current_iteration++;
					_nb_iterations_performed_in_phase++;
					_current_time_spent += used_time(_ltime);
					_time_spent += used_time(_gtime);						
					update_state();					
				}
			}			
			_intensification_in_phase = true;
				
			improve = _current_solution.is_better_than(_previous_solution);
			//improve = ((_minimizing) ? 
			//			 (_current_cost < cost_before_intensificate) :
			//			 (_current_cost > cost_before_intensificate) );

			/* SOFT DIVERSIFICATION, by penalizing the current solution */
			if (!improve) {
				for (int i = 0; i < _setup.nb_diversifications(); i++) {
					_current_solution.penalize(_tstruct);					
					found = choose_best_move(_problem, _current_solution, 
					                         _tstruct, *this, move);
					_current_solution.unpenalize(_tstruct);
					
					if (found) {
						delta = _current_solution.delta(move);
						_current_solution.apply(move);
						
						_tstruct.make_tabu(move, _current_solution);
						_tstruct.update();
						
						if (_setup.use_delta_function()) {
						    _current_cost += delta;
						} else {
						    _current_cost = _current_solution.fitness();
						}
						
						// Update the global and local state						
						_current_iteration++;
						_nb_iterations_performed_in_phase++;
						_current_time_spent += used_time(_ltime);
						_time_spent += used_time(_gtime);						
						update_state();							
					}
				}		
				_soft_diversification_in_phase = true;
				
				improve=_current_solution.is_better_than(_previous_solution);
                _nb_consecutive_repetitions = 0;
				//improve = ((_minimizing) ? 
				//			 (_current_cost < cost_before_intensificate) :
				//			 (_current_cost > cost_before_intensificate) );				

				/* ESCAPE (STRONG DIVERSIFICATION) */
				if (!improve) { 
					_current_solution.escape(_tstruct);
					_current_cost = _current_solution.fitness();
				
					// Update the global and local state							
					_current_iteration++;
					_nb_iterations_performed_in_phase++;
					_current_time_spent += used_time(_ltime);
					_time_spent += used_time(_gtime);							
					update_state();						
					_hard_diversification_in_phase = true;
				}
			}
		}
		
		/* NORMAL AND ESCAPED EXPLORATION */
		found = choose_best_move(_problem, _current_solution, _tstruct, 
		                         *this, move);					
        _current_iteration++;
		_nb_iterations_performed_in_phase++;
   		if (found) 	{
			delta = _current_solution.delta(move);
			_current_solution.apply(move);
						
			_tstruct.make_tabu(move, _current_solution);
			_tstruct.update();
						
			if (_setup.use_delta_function())  _current_cost += delta;
			else _current_cost = _current_solution.fitness();
						
			// Update the global and local state						
			_current_time_spent += used_time(_ltime);
			_time_spent += used_time(_gtime);		
			update_state();			
		}
        _current_time_spent += used_time(_ltime);
		_time_spent += used_time(_gtime);		
		
		// Check for repetitions
		//if (_current_solution.close_to(_previous_solution)) {
        ////if (_current_solution==_previous_solution) {		    
		//	_nb_consecutive_repetitions++;
		//} else {
		//	_nb_consecutive_repetitions = 0;
		//}
		if (_current_solution == _current_best_solution) {
		    _nb_consecutive_repetitions = 0;
	    } else {
	        ++_nb_consecutive_repetitions;
	    }
	}		
	
    /*-------------------AUXILIAR ATTRIBUTES DECLARATIONS----------------*/
     bool _MULTIOBJECTIVE_; // Stores optimization strategy (hier./simult.)
     bool _ENERGY_CASE_;
    /*-------------------------------------------------------------------*/
}
