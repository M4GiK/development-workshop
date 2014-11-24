// FILE: Memetic.pro.cc
#include "Memetic.hh"
#include <time.h>

#define SLAVE_TERMINATE 0
#define SLAVE_LSE       1
#define SLAVE_RECOMBINE 2
#define SLAVE_MUTATE    3

skeleton Memetic {

	// ************************************************************************
	// Setup
	// ************************************************************************
	Setup::Setup ()
	  :	_nb_independent_runs(10),
	  _nb_generations(0),
	  _max_time_to_spend(plus_infinity),
	  _population_size(26),
	  _max_repetitions_to_diversify(13),
	  _nb_solutions_to_recombine(2),
	  _nb_recombinations(13),
	  _nb_mutations(13),
	  _start_choice(StartAll),
	  _add_only_if_better(false),
      _select_choice(SelectRandom),
      _neighborhood_pattern(1),
	  _recombination_order(FixedLineSweep),
	  _mutation_order(FixedLineSweep),
	  _recombine_choice(RecombineOnePoint),
	  _select_extra_parameter(0),
	  _cross_choice(RecombineOnePoint),
	  _recombine_selection(SelectRandom),
	  _rec_selection_extra(0),
	  _mutate_choice(MutateMove),
	  _mutate_extra_parameter(0),
	  _mutate_selection(SelectRandom),
	  _mut_selection_extra(0),
	  _local_search_choice(0),
	  _nb_local_search_iterations(0),
	  _nb_ls_not_improving_iterations(plus_infinity),
	  _lsearch_extra_parameter(0),
	  _slave_local_search(vector<LS_config>(0))

	{
	}
	
	ostream& operator<< (ostream& os, const Setup& setup)
	{
		os << setup._nb_independent_runs << endl;
		os << setup._nb_generations << endl;
		os << setup._max_time_to_spend << endl;
		os << setup._population_size << endl;
		os << setup._max_repetitions_to_diversify << endl;
		os << setup._nb_solutions_to_recombine << endl;
		os << setup._nb_recombinations << endl;
		os << setup._nb_mutations << endl;
  		os << setup._start_choice << endl;
		os << setup._add_only_if_better << endl;
		os << setup._select_choice << endl;
		os << setup._select_extra_parameter << endl;
		os << setup._cross_choice << endl;		
		os << setup._recombine_selection << endl;		
		os << setup._rec_selection_extra << endl;		
		os << setup._mutate_choice << endl;
		os << setup._mutate_extra_parameter << endl;
		os << setup._mutate_selection << endl;		
		os << setup._mut_selection_extra << endl;		
		os << setup._local_search_choice << endl;
		os << setup._nb_local_search_iterations << endl;
		os << setup._nb_ls_not_improving_iterations << endl;
		os << setup._lsearch_extra_parameter << endl;
		os << ENERGY_CASE << endl;

		return os;
	}
	
	istream& operator>> (istream& is, Setup& setup)
	{
		is >> setup._nb_independent_runs;
		is >> setup._nb_generations;
		is >> setup._max_time_to_spend;
		is >> setup._population_size;
		is >> setup._max_repetitions_to_diversify;
		is >> setup._nb_solutions_to_recombine;
		is >> setup._nb_recombinations;
		is >> setup._nb_mutations;
		is >> setup._start_choice;
		is >> setup._add_only_if_better;
		is >> setup._select_choice;
		is >> setup._select_extra_parameter;
		is >> setup._cross_choice;
		is >> setup._recombine_selection;		
		is >> setup._rec_selection_extra;		
		is >> setup._mutate_choice;
		is >> setup._mutate_extra_parameter;
		is >> setup._mutate_selection;		
		is >> setup._mut_selection_extra;		
		is >> setup._local_search_choice;
		is >> setup._nb_local_search_iterations;
		is >> setup._nb_ls_not_improving_iterations;
		is >> setup._lsearch_extra_parameter;
		is >> ENERGY_CASE;

		return is;
	}

	opacket2& operator<< (opacket2& op, const Setup& setup)
	{
		op << setup._nb_independent_runs;
		op << setup._nb_generations;
		op << setup._max_time_to_spend;
		op << setup._population_size;
		op << setup._max_repetitions_to_diversify;
		op << setup._nb_solutions_to_recombine;
		op << setup._nb_recombinations;
		op << setup._nb_mutations;
  		op << setup._start_choice;
		op << setup._add_only_if_better;
		op << setup._select_choice;
		op << setup._select_extra_parameter;
		op << setup._cross_choice;		
		op << setup._recombine_selection;		
		op << setup._rec_selection_extra;		
		op << setup._mutate_choice;
		op << setup._mutate_extra_parameter;
		op << setup._mutate_selection;		
		op << setup._mut_selection_extra;		
		op << setup._local_search_choice;
		op << setup._nb_local_search_iterations;
		op << setup._nb_ls_not_improving_iterations;
		op << setup._lsearch_extra_parameter;
		op << (int) setup._slave_local_search.size();
		for (unsigned int i=0; i<setup._slave_local_search.size(); i++)
		{
			op << setup._slave_local_search[i].lsearch_id;
			op << setup._slave_local_search[i].nb_lsearch_iterations;
			op << setup._slave_local_search[i].nb_ls_not_improving_it;
			op << setup._slave_local_search[i].lsearch_extra_parameter;
		}
		return op;
	}
    
	ipacket2& operator>> (ipacket2& ip, Setup& setup)
	{
		ip >> setup._nb_independent_runs;
		ip >> setup._nb_generations;
		ip >> setup._max_time_to_spend;
		ip >> setup._population_size;
		ip >> setup._max_repetitions_to_diversify;
		ip >> setup._nb_solutions_to_recombine;
		ip >> setup._nb_recombinations;
		ip >> setup._nb_mutations;
  		ip >> setup._start_choice;
		ip >> setup._add_only_if_better;
		ip >> setup._select_choice;
		ip >> setup._select_extra_parameter;
		ip >> setup._cross_choice;		
		ip >> setup._recombine_selection;		
		ip >> setup._rec_selection_extra;		
		ip >> setup._mutate_choice;
		ip >> setup._mutate_extra_parameter;
		ip >> setup._mutate_selection;		
		ip >> setup._mut_selection_extra;		
		ip >> setup._local_search_choice;
		ip >> setup._nb_local_search_iterations;
		ip >> setup._nb_ls_not_improving_iterations;
		ip >> setup._lsearch_extra_parameter;

		int ls_size;
		ip >> ls_size;
		setup._slave_local_search.resize(ls_size);

		for (unsigned int i=0; i<setup._slave_local_search.size(); i++)
		{
			ip >> setup._slave_local_search[i].lsearch_id;
			ip >> setup._slave_local_search[i].nb_lsearch_iterations;
			ip >> setup._slave_local_search[i].nb_ls_not_improving_it;
			ip >> setup._slave_local_search[i].lsearch_extra_parameter;
		}
		return ip;
	}
	
	int Setup::nb_independent_runs () const
	{
		return _nb_independent_runs;
	}
	
	
	int Setup::nb_generations () const
	{
		return _nb_generations;
	}

	float Setup::max_time_to_spend () const
	{
		return _max_time_to_spend;
	}
	
	int Setup::population_size () const
	{
		return _population_size;
	}
	

	int Setup::max_repetitions_to_diversify () const
	{
		return _max_repetitions_to_diversify;
	}


	int Setup::nb_solutions_to_recombine () const
	{
		return _nb_solutions_to_recombine;
	}
	
	
	int Setup::nb_recombinations () const
	{
		return _nb_recombinations;
	}
	
	
	int Setup::nb_mutations () const
	{
		return _nb_mutations;
	}

	int Setup::start_choice () const
	{
		return _start_choice;
	}

	bool Setup::add_only_if_better () const
	{
	        return _add_only_if_better;
	}

	int Setup::select_choice () const
	{
	        return _select_choice;
	}

	float Setup::select_extra_parameter () const
	{
	        return _select_extra_parameter;
	}

	int Setup::cross_choice () const
	{
	        return _cross_choice;
	}

    int Setup::recombine_selection () const
	{
			return _recombine_selection;
	}

    float Setup::rec_selection_extra () const
	{
			return _rec_selection_extra;
	}
	
	int Setup::mutate_choice () const
	{
	        return _mutate_choice;
	}

	float Setup::mutate_extra_parameter () const
	{
	        return _mutate_extra_parameter;
	}

	int Setup::mutate_selection () const
	{
			return _mutate_selection;
	}

    float Setup::mut_selection_extra () const
	{
			return _mut_selection_extra;
	}


	int Setup::local_search_choice() const
	{
	        return _local_search_choice;
	}

	int Setup::nb_local_search_iterations() const
	{
	  return _nb_local_search_iterations;
	}
	 
	double Setup::nb_ls_not_improving_iterations() const
	{
	  return  _nb_ls_not_improving_iterations;
	}

	float Setup::lsearch_extra_parameter() const
	{
	  return _lsearch_extra_parameter;
	}

	vector<LS_config> Setup::slave_local_search() const
	{
		return _slave_local_search;
	}
	
	ofstream* Setup::trace() const 
	{
	  return _trace;
	}

	int Setup::energy_case() const
	{
		return ENERGY_CASE;
	}

	void Setup::set_nb_independent_runs (int i)
	{
		assert(i>=1);
		_nb_independent_runs = i;
	}
	
	
	void Setup::set_nb_generations (int i)
	{
		assert(i>=1);
		_nb_generations = i;
	}

	void Setup::set_max_time_to_spend (float f) 
	{
		assert(f>=0.0);
		_max_time_to_spend = f;
	}

        void Setup::set_population_height (int i)
	{
		assert(i>=1);
		_population_height = i;
	}

	void Setup::set_population_width (int i)
	{
		assert(i>=1);
		_population_width = i;
	}
		
	void Setup::set_population_size (int i) 
	{
		assert(i>=1);
		_population_size = i;
	}


	void Setup::set_max_repetitions_to_diversify (int i)
	{
		assert(i>=1);
		_max_repetitions_to_diversify = i;
	}
	
	
	void Setup::set_nb_solutions_to_recombine (int i)
	{
		assert(i>=0);
		_nb_solutions_to_recombine = i;
	}
	
	
	void Setup::set_nb_recombinations (int i)
	{
		assert(i>=0);
		_nb_recombinations = i;
	}
	
	
	void Setup::set_nb_mutations (int i)
	{
		assert(i>=0);
		_nb_mutations = i;
	}

	void Setup::set_start_choice (int i)
	{	
		_start_choice = i;
	}

	void Setup::set_add_only_if_better (bool b)
	{
		_add_only_if_better = b;
	}

        void Setup::set_neighborhood_pattern(int i)
	{
		_neighborhood_pattern = i;
	}

	void Setup::set_recombination_order (int i)
	{
		_recombination_order = i;
	}

	void Setup::set_mutation_order (int i)
	{
		_mutation_order = i;
	}

	void Setup::set_recombine_choice (int i)
	{
		 _recombine_choice = i;
	}

	void Setup::set_select_choice (int i)
	{	             
		 _select_choice = i;
	}

	void Setup::set_select_extra_parameter (float f)
	{
		_select_extra_parameter = f;
	}
	
	void Setup::set_cross_choice (int i)
	{	             
		 _cross_choice = i;
	}
	
	void Setup::set_recombine_selection (int i)
	{
		_recombine_selection = i;
	}

    void Setup::set_rec_selection_extra (float f)
	{
		_rec_selection_extra = f;
	}

	void Setup::set_mutate_choice (int i)
	{	          
		 _mutate_choice = i;
	}

	void Setup::set_mutate_extra_parameter (float f)
	{
		_mutate_extra_parameter = f;
	}

	void Setup::set_mutate_selection (int i)
	{
		_mutate_selection = i;
	}

    void Setup::set_mut_selection_extra (float f)
	{
		_mut_selection_extra = f;
	}

	void Setup::set_local_search_choice(int i)
	{
		_local_search_choice = i;
	}

	void Setup::set_nb_local_search_iterations(int i)
	{
		_nb_local_search_iterations = i;
	}
	 
	void Setup::set_nb_ls_not_improving_iterations(double d)
	{
		_nb_ls_not_improving_iterations = d;
	}
	
	void Setup::set_lsearch_extra_parameter (float f)
	{
		_lsearch_extra_parameter = f;
	}

	void Setup::set_slave_local_search(const vector<LS_config>& v)
	{
		_slave_local_search = v;
	}	

	void Setup::set_trace (ofstream* of)
	{
		_trace = of;
	}

	void Setup::set_energy_case (int i)
	{
		ENERGY_CASE = i;
	}


	/*********************************************************************/
    /* Setup_IR                                                          */
    /*********************************************************************/
	Setup_IR::Setup_IR ()
		: Setup()
	{
		/*Low limits*/
		_population_size_param.lim_inf = _population_size; 
		_max_repetitions_to_diversify_param.lim_inf = _max_repetitions_to_diversify;
		_nb_solutions_to_recombine_param.lim_inf = _nb_solutions_to_recombine;
		_nb_recombinations_param.lim_inf = _nb_recombinations;
		_nb_mutations_param.lim_inf = _nb_mutations;
		_start_choice_param.lim_inf = _start_choice;
		_select_choice_param.lim_inf = _select_choice;
		_select_extra_parameter_param.lim_inf = _select_extra_parameter;
		_cross_choice_param.lim_inf = _cross_choice;
		_recombine_selection_param.lim_inf = _recombine_selection;
		_rec_selection_extra_param.lim_inf = _rec_selection_extra;
		_mutate_choice_param.lim_inf = _mutate_choice;
		_mutate_extra_parameter_param.lim_inf = _mutate_extra_parameter;
		_mutate_selection_param.lim_inf = _mutate_selection;
		_mut_selection_extra_param.lim_inf = _mut_selection_extra;
		_local_search_choice_param.lim_inf = _local_search_choice;
		_nb_local_search_iterations_param.lim_inf = _nb_local_search_iterations;
		_nb_ls_not_improving_iterations_param.lim_inf = _nb_ls_not_improving_iterations;
		_lsearch_extra_parameter_param.lim_inf = _lsearch_extra_parameter;

		/*High limits*/
		_population_size_param.lim_sup = _population_size; 
		_max_repetitions_to_diversify_param.lim_sup = _max_repetitions_to_diversify;
		_nb_solutions_to_recombine_param.lim_sup = _nb_solutions_to_recombine;
		_nb_recombinations_param.lim_sup = _nb_recombinations;
		_nb_mutations_param.lim_sup = _nb_mutations;
		_start_choice_param.lim_sup = _start_choice;
		_select_choice_param.lim_sup = _select_choice;
		_select_extra_parameter_param.lim_sup = _select_extra_parameter;
		_cross_choice_param.lim_sup = _cross_choice;
		_recombine_selection_param.lim_sup = _recombine_selection;
		_rec_selection_extra_param.lim_sup = _rec_selection_extra;
		_mutate_choice_param.lim_sup = _mutate_choice;
		_mutate_extra_parameter_param.lim_sup = _mutate_extra_parameter;
		_mutate_selection_param.lim_sup = _mutate_selection;
		_mut_selection_extra_param.lim_sup = _mut_selection_extra;
		_local_search_choice_param.lim_sup = _local_search_choice;
		_nb_local_search_iterations_param.lim_sup = _nb_local_search_iterations;
		_nb_ls_not_improving_iterations_param.lim_sup = _nb_ls_not_improving_iterations;
		_lsearch_extra_parameter_param.lim_sup = _lsearch_extra_parameter;
		
		/*Parameters distributions*/
		_population_size_param.distr = 'c';
		_max_repetitions_to_diversify_param.distr = 'c';
		_nb_solutions_to_recombine_param.distr = 'c';
		_nb_recombinations_param.distr = 'c';
		_nb_mutations_param.distr = 'c';
		_start_choice_param.distr = 'c';
		_select_choice_param.distr = 'c';
		_select_extra_parameter_param.distr = 'c';
		_cross_choice_param.distr = 'c';
		_recombine_selection_param.distr = 'c';
		_rec_selection_extra_param.distr = 'c';
		_mutate_choice_param.distr = 'c';
		_mutate_extra_parameter_param.distr = 'c';
		_mutate_selection_param.distr = 'c';
		_mut_selection_extra_param.distr = 'c';
		_local_search_choice_param.distr = 'c';
		_nb_local_search_iterations_param.distr = 'c';
		_nb_ls_not_improving_iterations_param.distr = 'c';
		_lsearch_extra_parameter_param.distr = 'c';

		/*boolean parameters probabilities*/
		_add_only_if_better_probl = (_add_only_if_better)?(1.0):(0.0);
	}

	void Setup_IR::set_population_size (int i)
	{
		((Setup) *this).set_population_size(i);
		_population_size_param.lim_inf = i;
		_population_size_param.lim_sup = i;
        _population_size_param.distr = 'c';
	}

	void Setup_IR::set_max_repetitions_to_diversify (int i)
	{
		((Setup) *this).set_max_repetitions_to_diversify(i);
		_max_repetitions_to_diversify_param.lim_inf = i;
		_max_repetitions_to_diversify_param.lim_sup = i;
        _max_repetitions_to_diversify_param.distr = 'c';
	}

	void Setup_IR::set_nb_solutions_to_recombine (int i)
	{
		((Setup) *this).set_nb_solutions_to_recombine(i);
		_nb_solutions_to_recombine_param.lim_inf = i;
		_nb_solutions_to_recombine_param.lim_sup = i;
        _nb_solutions_to_recombine_param.distr = 'c';
	}

	void Setup_IR::set_nb_recombinations (int i)
	{
		((Setup) *this).set_nb_recombinations(i);
		_nb_recombinations_param.lim_inf = i;
		_nb_recombinations_param.lim_sup = i;
        _nb_recombinations_param.distr = 'c';
	}
	
	void Setup_IR::set_nb_mutations (int i)
	{
		((Setup) *this).set_nb_mutations(i);
		_nb_mutations_param.lim_inf = i;
		_nb_mutations_param.lim_sup = i;
        _nb_mutations_param.distr = 'c';
	}

	void Setup_IR::set_start_choice (int i)
	{
		((Setup) *this).set_start_choice(i);
		_start_choice_param.lim_inf = i;
		_start_choice_param.lim_sup = i;
        _start_choice_param.distr = 'c';
	}

	void Setup_IR::set_add_only_if_better (bool b)
	{
		((Setup) *this).set_add_only_if_better(b);
		_add_only_if_better_probl = (b)?(1.0):(0.0);
	}

	void Setup_IR::set_select_choice (int i)
	{
		((Setup) *this).set_select_choice(i);
		_select_choice_param.lim_inf = i;
		_select_choice_param.lim_sup = i;
        _select_choice_param.distr = 'c';
	}

	void Setup_IR::set_select_extra_parameter (float f)
	{
		((Setup) *this).set_select_extra_parameter(f);
		_select_extra_parameter_param.lim_inf = f;
		_select_extra_parameter_param.lim_sup = f;
        _select_extra_parameter_param.distr = 'c';
	}

	void Setup_IR::set_cross_choice (int i)
	{
		((Setup) *this).set_cross_choice(i);
		_cross_choice_param.lim_inf = i;
		_cross_choice_param.lim_sup = i;
        _cross_choice_param.distr = 'c';
	}

	void Setup_IR::set_recombine_selection (int i)
	{
		((Setup) *this).set_recombine_selection(i);
		_recombine_selection_param.lim_inf = i;
		_recombine_selection_param.lim_sup = i;
        _recombine_selection_param.distr = 'c';
	}

	void Setup_IR::set_rec_selection_extra (float f)
	{
		((Setup) *this).set_rec_selection_extra(f);
		_rec_selection_extra_param.lim_inf = f;
		_rec_selection_extra_param.lim_sup = f;
        _rec_selection_extra_param.distr = 'c';
	}

	void Setup_IR::set_mutate_choice (int i)
	{
		((Setup) *this).set_mutate_choice(i);
		_mutate_choice_param.lim_inf = i;
		_mutate_choice_param.lim_sup = i;
        _mutate_choice_param.distr = 'c';
	}

	void Setup_IR::set_mutate_extra_parameter (float f)
	{
		((Setup) *this).set_mutate_extra_parameter(f);
		_mutate_extra_parameter_param.lim_inf = f;
		_mutate_extra_parameter_param.lim_sup = f;
        _mutate_extra_parameter_param.distr = 'c';
	}

	void Setup_IR::set_mutate_selection (int i)
	{
		((Setup) *this).set_mutate_selection(i);
		_mutate_selection_param.lim_inf = i;
		_mutate_selection_param.lim_sup = i;
        _mutate_selection_param.distr = 'c';
	}

	void Setup_IR::set_mut_selection_extra (float f)
	{
		((Setup) *this).set_mut_selection_extra(f);
		_mut_selection_extra_param.lim_inf = f;
		_mut_selection_extra_param.lim_sup = f;
        _mut_selection_extra_param.distr = 'c';
	}

	void Setup_IR::set_local_search_choice (int i)
	{
		((Setup) *this).set_local_search_choice(i);
		_local_search_choice_param.lim_inf = i;
		_local_search_choice_param.lim_sup = i;
        _local_search_choice_param.distr = 'c';
	}

	void Setup_IR::set_nb_local_search_iterations (int i)
	{
		((Setup) *this).set_nb_local_search_iterations(i);
		_nb_local_search_iterations_param.lim_inf = i;
		_nb_local_search_iterations_param.lim_sup = i;
        _nb_local_search_iterations_param.distr = 'c';
	}

	void Setup_IR::set_nb_ls_not_improving_iterations (double d)
	{
		((Setup) *this).set_nb_ls_not_improving_iterations(d);
		_nb_ls_not_improving_iterations_param.lim_inf = d;
		_nb_ls_not_improving_iterations_param.lim_sup = d;
        _nb_ls_not_improving_iterations_param.distr = 'c';
	}

	void Setup_IR::set_lsearch_extra_parameter (float f)
	{
		((Setup) *this).set_lsearch_extra_parameter(f);
		_lsearch_extra_parameter_param.lim_inf = f;
		_lsearch_extra_parameter_param.lim_sup = f;
        _lsearch_extra_parameter_param.distr = 'c';
	}

	void Setup_IR::set_population_size (int lim_inf, int lim_sup, char distr)
	{
		_population_size = (lim_sup + lim_inf)/2;
		_population_size_param.lim_inf = lim_inf;
		_population_size_param.lim_sup = lim_sup;
        _population_size_param.distr = distr;
	}

	void Setup_IR::set_max_repetitions_to_diversify (int lim_inf, int lim_sup, char distr)
	{
		_max_repetitions_to_diversify = (lim_sup + lim_inf)/2;
		_max_repetitions_to_diversify_param.lim_inf = lim_inf;
		_max_repetitions_to_diversify_param.lim_sup = lim_sup;
        _max_repetitions_to_diversify_param.distr = distr;
	}

	void Setup_IR::set_nb_solutions_to_recombine (int lim_inf, int lim_sup, char distr)
	{
		_nb_solutions_to_recombine = (lim_sup + lim_inf)/2;
		_nb_solutions_to_recombine_param.lim_inf = lim_inf;
		_nb_solutions_to_recombine_param.lim_sup = lim_sup;
        _nb_solutions_to_recombine_param.distr = distr;
	}

	void Setup_IR::set_nb_recombinations (int lim_inf, int lim_sup, char distr)
	{
		_nb_recombinations = (lim_sup + lim_inf)/2;
		_nb_recombinations_param.lim_inf = lim_inf;
		_nb_recombinations_param.lim_sup = lim_sup;
        _nb_recombinations_param.distr = distr;
	}
	
	void Setup_IR::set_nb_mutations (int lim_inf, int lim_sup, char distr)
	{
		_nb_mutations = (lim_sup + lim_inf)/2;
		_nb_mutations_param.lim_inf = lim_inf;
		_nb_mutations_param.lim_sup = lim_sup;
        _nb_mutations_param.distr = distr;
	}

	void Setup_IR::set_start_choice (int lim_inf, int lim_sup, char distr)
	{
		_start_choice = (lim_sup + lim_inf)/2;
		_start_choice_param.lim_inf = lim_inf;
		_start_choice_param.lim_sup = lim_sup;
        _start_choice_param.distr = distr;
	}

	void Setup_IR::set_add_only_if_better (double p)
	{
		_add_only_if_better_probl = p;
	}

	void Setup_IR::set_select_choice (int lim_inf, int lim_sup, char distr)
	{
		_select_choice = (lim_sup + lim_inf)/2;
		_select_choice_param.lim_inf = lim_inf;
		_select_choice_param.lim_sup = lim_sup;
        _select_choice_param.distr = distr;
	}

	void Setup_IR::set_select_extra_parameter (float lim_inf, float lim_sup, char distr)
	{
		_select_extra_parameter = (lim_sup + lim_inf)/2;
		_select_extra_parameter_param.lim_inf = lim_inf;
		_select_extra_parameter_param.lim_sup = lim_sup;
        _select_extra_parameter_param.distr = distr;
	}

	void Setup_IR::set_cross_choice (int lim_inf, int lim_sup, char distr)
	{
		_cross_choice = (lim_sup + lim_inf)/2;
		_cross_choice_param.lim_inf = lim_inf;
		_cross_choice_param.lim_sup = lim_sup;
        _cross_choice_param.distr = distr;
	}

	void Setup_IR::set_recombine_selection (int lim_inf, int lim_sup, char distr)
	{
		_recombine_selection = (lim_sup + lim_inf)/2;
		_recombine_selection_param.lim_inf = lim_inf;
		_recombine_selection_param.lim_sup = lim_sup;
        _recombine_selection_param.distr = distr;
	}

	void Setup_IR::set_rec_selection_extra (float lim_inf, float lim_sup, char distr)
	{
		_rec_selection_extra = (lim_sup + lim_inf)/2;
		_rec_selection_extra_param.lim_inf = lim_inf;
		_rec_selection_extra_param.lim_sup = lim_sup;
        _rec_selection_extra_param.distr = distr;
	}

	void Setup_IR::set_mutate_choice (int lim_inf, int lim_sup, char distr)
	{
		_mutate_choice = (lim_sup + lim_inf)/2;
		_mutate_choice_param.lim_inf = lim_inf;
		_mutate_choice_param.lim_sup = lim_sup;
        _mutate_choice_param.distr = distr;
	}

	void Setup_IR::set_mutate_extra_parameter (float lim_inf, float lim_sup, char distr)
	{
		_mutate_extra_parameter = (lim_sup + lim_inf)/2;
		_mutate_extra_parameter_param.lim_inf = lim_inf;
		_mutate_extra_parameter_param.lim_sup = lim_sup;
        _mutate_extra_parameter_param.distr = distr;
	}

	void Setup_IR::set_mutate_selection (int lim_inf, int lim_sup, char distr)
	{
		_mutate_selection = (lim_sup + lim_inf)/2;
		_mutate_selection_param.lim_inf = lim_inf;
		_mutate_selection_param.lim_sup = lim_sup;
        _mutate_selection_param.distr = distr;
	}

	void Setup_IR::set_mut_selection_extra (float lim_inf, float lim_sup, char distr)
	{
		_mut_selection_extra = (lim_sup + lim_inf)/2;
		_mut_selection_extra_param.lim_inf = lim_inf;
		_mut_selection_extra_param.lim_sup = lim_sup;
        _mut_selection_extra_param.distr = distr;
	}

	void Setup_IR::set_local_search_choice (int lim_inf, int lim_sup, char distr)
	{
		_local_search_choice = (lim_sup + lim_inf)/2;
		_local_search_choice_param.lim_inf = lim_inf;
		_local_search_choice_param.lim_sup = lim_sup;
        _local_search_choice_param.distr = distr;
	}

	void Setup_IR::set_nb_local_search_iterations (int lim_inf, int lim_sup, char distr)
	{
		_nb_local_search_iterations = (lim_sup + lim_inf)/2;
		_nb_local_search_iterations_param.lim_inf = lim_inf;
		_nb_local_search_iterations_param.lim_sup = lim_sup;
        _nb_local_search_iterations_param.distr = distr;
	}

	void Setup_IR::set_nb_ls_not_improving_iterations (double lim_inf, double lim_sup, char distr)
	{
		_nb_ls_not_improving_iterations = (lim_sup + lim_inf)/2;
		_nb_ls_not_improving_iterations_param.lim_inf = lim_inf;
		_nb_ls_not_improving_iterations_param.lim_sup = lim_sup;
        _nb_ls_not_improving_iterations_param.distr = distr;
	}

	void Setup_IR::set_lsearch_extra_parameter (float lim_inf, float lim_sup, char distr)
	{
		_lsearch_extra_parameter = (lim_sup + lim_inf)/2;
		_lsearch_extra_parameter_param.lim_inf = lim_inf;
		_lsearch_extra_parameter_param.lim_sup = lim_sup;
        _lsearch_extra_parameter_param.distr = distr;
	}

	void Setup_IR::fill_setup(Setup& setup, int n) const
	{
		int i_val;
		double d_val;
		setup.set_nb_independent_runs (_nb_independent_runs);
		setup.set_nb_generations (_nb_generations);
		setup.set_max_time_to_spend (_max_time_to_spend);
		
		setup.set_population_size (generate_value(_population_size_param.lim_inf, 
							_population_size_param.lim_sup, 
							_population_size_param.distr, n));

		setup.set_max_repetitions_to_diversify (generate_value(_max_repetitions_to_diversify_param.lim_inf, 
							_max_repetitions_to_diversify_param.lim_sup, 
							_max_repetitions_to_diversify_param.distr, n));
		
		setup.set_nb_solutions_to_recombine (generate_value(_nb_solutions_to_recombine_param.lim_inf, 
							_nb_solutions_to_recombine_param.lim_sup, 
							_nb_solutions_to_recombine_param.distr, n));

		/*_nb_recombinations refers now to a proportion(%) of _population_size*/
		i_val = (int) ((generate_value(_nb_recombinations_param.lim_inf, 
							_nb_recombinations_param.lim_sup, 
							_nb_recombinations_param.distr, n) *
							setup.population_size() ) / 100);
		setup.set_nb_recombinations(i_val);

		/*_nb_mutations refers now to a proportion(%) of _population_size*/
		i_val = (int) ((generate_value(_nb_mutations_param.lim_inf, 
							_nb_mutations_param.lim_sup, 
							_nb_mutations_param.distr, n) *
							setup.population_size() ) / 100);
		setup.set_nb_mutations(i_val);

		setup.set_start_choice (generate_value(_start_choice_param.lim_inf, 
							_start_choice_param.lim_sup, 
							_start_choice_param.distr, n));

		setup.set_add_only_if_better (rand_(_add_only_if_better_probl));

		setup.set_select_choice (generate_value(_select_choice_param.lim_inf, 
							_select_choice_param.lim_sup, 
							_select_choice_param.distr, n));

		setup.set_select_extra_parameter (generate_value(_select_extra_parameter_param.lim_inf, 
							_select_extra_parameter_param.lim_sup, 
							_select_extra_parameter_param.distr, n));

		setup.set_cross_choice (generate_value(_cross_choice_param.lim_inf, 
							_cross_choice_param.lim_sup, 
							_cross_choice_param.distr, n));

		setup.set_recombine_selection (generate_value(_recombine_selection_param.lim_inf, 
							_recombine_selection_param.lim_sup, 
							_recombine_selection_param.distr, n));

		setup.set_rec_selection_extra (generate_value(_rec_selection_extra_param.lim_inf, 
							_rec_selection_extra_param.lim_sup, 
							_rec_selection_extra_param.distr, n));

		setup.set_mutate_choice (generate_value(_mutate_choice_param.lim_inf, 
							_mutate_choice_param.lim_sup, 
							_mutate_choice_param.distr, n));

		setup.set_mutate_extra_parameter (generate_value(_mutate_extra_parameter_param.lim_inf, 
							_mutate_extra_parameter_param.lim_sup, 
							_mutate_extra_parameter_param.distr, n));
		setup.set_mutate_selection (generate_value(_mutate_selection_param.lim_inf, 
							_mutate_selection_param.lim_sup, 
							_mutate_selection_param.distr, n));

		setup.set_mut_selection_extra (generate_value(_mut_selection_extra_param.lim_inf, 
							_mut_selection_extra_param.lim_sup, 
							_mut_selection_extra_param.distr, n));

		setup.set_local_search_choice(generate_value(_local_search_choice_param.lim_inf, 
							_local_search_choice_param.lim_sup, 
							_local_search_choice_param.distr, n));

		setup.set_nb_local_search_iterations (generate_value(_nb_local_search_iterations_param.lim_inf, 
							_nb_local_search_iterations_param.lim_sup, 
							_nb_local_search_iterations_param.distr, n));

		/*_nb_ls_not_improving_iterations refers now to a proportion(%) of _nb_local_search_iterations*/
		d_val = ((generate_value(_nb_ls_not_improving_iterations_param.lim_inf, 
							_nb_ls_not_improving_iterations_param.lim_sup, 
							_nb_ls_not_improving_iterations_param.distr, n) *
							setup.nb_local_search_iterations() ) / 100);
		setup.set_nb_ls_not_improving_iterations (d_val);

		setup.set_lsearch_extra_parameter (generate_value(_lsearch_extra_parameter_param.lim_inf, 
							_lsearch_extra_parameter_param.lim_sup, 
							_lsearch_extra_parameter_param.distr, n));

		setup.set_slave_local_search(_slave_local_search);
		setup.set_trace (_trace);
	}

	double Setup_IR::generate_value (double inf, double sup, char distr, int n) const
	{
		Distribution* d = NULL;
		double mean = (sup + inf)/2;
		double res = 0;
		long int seed = (long int) rand_(0, n*(unsigned)time(NULL));
				
		if (n == 1) res = mean;
		else
		{
			switch (distr) 
			{
				case 'c':
					d = new ConstantDistribution(mean, 0);
				break;

				case 'n':
					d = new NormalDistribution (mean, (mean-inf)/2, seed);
				break;

				case 'u':
					d = new UniformDistribution (inf, sup, seed);
				break;

				case 'e':
					d = new ExponentialDistribution (mean, seed);
				break;

				case 'z':
					d = new ZipfDistribution (inf, sup, 0.96, seed);
				break;

				case 't':
					d = new TriangleDistribution (inf, mean, sup, seed);
				break;

				default:
					d = new ConstantDistribution(mean, 0);
				break;
			}
			res = d->generate(0);
			res = min(max(inf, res), sup);
			delete d;
		}
		return res;
	}
	
	int Setup_IR::generate_value (int inf, int sup, char distr, int n) const
	{
		int res;
		res = (int) generate_value ((double) inf, (double) (sup + 1), distr, n);
		if (res > sup) res--;
		return res;
	}
  

	// ************************************************************************
	// Solver
	// ************************************************************************
	Solver::Solver (const Problem& pbm, const Setup& setup)
	:	_problem(pbm), 
		_setup(setup),
		_minimizing(pbm.direction()==Minimize),
		_best_solution(_problem),
		_best_Mksp_solution(_best_solution),
		_best_cost((_minimizing)?(plus_infinity):(minus_infinity)),
		_time_spent(0.0),				
		_time_best(0.0),
		_independent_run(0),
		_generation(0),
		_current_population(*this),
		_nb_repetitions_best_solution(0),
		_current_time_spent(0.0),
		_local_search_engine(*this)
	{
	}
	
	
	Solver::~Solver()
	{
	}
	
	const Problem& Solver::problem () const
	{
		return _problem;
	}
	
	const Setup& Solver::setup () const
	{
		return _setup;
	}
	
	Solution Solver::best_solution () const
	{
		return _best_solution;
	}

	Solution Solver::best_Mksp_solution () const
	{
		return _best_Mksp_solution;
	}
	
	double Solver::best_cost () const
	{
		return _best_cost;
	}
	
	float Solver::time_spent () const
	{
		return _time_spent;
	}

	float Solver::time_best  () const
	{
		return _time_best;
	}
	
	int Solver::independent_run () const
	{
		return _independent_run;
	}
	
	int Solver::generation () const
	{
		return _generation;
	}
	
	Population Solver::current_population () const
	{
		return _current_population;
	}

	Solution Solver::current_best_solution () const
	{
		return _current_population.best();
	}
	
	Solution Solver::current_best_Mksp_solution () const
	{
		return _current_population.best_Mksp_solution();
	}
	
	double Solver::current_best_cost () const
	{
		return _current_population.best().fitness();
	}
	
	int Solver::nb_repetitions_best_solution () const
	{
		return _nb_repetitions_best_solution;
	}
	
	float Solver::current_time_spent () const
	{
		return _current_time_spent;
	}
	
	void Solver::update_best_values ()
	{
		double cbcost = current_best_cost();
		Solution _current_best_Mksp_sol(_problem);
		_current_best_Mksp_sol = current_best_Mksp_solution();
		
		/*Tests if makespan value is improved*/
	    if (improvement(_current_best_Mksp_sol,_best_Mksp_solution))
		{
			_best_Mksp_solution = _current_best_Mksp_sol;
			/****************/
			#ifdef TRACE_MKSP
			(_setup.trace())->precision(13);
			*(_setup.trace()) << _generation << " ";
			*(_setup.trace()) << _best_Mksp_solution.local_makespan << " ";
			*(_setup.trace()) << _best_Mksp_solution.local_avg_utilization << " ";
			*(_setup.trace()) << _best_Mksp_solution.flowtime << endl;
			#endif
			/****************/
		}

		if (_minimizing)
		{
			if (cbcost < _best_cost) 
			{
				_best_solution = current_best_solution();
				_best_cost     = cbcost;
				_nb_repetitions_best_solution = 0;
				_time_best	   = _current_time_spent + _time_spent;

				/****************/
				#ifdef TRACE_FIT
				(_setup.trace())->precision(13);
				*(_setup.trace()) << _generation << " ";
				*(_setup.trace()) << _best_solution.local_makespan << " ";
				*(_setup.trace()) << _best_solution.local_avg_utilization << " ";
				*(_setup.trace()) << _best_solution.flowtime << endl;
				#endif
				/****************/
			} 
			else _nb_repetitions_best_solution++;
		} 
		else 
		{
			if (cbcost > _best_cost) 
			{
				_best_solution = current_best_solution();
				_best_cost     = cbcost;
				_nb_repetitions_best_solution = 0;
				_time_best	   = _current_time_spent + _time_spent;

				/****************/
				#ifdef TRACE_FIT
					(_setup.trace())->precision(13);
					*(_setup.trace()) << _generation << " ";
					*(_setup.trace()) << _best_solution.local_makespan << " ";
					*(_setup.trace()) << _best_solution.local_avg_utilization << " ";
					*(_setup.trace()) << _best_solution.flowtime << endl;
				#endif
				/****************/

			}
			else _nb_repetitions_best_solution++;
		}	
	}
	

	// ************************************************************************
	// Solver_Seq
	// ************************************************************************	
	Solver_Seq::Solver_Seq (const Problem& pbm, const Setup& setup)
	:	Solver(pbm, setup)
	{
	}
	
	
	Solver_Seq::~Solver_Seq ()
	{
	}
		
	void Solver_Seq::run ()
	{
		vector<Solution> spar;
		Solution offspring(_problem), s(_problem);
		float ltime, gtime;
		
		gtime = used_time();
		_time_spent = 0.0;

		while(_independent_run < _setup.nb_independent_runs())
		{
			#ifdef MA_VERBOSE
			cout << "\n\n-------------------------------------------" << endl;
			cout << " MEMETIC ALGORITHM: Independent Run # ";
			cout << _independent_run;
			cout << "\n-------------------------------------------" << endl;
			#endif		

			_generation = 0;
			_nb_repetitions_best_solution = 0;
			ltime = used_time();
			_current_time_spent = 0.0;
			_current_population.initialize();

			#ifdef MA_VERBOSE
			cout << "\nInitial population:\n" << _current_population << endl;			
			#endif		

			for (int i=0; i<_current_population.size(); i++)  
				_local_search_engine.apply_to(_current_population[i]);
			_current_population.update();

			#ifdef MA_VERBOSE
			cout << "\nImproved population:\n" << _current_population << endl;			
			#endif		

			while (!terminateQ(*this))
			{
				#ifdef MA_VERBOSE
				cout << "\n\nGENERATION #" << _generation+1;
				cout << " of " << _setup.nb_generations();
				cout << "\n----------------------" << endl << endl;
				#endif
				
				for(int j=1; j<=_setup.nb_recombinations(); j++)
				{	
					spar = _current_population.select_to_merge(true); 
					offspring = recombine(*this, spar);	
					_local_search_engine.apply_to(offspring);
					_current_population.add(offspring);
				}
				_current_population.update();				

				#ifdef RECOMBINATION_VERBOSE
				cout << "\nRecombined population:\n" << _current_population << endl;
				#endif		

				for (int j=1; j<=_setup.nb_mutations(); j++)
				{
					s = _current_population.select_to_mutate();
					s.mutate(*this);		
					_local_search_engine.apply_to(s);
					_current_population.add(s);					
				}
				_current_population.update();

				#ifdef MUTATION_VERBOSE
				cout << "\nMutated population:\n" << _current_population << endl;
				#endif		

				_current_population.select();
				update_best_values();

				if (_current_population.meets_conv_criteria()) 
				{
					_current_population.restart();
					update_best_values();

					#ifdef RESTART_VERBOSE
					cout << "\nRestarted population:\n" << _current_population << endl;
					#endif							
				}
								
				_current_time_spent += used_time(ltime);
				_generation++;

				#ifdef MA_VERBOSE
				cout << "\nPopulation at the end:\n" << _current_population << endl;
				#endif
			}
			
			_time_spent += used_time(gtime);
			_independent_run++;
		}
	}


	// ************************************************************************
	// Solver_Lan_IR
	// ************************************************************************	
	Solver_Lan_IR::Solver_Lan_IR (const Problem& pbm, const Setup& setup)
	:	Solver(pbm, setup),
		_best_config(setup),
		_last_it(0)
	{
	}
	
	Solver_Lan_IR::~Solver_Lan_IR ()
	{
	}

	Setup Solver_Lan_IR::best_configuration () const 
	{
		return _best_config;
	}

	int	Solver_Lan_IR::last_iteration () const
	{
		return _last_it;
	}
	
	void Solver_Lan_IR::run () 
	{		
		int rank;
		/*int err = MPI_Init(0, 0);
		assert(err==MPI_SUCCESS);*/
		MPI_Comm_size(MPI_COMM_WORLD, &_comm_size);
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);
				
		if(rank==0)
			master();
		else
			slave();
		
		/*MPI_Finalize();*/	
	}

	void Solver_Lan_IR::master()
	{
		#ifdef MA_VERBOSE
		cout << "Master initialized" << endl;
		#endif
		
		/*master waits results from other processes*/
		Solution sol_received(_problem);
		Solution sol_Mksp_received(sol_received);
		float fitness, time_received;
		float best_time;
		int nb_rep_best_sol;

		int best_proc = 0;
		for(int proc=1; proc<_comm_size; ++proc)
		{
			ipacket2 ip(proc);
			ip >> sol_received >> sol_Mksp_received >> time_received 
				>> best_time >> nb_rep_best_sol;
			fitness = sol_received.fitness();

			if(proc==1 ||
				(_minimizing && fitness<_best_cost) ||
			   (!_minimizing && fitness>_best_cost))
			{	
				_best_solution = sol_received;
				_best_cost = fitness;		
				_time_best = best_time;
				_nb_repetitions_best_solution = nb_rep_best_sol;
			}

			if (proc==1 || improvement(sol_Mksp_received, _best_Mksp_solution))
			{
				_best_Mksp_solution = sol_Mksp_received;
				best_proc = proc;
			}
			
			if(time_received>_time_spent)
				_time_spent = time_received;
		}
		
		/*takes configuration of the process that found best makespan valued solution*/
		for(int proc=1; proc<_comm_size; proc++)
		{
			opacket2 op_false; op_false << false;
			opacket2 op_true; op_true << true;
			
			if (proc == best_proc) op_true.send(proc);
			else op_false.send(proc);
		}
		
		if (best_proc > 0)
		{
			ipacket2 ip_conf(best_proc);
			ip_conf >> _best_config >> _last_it;
		}
		#ifdef MA_VERBOSE
		cout << "Master finalized" << endl;
		#endif				
	}
		
	void Solver_Lan_IR::slave()
	{	
		bool info;
		#ifdef MA_VERBOSE
		cout << "Slave initialized" << endl;
		#endif
		
		Solver_Seq solver(_problem, _setup);
	
		solver.run();
		
		opacket2 op;
		op << solver.best_solution() << solver.best_Mksp_solution() << solver.time_spent();
		op << solver.time_best() << solver.nb_repetitions_best_solution();
		op.send(0);

		ipacket2 ip(0);
		ip >> info;
		if (info)
		{
			opacket2 op_conf;
			op_conf << _setup << solver.generation();
			op_conf.send(0);
		}
		
		#ifdef MA_VERBOSE
		cout << "Slave finalized" << endl;
		#endif
	}
	
	/*********************************************************************/
    /* Solver_Lan_IR_DS                                                  */
    /*********************************************************************/
    Solver_Lan_IR_DS::Solver_Lan_IR_DS (const Problem& pbm, const Setup_IR& setup_IR)
    :   Solver(pbm,setup_IR),
		_setup_IR(setup_IR),
		_best_config(_setup),
		_last_it(0)
    {
    }

    Solver_Lan_IR_DS::~Solver_Lan_IR_DS() 
	{
    }

	Setup Solver_Lan_IR_DS::best_configuration () const 
	{
		return _best_config;
	}

	int	Solver_Lan_IR_DS::last_iteration () const
	{
		return _last_it;
	}

	void Solver_Lan_IR_DS::run () 
	{
		int rank;

		/*int err = MPI_Init(_setup.mpi_argc(), _setup.mpi_argv());
		  assert(err==MPI_SUCCESS);*/

		MPI_Comm_size(MPI_COMM_WORLD, &_comm_size);
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);

		if(rank==0) master();
		else slave();		
	}

	void Solver_Lan_IR_DS::master()
	{
		Solution sol_received(_problem);
		Solution sol_Mksp_received(sol_received);
		float fitness;
		float best_time, gtime;;
		int nb_rep_best_sol, last_it;

		gtime = MPI_Wtime();

		/*Sending the corresponding configuration to each process*/
		vector<Setup> proc_config(_comm_size-1);
		for (int proc=1; proc<_comm_size; proc++)
		{
			opacket2 op;
			_setup_IR.fill_setup(proc_config[proc-1], _comm_size-1);
			op << proc_config[proc-1];
			op.send(proc);
		}
		
		/*Receiving results from each process*/
		int best_proc = 0;
		for(int proc=1; proc<_comm_size; proc++)
		{
			ipacket2 ip(proc);
			ip >> sol_received >> sol_Mksp_received >> best_time >> nb_rep_best_sol >> last_it;						
			fitness = sol_received.fitness();

			if(proc==1||(_minimizing && fitness<_best_cost)||(!_minimizing && fitness>_best_cost))
			{	
				_best_solution = sol_received;
				_best_cost = fitness;
				
				_time_best = best_time;
				_nb_repetitions_best_solution = nb_rep_best_sol;
			}

			if (proc==1 || improvement(sol_Mksp_received, _best_Mksp_solution))
			{
				_best_Mksp_solution = sol_Mksp_received;
				best_proc = proc;
				_last_it = last_it;
			}			
		}

		/*Best configuration (the one which obtained best makespan value) is kept*/
		if (best_proc > 0) _best_config = proc_config[best_proc-1];
		_time_spent = MPI_Wtime() - gtime;
	}
	
	void Solver_Lan_IR_DS::slave()
	{	
		ipacket2 ip_config(0);
		Setup setup_proc;
		
		/*Receiving configuration*/
		ip_config >> setup_proc;		
		
		/*Running sequential Solver*/
		Solver_Seq solver(_problem, setup_proc);
		solver.run();
	
		/*Sending results to main Solver*/
		opacket2 op;
		op << solver.best_solution() << solver.best_Mksp_solution()
			<< solver.time_best() << solver.nb_repetitions_best_solution()
			<< solver.generation ();
		op.send(0);
	}

	// ************************************************************************
	// Solver_Lan_MS
	// ************************************************************************	
	
	Solver_Lan_MS::Solver_Lan_MS (const Problem& pbm, const Setup& setup)
	:	Solver(pbm, setup),
		_comm_time_spent(0.0)
	{
	}
	
	
	Solver_Lan_MS::~Solver_Lan_MS ()
	{
	}
	
	void Solver_Lan_MS::run ()
	{				
		int rank;
		
		/*int err = MPI_Init(0, 0);
		assert(err==MPI_SUCCESS);*/
		MPI_Comm_size(MPI_COMM_WORLD, &_comm_size);
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);
		
		if(rank==0)
			master();
		else
			slave();
		
		//MPI_Finalize();
	}
	
	float Solver_Lan_MS::comm_time_spent () const
	{
		return _comm_time_spent;
	}
	
	void Solver_Lan_MS::master()
	{
		float gtime =  MPI_Wtime(); 
		_time_spent = 0.0;
		vector<Solution>  spar, s(_comm_size, Solution(_problem));
		Solution offspring(_problem);
		Solution_Changes changes(_problem);
		float t;

		while(_independent_run < _setup.nb_independent_runs())
		{
			#ifdef MA_VERBOSE
			cout << "\n\n-------------------------------------------" << endl;
			cout << " MEMETIC ALGORITHM: Independent Run # ";
			cout << _independent_run;
			cout << "\n-------------------------------------------" << endl;
			#endif		
		
			float ltime = MPI_Wtime();
			_generation = 0;
			_nb_repetitions_best_solution = 0;
			_current_time_spent = 0.0;
			_current_population.initialize();

			#ifdef MA_VERBOSE
			cout << "\nInitial population:\n" << _current_population << endl;
			#endif		

			// Apply Local Search Engine to population
			for (int i=0; i<_current_population.size(); i+=_comm_size-1)
			{
				for(int proc=1; proc<_comm_size && i+proc-1<_current_population.size(); ++proc)
				{
					opacket2 op;
					op << SLAVE_LSE;					
					op << _current_population[i+proc-1];
					t =  MPI_Wtime();
					op.send(proc);
					_comm_time_spent +=  MPI_Wtime() - t;
				}
				for(int proc=1; proc<_comm_size && i+proc-1<_current_population.size(); ++proc)
				{
					ipacket2 ip(proc);					
					ip >> changes;
					changes.apply_to(_current_population[i+proc-1]);
				}
			}				
			_current_population.update();

			#ifdef MA_VERBOSE
			cout << "\nImproved population:\n" << _current_population << endl;
			#endif		

			while (!terminateQ(*this))
			{
				#ifdef MA_VERBOSE
				cout << "\n\nGENERATION #" << _generation+1;
				cout << " of " << _setup.nb_generations();
				cout << "\n----------------------" << endl << endl;
				#endif
				
				for(int j=1; j<=_setup.nb_recombinations(); j+=_comm_size-1)
				{
					for(int proc=1; proc<_comm_size && j+proc-1<=_setup.nb_recombinations(); ++proc)
					{
						spar = _current_population.select_to_merge(true);
						opacket2 op;
						op << SLAVE_RECOMBINE;
						for(int i=0; i<_setup.nb_solutions_to_recombine(); ++i)
							op << spar[i];						
						t =  MPI_Wtime();
						op.send(proc);
						_comm_time_spent +=  MPI_Wtime() - t;
					}					
					for(int proc=1; proc<_comm_size && j+proc-1<=_setup.nb_recombinations(); ++proc)
					{
						ipacket2 ip(proc);
						ip >> offspring;
						_current_population.add(offspring);
					}
				}		
				_current_population.update();

				#ifdef RECOMBINATION_VERBOSE
				cout << "\nRecombined population:\n" << _current_population;
				cout << endl;
				#endif		
				
				for (int j=1; j<=_setup.nb_mutations(); j+=_comm_size-1)
				{
					for(int proc=1; proc<_comm_size && j+proc-1<=_setup.nb_mutations(); ++proc)
					{
						s[proc-1] = _current_population.select_to_mutate();
						opacket2 op;
						op << SLAVE_MUTATE;
						op << s[proc-1];
						t =  MPI_Wtime();
						op.send(proc);
						_comm_time_spent +=  MPI_Wtime() - t;
					}
					for(int proc=1; proc<_comm_size && j+proc-1<=_setup.nb_mutations(); ++proc)
					{
						ipacket2 ip(proc);
						ip >> changes;
						changes.apply_to(s[proc-1]);
						_current_population.add(s[proc-1]);
					}
				}

				_current_population.update();

				#ifdef MUTATION_VERBOSE
				cout << "\nMutated population:\n" << _current_population;
				cout << endl;
				#endif		

				_current_population.select();
				update_best_values();

				if (_current_population.meets_conv_criteria()) 
				{
					_current_population.restart();
					update_best_values();

					#ifdef RESTART_VERBOSE
					cout << "\nRestarted population:\n";
					cout << _current_population << endl;
					#endif							
				}
				
				_current_time_spent -= ltime;
				ltime = MPI_Wtime();
				_current_time_spent += ltime;
				_generation++;							

				#ifdef MA_VERBOSE
				cout << "\nPopulation at the end:\n";
				cout << _current_population << endl;
				#endif
			}
			_time_spent -= gtime;
			gtime = MPI_Wtime();
			_time_spent += gtime;
			_independent_run++;
		}
		// send terminate signal to slaves
		for(int proc=1; proc<_comm_size; ++proc)
		{
			opacket2 op;
			op << SLAVE_TERMINATE;
			t =  MPI_Wtime();
			op.send(proc);
			_comm_time_spent +=  MPI_Wtime() - t;
		}
		// read communication time spent from slaves
		for(int proc=1; proc<_comm_size; ++proc)
		{
			ipacket2 ip(proc);
			ip >> t;
			_comm_time_spent += t;			
		}
		_time_spent -= gtime;
		gtime = MPI_Wtime();
		_time_spent += gtime;
	}
	
	void Solver_Lan_MS::slave()
	{
		int action;
		Solution sol1(_problem), sol2(_problem), offspring(_problem);
		vector<Solution> spar(_setup.nb_solutions_to_recombine(), Solution(_problem));
		Solution_Changes changes(_problem);
		float t;

		/* Setting the Local Search Engine particular to this slave for  */
		/* it may perform a different Local Search from the other slaves */
		int rank;
		LS_config ls_conf;
		Setup slave_setup = _setup;

		/*Taking Local Serch Configuration from Setup.slave_local_search list acording to its rank*/
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);
		if ((unsigned int) rank-1 < _setup.slave_local_search().size())
		{
			ls_conf = _setup.slave_local_search()[rank-1];
			slave_setup.set_local_search_choice(ls_conf.lsearch_id);
			slave_setup.set_nb_local_search_iterations (ls_conf.nb_lsearch_iterations);
			slave_setup.set_nb_ls_not_improving_iterations (ls_conf.nb_ls_not_improving_it);
			slave_setup.set_lsearch_extra_parameter (ls_conf.lsearch_extra_parameter);
		}

		Solver_Seq solver_aux(_problem, slave_setup);
		Local_Search_Engine slave_lsearch_engine(solver_aux);
	
		for(int i=0; i<_setup.nb_solutions_to_recombine(); ++i)
			spar[i] = Solution(_problem);
				
		// Slave loop
		do
		{
			// Receive request from the master
			ipacket2 ip(0);
			ip >> action;
			
			opacket2 op;
			switch(action)
			{
				case SLAVE_LSE:
					ip >> sol1;	
					sol2 = sol1;
					slave_lsearch_engine.apply_to(sol1);
					changes.differences(sol2, sol1);
					op << changes;
					t =  MPI_Wtime();
					op.send(0);
					_comm_time_spent +=  MPI_Wtime() - t;
					break;
				
				case SLAVE_RECOMBINE:
					for(int i=0; i<_setup.nb_solutions_to_recombine(); ++i)
						ip >> spar[i];
					offspring = recombine(*this, spar);
					slave_lsearch_engine.apply_to(offspring);
					op << offspring;
					t =  MPI_Wtime();
					op.send(0);
					_comm_time_spent +=  MPI_Wtime() - t;
					break;
				
				case SLAVE_MUTATE:
					ip >> sol1;
					sol2 = sol1;
					sol1.mutate(*this);
					slave_lsearch_engine.apply_to(sol1);
					changes.differences(sol2, sol1);
					op << changes;
					t =  MPI_Wtime();
					op.send(0);
					_comm_time_spent +=  MPI_Wtime() - t;
					break;
			}
		} while(action!=SLAVE_TERMINATE);
		
		// send communication time spent
		opacket2 op;
		op << _comm_time_spent;
		op.send(0);
	}

	/*--------------------AUXILIAR ATTRIBUTES DEFINITIONS----------------*/
	   bool ENERGY_CASE;
	/*-------------------------------------------------------------------*/
}
