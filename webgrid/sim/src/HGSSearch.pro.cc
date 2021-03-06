/*
 * HGSSearch.pro.cc
 *
 *  Created on: 2010-01-14
 *      Author: LukkoL
 */

#include "HGSSearch.hh"
#include "GAParams.hh"
#include <math.h>

bool compare_solutions (const HGSSearch::Solution* i,
			            const HGSSearch::Solution* j);



skeleton HGSSearch {
	bool HGS_MULTIOBJECTIVE;
	bool HGS_ENERGY_CASE;
	bool compare_solutions (const HGSSearch::Solution* i,
				            const HGSSearch::Solution* j);

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
	        _user_information(""),
	        _mutation_size(0)
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
			os << "Mutation length: "<< stp._mutation_size<<endl;
			os << "Select choice : "         << stp._select_choice                << endl;
			os << "Select extra parameter: " << stp._select_extra_parameter       << endl;
			os << "Cross choice: "           << stp._cross_choice                 << endl;
			os << "Cross extra parameter: "  << stp._cross_extra_parameter        << endl;
			os << "Mutate choice: "          << stp._mutate_choice                << endl;
			os << "Mutate extra parameter: " << stp._mutate_extra_parameter       << endl;
			os << "Replace only if better: " << stp._replace_only_if_better       << endl;
			os << "Replace generational: "   << stp._replace_generational         << endl;
			os << "Start choice: "           << stp._start_choice                 << endl;
			os << "Enviroment type: "<<stp._enviroment_choice<<endl;
			os << "Enviroment length: "<<stp._enviroment_length<<endl;
	        os << "------------------SPECIFIC PARAMETERS-----------------"        << endl;
			os << "User information: "       << stp._user_information             << endl;

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
	        is >> stp._mutation_size;
	        is >> stp._select_choice;
	        is >> stp._select_extra_parameter;
	        is >> stp._cross_choice;
	        is >> stp._cross_extra_parameter;
	        is >> stp._mutate_choice;
	        is >> stp._mutate_extra_parameter;
	        is >> stp._replace_only_if_better;
	        is >> stp._replace_generational;
	        is >> stp._start_choice;
	        is >> stp._enviroment_choice;
	        is >> stp._enviroment_length;
	        is >> stp._user_information;

	        return is;
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

	    bool Setup::optimization_strategy () const {
	    	return HGS_MULTIOBJECTIVE;
	    }

	    int Setup::energy_case () const {
	    	return HGS_ENERGY_CASE;
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

	    void Setup::set_optimization_strategy(bool b){
	    	HGS_MULTIOBJECTIVE = b;
	    }

	    void Setup::set_energy_case(int i){
	    	HGS_ENERGY_CASE = i;
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
	        _offsprings(*this),
	        _all_iterations(0)
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

	        if(_setup.mutation_size() == 0)
	        	_offsprings.mutate();
	        else
	        	_offsprings.mutate_hgs();

	        update_state(_offsprings.best_solution());
	        _parents.replace(_offsprings);

	        // Update the global and local state
	        _current_iteration++;
	        _current_time_spent += used_time(_ltime);
	        _time_spent += used_time(_gtime);

	        _all_iterations++;
	    }
	//*****************************************************************************

	    //********************************************************
	    Solver_Seq::Solver_Seq(Solver_Seq& other):Solver(other._problem,other._setup)
	    {
	    	this->_independent_run = other._independent_run;
	    	this->_time_spent = other._time_spent;
	    	this->_best_solution = other._best_solution;
	    	this->_independent_run_best_found = other._independent_run_best_found;
	    	this->_iteration_best_found = other._iteration_best_found;
	    	this->_time_best_found = other._time_best_found;
	    	this->_best_cost = other._best_cost;
	    	this->_worst_cost = other._worst_cost;
	    	this->_nb_repetitions_best_solution = other._nb_repetitions_best_solution;
	    	this->_current_solution = other._current_solution;
	    	this->_current_cost = other._current_cost;
	    	this->_current_iteration = other._current_iteration;
	    	this->_current_initial_solution = other._current_initial_solution;
	    	this->_current_initial_cost = other._current_initial_cost;
	    	this->_current_time_spent = other._current_time_spent;
	    	this->_current_best_solution = other._current_best_solution;
	    	this->_current_best_solution_iteration = other._current_best_solution_iteration;
	    	this->_current_best_solution_time = other._current_best_solution_time;
	    	this->_current_best_cost = other._current_best_cost;
	    	this->_current_worst_cost = other._current_worst_cost;
	    	this->_parents = other._parents;
	    	this->_offsprings = other._offsprings;
	    	this->_gtime = other._gtime;
	    	this->_ltime = other._ltime;
	    	this->_1st_phase_belongs_independent_run = other._1st_phase_belongs_independent_run;
	    	this->_all_iterations = other._all_iterations;
	    }
	    //********************************************************
	    void Solver_Seq::perform_independent_run()
	    {
	    	_independent_run = 1;
	    	_gtime = used_time();
	        _current_iteration = 0;

	    	perform_one_phase();
	    	while( !terminateQ(*this) )
	    	{
	    		perform_one_phase();
	    	}
	    }
	    //********************************************************
	    void Solver_Seq::perform_one_epoch(const Solution& best_solution)
	    {
	    	if(!_all_iterations)
	    	{
	    		_parents.start_epoch(best_solution);
	    		_parents.sort ();
	    		_parents.update();
	    		update_state(_parents.best_solution());

	    		_1st_phase_belongs_independent_run = false;


	    	}
	    	_current_iteration = 0;

	        perform_one_phase();
	        while( !terminateQ(*this) )
	        {
	            perform_one_phase();
	        }
	    }
	    //********************************************************
	    void Solver_Seq::set_1st_phase_belongs_independent_run(bool state)
	    {//żeby pominąc domyślne inicjalizowanie w void Solver_Seq::perform_one_phase()
	    	this->_1st_phase_belongs_independent_run = state;
	    }
	    //********************************************************
	    int Solver::all_iterations() const
	    {
	    	return this->_all_iterations;
	    }
	    //********************************************************
	    void Solver_Seq::prepare_population(const Solution& best_solution)
	    {
	       if(!_1st_phase_belongs_independent_run)
	       {
	    	   _current_initial_solution.set_initial(best_solution, _setup.enviroment_length(),_setup.enviroment_choice());
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
	    }
	    //********************************************************
	    Population Solver_Seq::get_population() const
	    {
	    	return _parents;
	    }
	    //**********************************************************
	    void Solver_Seq::replace_parents(Population& pop)
	    {
	    	_parents.replace(pop);
	    }
	    //***********************************************************
	    void Solver_Seq::set_parent_branch_number(int branch_number)
	    {
	    	this->_parent_branch_number = branch_number;
	    }
	    //***********************************************************
	    int Solver_Seq::parent_branch_number() const
	    {
	    	return _parent_branch_number;
	    }
	    /*********************************************************************/
	    /* Setup for HGS                                                         */
	    /*********************************************************************/

	    //********************************************************************
	    void Setup::set_enviroment_length(int env_length)
	    {
	    	assert(env_length >= 0);
	    	this->_enviroment_length = env_length;
	    }
	    //************************************************************
	    int Setup::enviroment_length() const
	    {
	    	return this->_enviroment_length;
	    }
	    //************************************************************
	    void Setup::set_enviroment_choice(int choice)
	    {
	    	this->_enviroment_choice = choice;
	    }
	    //************************************************************
	    int Setup::enviroment_choice() const
	    {
	    	return this->_enviroment_choice;
	    }
	    //************************************************************
	    void Setup::set_mutation_size(int i)
	    {
	    	this->_mutation_size = i;
	    }
	    //************************************************************
	    int Setup::mutation_size() const
	    {
	    	return this->_mutation_size;
	    }
	//*****************************************************************************
	    /*********************************************************************/
	    /* Solver_HGS                                                         */
	    /*********************************************************************/
	    Solver_HGS::~Solver_HGS()
	    {
	    	//int ilosc_petli = _branches[0][0]->_all_iterations;
	    	for(int i=0;i<(int)_branches.size();i++)
	    		for(int j=0;j<(int)_branches[i].size();j++)
	    		{
	    		//	ilosc_petli += _branches[i][j]->_all_iterations;
	    			delete _branches[i][j];
	    		}
	    	//cout<<"\n\nIlosc petli: "<<ilosc_petli<<endl;
	    }
	    //*******************************************************************
	    Solver_HGS::Solver_HGS(Problem problem, vector<Setup> setup, int evolution_steps):_best_solution(problem),_time_spent(0)
	    {
	    	this->_problem = problem;
	    	this->_setup = setup;
	    	this->_nb_evolutions_steps = evolution_steps;

	    	start();
	    }
	    //*******************************************************
	    void Solver_HGS::run()
	    {
	    	_actual_evolution_step = 0;
	    	_branches[0][0]->run();//root started, 1st time run
	    	_best_solution = _branches[0][0]->best_solution();
	    	_best_found_metaepoch = 0;

	    	if(_setup.size()> 1)
	    		add_branch(_best_solution, 1, 0);

	    	for( ; _actual_evolution_step<this->_nb_evolutions_steps; _actual_evolution_step++)
	    	{
	    		if(_actual_evolution_step)
	    		{
	    			_branches[0][0]->perform_independent_run();//root continuation
	    			Solution tmp_sol = _branches[0][0]->best_solution();
	    			if(tmp_sol.is_better_than(_best_solution))
	    			{
	    				_best_solution = tmp_sol;
	    				_best_found_metaepoch = _actual_evolution_step;
	    			}
	    			if(compare_branches(1,0))
	    				add_branch(_branches[0][0]->best_solution(),1,0);
	    		}
	    		if(_setup.size()>1)
	    			run_branch(1);
	    	}
	    }
	    //*******************************************************
	    void Solver_HGS::run_branch(int branch_id)
	    {
	    	assert(branch_id<=(int)_setup.size());

	    	int size = _branches[branch_id].size();

	    	for(int i=0;i<size;i++)//dla każdej podgałęzi gałęzi wyższej
	    	{
	    		_branches[branch_id][i]->perform_one_epoch(_branches[branch_id-1][0]->best_solution());
	    		Solution tmp_sol = _branches[branch_id][i]->best_solution();
	    		if(tmp_sol.is_better_than(_best_solution))
	    		{
	    			_best_solution = tmp_sol;
	    			_best_found_metaepoch = _actual_evolution_step;
	        	}

	    		if(compare_branches(branch_id+1, i) && (branch_id+1 < (int)_branches.size()) )
	    		{
	    			add_branch(_branches[branch_id][i]->best_solution(),branch_id+1, i);
	    		}
	    	}
	    	stick_branches(branch_id);
	    	if(branch_id < (int)_setup.size()-1)
	    		run_branch(branch_id+1);
	    }
	    //*******************************************************
	    void Solver_HGS::start()//initial function
	    {
	    	assert(_setup.size() > 0);
	    	_branches = vector< vector<Solver_Seq*> >(_setup.size());//ilość metaepok

	    	for(int i=0;i<(int)_setup.size();i++)
	    		_branches[i] = vector<Solver_Seq*>();

	    	_branches[0].push_back( new Solver_Seq(_problem, _setup[0]));//pień ma tylko jeden algorytm

	    }
	    //*******************************************************
	    void Solver_HGS::add_branch(const Solution& solution, int branch_nb, int parent_branch)
	    {
	    	assert(branch_nb < (int)_branches.size());

	    	_branches[branch_nb].push_back(new Solver_Seq(_problem,_setup[branch_nb]));
	    	_branches[branch_nb][_branches[branch_nb].size()-1]->prepare_population(solution);

	    	_branches[branch_nb][_branches[branch_nb].size()-1]->set_parent_branch_number(parent_branch);
	    }
	    //*******************************************************
	    void Solver_HGS::stick_branches(int branch_nb)
	    {
	    	int count;
	    	if(2 > _branches[branch_nb].size())
	    		return;

	    	for(int i=0;i<(int)_branches[branch_nb].size();i++)//porównanie każdy z każdym - na danym poziomie
	    	{
	    		count = 0;
	    		for(int j=i+1;j<(int)_branches[branch_nb].size();j++)
	    		{
	    			Population pop1(_branches[branch_nb][i]->get_population()), pop2(_branches[branch_nb][j]->get_population());
	    			if(_branches[branch_nb][i]->best_solution() == _branches[branch_nb][j]->best_solution())//równość najlepszych osobników jest warunkiem koniecznym, ale nie wystarczającym do sklejania
	    			{//kolejne porównanie każdy z każdym, póki co nie wiem jak sensowniej zoptymalizować...
	    				int a=0,b=0;
	    				pop1.sort();pop2.sort();

	    				while((a < pop1.size() && b < pop2.size()))
	    				{
	    					Solution sol1(*pop1._solutions[a]), sol2(*pop2._solutions[b]);
	    					if(sol1 == sol2)
	    					{
	    						count++;
	    						a++; b++;

	    					}
	    					else if(sol2.is_better_than(sol1))
	    					{
	    						b++;
	    					}
	    					else if(sol1.is_better_than(sol2))
	    					{
	    						a++;
	    					}
	    					else
	    					{
	    						if(sol1.fitness() > sol2.fitness())
	    							a++;
	    						else if(sol1.fitness() > sol2.fitness())
									b++;
	    						else if(sol1.fitness() == sol2.fitness())
	    						{
	    							//count++;
	    							a++;b++;
	    						}
	    					}
	    				}//while(a < pop1.size())

	    				if((double)count/pop1.size() > 0.5)
	    				{
	    					connect_branches(branch_nb, i, j);
	    				}//if((double)count/pop1.size() > 0.5)

	    			}//if(_branches[branch_nb][i]->best_solution() == _branches[branch_nb][j]->best_solution())//równość najlepszych osobników jest warunkiem koniecznym, ale nie wystarczającym do sklejania
	    		}
	    	}
	    }
	    //*******************************************************
	    void Solver_HGS::connect_branches(int branch_id, int item1, int item2)
	    {
	    	Population pop1(_branches[branch_id][item1]->get_population()), pop2(_branches[branch_id][item2]->get_population());
	    	Population new_pop(connect_populations(pop1,pop2));

	    	_branches[branch_id][item1]->replace_parents(new_pop);//dla pierwszego solvera nadpisanie populacji rodzicielskiej

	    	delete _branches[branch_id][item2];//usuwam ten dalej położony solver w wektorze
	    	_branches[branch_id].erase(_branches[branch_id].begin()+item2);
	    }
	    //*******************************************************
	    bool Solver_HGS::compare_branches(int branch_nb, int branch_id)
	    {
	    	if(branch_nb >=  (int)_branches.size() || branch_nb < 0)
	    		return false;

	    	bool result = true;

	    	for(int i=0;i<(int)_branches[branch_nb].size();i++)
	    	{
	    		if(_branches[branch_nb-1][branch_id]->parent_branch_number() != _branches[branch_nb][i]->parent_branch_number())
	    			continue;

	    		if(_branches[branch_nb][i]->best_solution().schedule == _branches[branch_nb-1][branch_id]->best_solution().schedule)
	    		{
	    			result = false;
	    			break;
	    		}
	    	}

	    	return result;
	    }
	    //*******************************************************
		int Solver_HGS::nb_evolution_steps() const
		{
			return this->_nb_evolutions_steps;
		}
		//*******************************************************
		void Solver_HGS::set_evolution_steps(int nb_evolution_steps)
		{
			assert(nb_evolution_steps > 0);
			this->_nb_evolutions_steps = nb_evolution_steps;
		}
		//*******************************************************
		Solution Solver_HGS::best_solution() const
		{
			return _best_solution;
		}
		//*******************************************************
		int Solver_HGS::best_found_metaepoch() const
		{
			return _best_found_metaepoch;
		}
		//*******************************************************
	}
