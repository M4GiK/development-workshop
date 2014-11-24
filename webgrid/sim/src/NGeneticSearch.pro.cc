/*************************************************************************/
/* FILE: GeneticSearch.pro.cc                                            */
/*************************************************************************/
#include "NGeneticSearch.hh"

bool compare_solutions (const NGeneticSearch::Solution* i, const NGeneticSearch::Solution* j);

skeleton NGeneticSearch
{
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
        _struggle_replace(false),
		_struggle_extra_parameter(1),
        _start_choice(StartAll),
        _user_information("")
    {
    }
	
	ostream& operator<< (ostream& os, const Setup& stp) 
	{
        os << stp._nb_independent_runs          << endl;
        os << stp._nb_evolution_steps           << endl;
        os << stp._max_time_to_spend            << endl;
        os << stp._nb_repetitions_to_end        << endl;
        os << stp._population_size              << endl;
        os << stp._intermediate_population_size << endl;
        os << stp._cross_probability            << endl;
        os << stp._mutate_probability           << endl;
        os << stp._select_choice                << endl;
        os << stp._select_extra_parameter       << endl;
        os << stp._cross_choice                 << endl;
        os << stp._cross_extra_parameter        << endl;
        os << stp._mutate_choice                << endl;
        os << stp._mutate_extra_parameter       << endl;
        os << stp._replace_only_if_better       << endl;
        os << stp._replace_generational         << endl;
		os << stp._struggle_replace             << endl;
		os << stp._struggle_extra_parameter		<< endl;
        os << stp._start_choice                 << endl;
        
        return os;
    }

    istream& operator>> (istream& is, Setup& stp) 
	{
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
		is >> stp._struggle_replace;
		is >> stp._struggle_extra_parameter;
        is >> stp._start_choice;
        is >> stp._user_information;

        return is;
    }

	opacket2& operator<< (opacket2& op, const Setup& stp) 
	{
		op << stp._nb_independent_runs;
        op << stp._nb_evolution_steps;
        op << stp._max_time_to_spend;
        op << stp._nb_repetitions_to_end;
        op << stp._population_size;
        op << stp._intermediate_population_size;
        op << stp._cross_probability;
        op << stp._mutate_probability;
        op << stp._select_choice;
        op << stp._select_extra_parameter;
        op << stp._cross_choice;
        op << stp._cross_extra_parameter;
        op << stp._mutate_choice;
        op << stp._mutate_extra_parameter;
        op << stp._replace_only_if_better;
        op << stp._replace_generational;
		op << stp._struggle_replace;
		op << stp._struggle_extra_parameter;
        op << stp._start_choice;
        op << stp._user_information;

		return op;
	}

    ipacket2& operator>> (ipacket2& ip, Setup& stp) 
	{
		ip >> stp._nb_independent_runs;
        ip >> stp._nb_evolution_steps;
        ip >> stp._max_time_to_spend;
        ip >> stp._nb_repetitions_to_end;
        ip >> stp._population_size;
        ip >> stp._intermediate_population_size;
        ip >> stp._cross_probability;
        ip >> stp._mutate_probability;
        ip >> stp._select_choice;
        ip >> stp._select_extra_parameter;
        ip >> stp._cross_choice;
        ip >> stp._cross_extra_parameter;
        ip >> stp._mutate_choice;
        ip >> stp._mutate_extra_parameter;
        ip >> stp._replace_only_if_better;
        ip >> stp._replace_generational;
		ip >> stp._struggle_replace;
		ip >> stp._struggle_extra_parameter;
        ip >> stp._start_choice;
        ip >> stp._user_information;

		return ip;
	}

    int Setup::nb_independent_runs () const 
	{
        return _nb_independent_runs;
    }

    int Setup::nb_evolution_steps () const 
	{
        return _nb_evolution_steps;
    }
	
    float Setup::max_time_to_spend () const 
	{
        return _max_time_to_spend;
    }
	
    int Setup::nb_repetitions_to_end () const 
	{
    	return _nb_repetitions_to_end;
    }
    
    int Setup::population_size () const 
	{
        return _population_size;
    }

    int Setup::intermediate_population_size () const 
	{
        return _intermediate_population_size;
    }

    float Setup::cross_probability () const 
	{
        return _cross_probability;
    }

    float Setup::mutate_probability () const 
	{
        return _mutate_probability;
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

    float Setup::cross_extra_parameter () const 
	{
        return _cross_extra_parameter;
    }

    int Setup::mutate_choice () const 
	{
        return _mutate_choice;
    }

    float Setup::mutate_extra_parameter () const 
	{
        return _mutate_extra_parameter;
    }

    bool Setup::replace_only_if_better () const 
	{
        return _replace_only_if_better;
    }

    bool Setup::replace_generational () const 
	{
        return _replace_generational;
    }

    bool Setup::struggle_replace () const 
	{
        return _struggle_replace;
    }

	int Setup::struggle_extra_parameter () const 
	{
		return _struggle_extra_parameter;
	}

    int Setup::start_choice () const 
	{
        return _start_choice;
    }	

    const string& Setup::user_information () const 
	{
        return _user_information;
    }

    ofstream* Setup::trace() const {
        return _trace;
    }

    int Setup::energy_case() const {
    	return ENERGY_CASE;
    }
 
    void Setup::set_nb_independent_runs (int i) 
	{
        assert(i>=1);
        _nb_independent_runs = i;
    }

    void Setup::set_nb_evolution_steps (int i) 
	{
        assert(i>=1);
        _nb_evolution_steps = i;
    }
	
    void Setup::set_max_time_to_spend (float f) 
	{
        assert(f>=0.0);
        _max_time_to_spend = f;
    }
	
    void Setup::set_nb_repetitions_to_end (int i) 
	{
    	_nb_repetitions_to_end = i;
    }
    
    void Setup::set_population_size (int i) 
	{
        assert(i>=1);
        _population_size = i;
    }

    void Setup::set_intermediate_population_size (int i) 
	{
        assert(i>=1);
        _intermediate_population_size = i;
    }

    void Setup::set_cross_probability (float f) 
	{
        assert((f>=0) && (f<=1));
        _cross_probability = f;
    }

    void Setup::set_mutate_probability (float f) 
	{
        assert((f>=0) && (f<=1));
        _mutate_probability = f;
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

    void Setup::set_cross_extra_parameter (float f) 
	{
        _cross_extra_parameter = f;
    }

    void Setup::set_mutate_choice (int i)
	{
        _mutate_choice = i;
    }

    void Setup::set_mutate_extra_parameter (float f)
	{
        _mutate_extra_parameter = f;
    }

    void Setup::set_replace_only_if_better (bool b)
	{
        _replace_only_if_better = b;
    }

    void Setup::set_replace_generational (bool b) 
	{
        _replace_generational = b;
    }

    void Setup::set_struggle_replace (bool b)
	{
        _struggle_replace = b;
    }

	void Setup::set_struggle_extra_parameter (int i)
	{
		_struggle_extra_parameter = i;
	}

    void Setup::set_start_choice (int i)
	{
        _start_choice = i;
    }
	
    void Setup::set_user_information (const string& s)
	{
        _user_information = s;
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
		_nb_repetitions_to_end_param.lim_inf = _nb_repetitions_to_end;
        _population_size_param.lim_inf = _population_size;        
        _intermediate_population_size_param.lim_inf = _intermediate_population_size;
        _cross_probability_param.lim_inf = _cross_probability;
        _mutate_probability_param.lim_inf = _mutate_probability;
        _select_choice_param.lim_inf = _select_choice;
        _select_extra_parameter_param.lim_inf = _select_extra_parameter;
        _cross_choice_param.lim_inf = _cross_choice;
        _cross_extra_parameter_param.lim_inf = _cross_extra_parameter;
        _mutate_choice_param.lim_inf = _mutate_choice;
        _mutate_extra_parameter_param.lim_inf = _mutate_extra_parameter;
		_struggle_extra_parameter_param.lim_inf = _struggle_extra_parameter;
        _start_choice_param.lim_inf = _start_choice;

		/*High limits*/
		_nb_repetitions_to_end_param.lim_sup = _nb_repetitions_to_end;
        _population_size_param.lim_sup = _population_size;        
        _intermediate_population_size_param.lim_sup = _intermediate_population_size;
        _cross_probability_param.lim_sup = _cross_probability;
        _mutate_probability_param.lim_sup = _mutate_probability;
        _select_choice_param.lim_sup = _select_choice;
        _select_extra_parameter_param.lim_sup = _select_extra_parameter;
        _cross_choice_param.lim_sup = _cross_choice;
        _cross_extra_parameter_param.lim_sup = _cross_extra_parameter;
        _mutate_choice_param.lim_sup = _mutate_choice;
        _mutate_extra_parameter_param.lim_sup = _mutate_extra_parameter;
		_struggle_extra_parameter_param.lim_sup = _struggle_extra_parameter;
        _start_choice_param.lim_sup = _start_choice;

		/*Parameters distributions*/
		_nb_repetitions_to_end_param.distr = 'c';
        _population_size_param.distr = 'c';        
        _intermediate_population_size_param.distr = 'c';
        _cross_probability_param.distr = 'c';
        _mutate_probability_param.distr = 'c';
        _select_choice_param.distr = 'c';
        _select_extra_parameter_param.distr = 'c';
        _cross_choice_param.distr = 'c';
        _cross_extra_parameter_param.distr = 'c';
        _mutate_choice_param.distr = 'c';
        _mutate_extra_parameter_param.distr = 'c';
		_struggle_extra_parameter_param.distr = 'c';
        _start_choice_param.distr = 'c';

		/*boolean parameters probabilities*/
		_replace_only_if_better_probl = (_replace_only_if_better)?(1.0):(0.0);
        _replace_generational_probl = (_replace_generational)?(1.0):(0.0);
        _struggle_replace_probl = (_struggle_replace)?(1.0):(0.0);
	}

	void Setup_IR::set_nb_repetitions_to_end (int i) 
	{
		((Setup) *this).set_nb_repetitions_to_end(i);
		_nb_repetitions_to_end_param.lim_inf = i;
		_nb_repetitions_to_end_param.lim_sup = i;
        _nb_repetitions_to_end_param.distr = 'c';
	}

	void Setup_IR::set_population_size (int i) {
		((Setup) *this).set_population_size (i);
		_population_size_param.lim_inf = i;
		_population_size_param.lim_sup = i;
		_population_size_param.distr = 'c';
	}

	void Setup_IR::set_intermediate_population_size (int i)
	{
		/*_intermediate_population_size refers now to a proportion(%) of _population_size*/
		((Setup) *this).set_intermediate_population_size(i);
		_intermediate_population_size_param.lim_inf = i;
		_intermediate_population_size_param.lim_sup = i;
		_intermediate_population_size_param.distr = 'c';
	}

	void Setup_IR::set_cross_probability (float f)
	{
		((Setup) *this).set_cross_probability(f);
		_cross_probability_param.lim_inf = f;
		_cross_probability_param.lim_sup = f;
		_cross_probability_param.distr = 'c';
	}

	void Setup_IR::set_mutate_probability (float f)
	{
		((Setup) *this).set_mutate_probability(f);
		_mutate_probability_param.lim_inf = f;
		_mutate_probability_param.lim_sup = f;
		_mutate_probability_param.distr = 'c';
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

	void Setup_IR::set_cross_extra_parameter (float f)
	{
		((Setup) *this).set_cross_extra_parameter(f);
		_cross_extra_parameter_param.lim_inf = f;
		_cross_extra_parameter_param.lim_sup = f;
		_cross_extra_parameter_param.distr = 'c';
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

	void Setup_IR::set_replace_only_if_better (bool b)
	{
		((Setup) *this).set_replace_only_if_better(b);
		_replace_only_if_better_probl = (b)?(1.0):(0.0);
	}

	void Setup_IR::set_replace_generational (bool b)
	{
		((Setup) *this).set_replace_generational(b);
		_replace_generational_probl = (b)?(1.0):(0.0);
	}

	void Setup_IR::set_struggle_replace (bool b)
	{
		((Setup) *this).set_struggle_replace(b);
		_struggle_replace_probl = (b)?(1.0):(0.0);
	}

	void Setup_IR::set_struggle_extra_parameter (int i)
	{
		((Setup) *this).set_struggle_extra_parameter(i);
		_struggle_extra_parameter_param.lim_inf = i;
		_struggle_extra_parameter_param.lim_sup = i;
		_struggle_extra_parameter_param.distr = 'c';
	}

	void Setup_IR::set_start_choice (int i)
	{
		((Setup) *this).set_start_choice(i);
		_start_choice_param.lim_inf = i;
		_start_choice_param.lim_sup = i;
		_start_choice_param.distr = 'c';
	}

	void Setup_IR::set_nb_repetitions_to_end (int lim_inf, int lim_sup, char distr)
	{
		_nb_repetitions_to_end = (lim_sup + lim_inf)/2;
		_nb_repetitions_to_end_param.lim_inf = lim_inf;
		_nb_repetitions_to_end_param.lim_sup = lim_sup;
		_nb_repetitions_to_end_param.distr = distr;
	}

	void Setup_IR::set_population_size (int lim_inf, int lim_sup, char distr)
	{
		_population_size = (lim_sup + lim_inf)/2;
		_population_size_param.lim_inf = lim_inf;
		_population_size_param.lim_sup = lim_sup;
		_population_size_param.distr = distr;
	}

	void Setup_IR::set_intermediate_population_size (int lim_inf, int lim_sup, char distr)
	{
		_intermediate_population_size = (lim_sup + lim_inf)/2;
		_intermediate_population_size_param.lim_inf = lim_inf;
		_intermediate_population_size_param.lim_sup = lim_sup;
		_intermediate_population_size_param.distr = distr;
	}

	void Setup_IR::set_cross_probability (float lim_inf, float lim_sup, char distr)
	{
		_cross_probability = (lim_sup + lim_inf)/2;
		_cross_probability_param.lim_inf = lim_inf;
		_cross_probability_param.lim_sup = lim_sup;
		_cross_probability_param.distr = distr;
	}

	void Setup_IR::set_mutate_probability (float lim_inf, float lim_sup, char distr)
	{
		_mutate_probability = (lim_sup + lim_inf)/2;
		_mutate_probability_param.lim_inf = lim_inf;
		_mutate_probability_param.lim_sup = lim_sup;
		_mutate_probability_param.distr = distr;
	}

	void Setup_IR::set_select_choice(int lim_inf, int lim_sup, char distr)
	{
		_select_choice = (lim_sup + lim_inf)/2;
		_select_choice_param.lim_inf = lim_inf;
		_select_choice_param.lim_sup = lim_sup;
		_select_choice_param.distr = distr;
	}

	void Setup_IR::set_select_extra_parameter(float lim_inf, float lim_sup, char distr)
	{
		_select_extra_parameter = (lim_sup + lim_inf)/2;
		_select_extra_parameter_param.lim_inf = lim_inf;
		_select_extra_parameter_param.lim_sup = lim_sup;
		_select_extra_parameter_param.distr = distr;
	}

	void Setup_IR::set_cross_choice(int lim_inf, int lim_sup, char distr)
	{
		_cross_choice = (lim_sup + lim_inf)/2;
		_cross_choice_param.lim_inf = lim_inf;
		_cross_choice_param.lim_sup = lim_sup;
		_cross_choice_param.distr = distr;
	}

	void Setup_IR::set_cross_extra_parameter(float lim_inf, float lim_sup, char distr)
	{
		_cross_extra_parameter = (lim_sup + lim_inf)/2;
		_cross_extra_parameter_param.lim_inf = lim_inf;
		_cross_extra_parameter_param.lim_sup = lim_sup;
		_cross_extra_parameter_param.distr = distr;
	}

	void Setup_IR::set_mutate_choice(int lim_inf, int lim_sup, char distr)
	{
		_mutate_choice = (lim_sup + lim_inf)/2;
		_mutate_choice_param.lim_inf = lim_inf;
		_mutate_choice_param.lim_sup = lim_sup;
		_mutate_choice_param.distr = distr;
	}

	void Setup_IR::set_mutate_extra_parameter(float lim_inf, float lim_sup, char distr)
	{
		_mutate_extra_parameter = (lim_sup + lim_inf)/2;
		_mutate_extra_parameter_param.lim_inf = lim_inf;
		_mutate_extra_parameter_param.lim_sup = lim_sup;
		_mutate_extra_parameter_param.distr = distr;
	}

	void Setup_IR::set_replace_only_if_better(double p)
	{
		_replace_only_if_better_probl = p;
	}

	void Setup_IR::set_replace_generational(double p)
	{
		_replace_generational_probl = p;
	}

	void Setup_IR::set_struggle_replace(double p)
	{
		_struggle_replace_probl = p;
	}

	void Setup_IR::set_struggle_extra_parameter(int lim_inf, int lim_sup, char distr)
	{
		_struggle_extra_parameter = (lim_sup + lim_inf)/2;
		_struggle_extra_parameter_param.lim_inf = lim_inf;
		_struggle_extra_parameter_param.lim_sup = lim_sup;
		_struggle_extra_parameter_param.distr = distr;
	}

	void Setup_IR::set_start_choice(int lim_inf, int lim_sup, char distr)
	{
		_start_choice = (lim_sup + lim_inf)/2;
		_start_choice_param.lim_inf = lim_inf;
		_start_choice_param.lim_sup = lim_sup;
		_start_choice_param.distr = distr;
	}

	void Setup_IR::fill_setup(Setup& setup, int n) const
	{
		setup.set_nb_independent_runs (_nb_independent_runs);
        setup.set_nb_evolution_steps (_nb_evolution_steps);
        setup.set_max_time_to_spend (_max_time_to_spend);
        setup.set_nb_repetitions_to_end 
			(generate_value(_nb_repetitions_to_end_param.lim_inf, 
							_nb_repetitions_to_end_param.lim_sup, 
							_nb_repetitions_to_end_param.distr, n));

        setup.set_population_size 
			(generate_value(_population_size_param.lim_inf, 
							_population_size_param.lim_sup, 
							_population_size_param.distr, n));

		/*_intermediate_population_size refers to a proportion(%) of _population_size*/
		int val = (int) ((generate_value(_intermediate_population_size_param.lim_inf, 
							_intermediate_population_size_param.lim_sup, 
							_intermediate_population_size_param.distr, n) *
							setup.population_size() ) / 100);
		val = max(val/2, 1);

        setup.set_intermediate_population_size (val);

        setup.set_cross_probability 
			(generate_value(_cross_probability_param.lim_inf, 
							_cross_probability_param.lim_sup, 
							_cross_probability_param.distr, n));

        setup.set_mutate_probability 
			(generate_value(_mutate_probability_param.lim_inf, 
							_mutate_probability_param.lim_sup, 
							_mutate_probability_param.distr, n));

        setup.set_select_choice 
			(generate_value(_select_choice_param.lim_inf, 
							_select_choice_param.lim_sup, 
							_select_choice_param.distr, n));

        setup.set_select_extra_parameter 
			(generate_value(_select_extra_parameter_param.lim_inf, 
							_select_extra_parameter_param.lim_sup, 
							_select_extra_parameter_param.distr, n));

        setup.set_cross_choice 
			(generate_value(_cross_choice_param.lim_inf, 
							_cross_choice_param.lim_sup, 
							_cross_choice_param.distr, n));

        setup.set_cross_extra_parameter 
			(generate_value(_cross_extra_parameter_param.lim_inf, 
							_cross_extra_parameter_param.lim_sup, 
							_cross_extra_parameter_param.distr, n));

        setup.set_mutate_choice 
			(generate_value(_mutate_choice_param.lim_inf, 
							_mutate_choice_param.lim_sup,
							_mutate_choice_param.distr, n));

        setup.set_mutate_extra_parameter 
			(generate_value(_mutate_extra_parameter_param.lim_inf, 
							_mutate_extra_parameter_param.lim_sup, 
							_mutate_extra_parameter_param.distr, n));

        setup.set_replace_only_if_better (rand_(_replace_only_if_better_probl));

        setup.set_replace_generational (rand_(_replace_generational_probl));

        setup.set_struggle_replace (rand_(_struggle_replace_probl));

		setup.set_struggle_extra_parameter 
			(generate_value(_struggle_extra_parameter_param.lim_inf, 
							_struggle_extra_parameter_param.lim_sup, 
							_struggle_extra_parameter_param.distr, n));

        setup.set_start_choice (generate_value(_start_choice_param.lim_inf, 
								_start_choice_param.lim_sup, _start_choice_param.distr, n));
        setup.set_user_information (_user_information);
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
		_best_Mksp_solution(_best_solution),
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

    void Solver::set_current_solution (const Solution& sol)
	{
        _current_solution = sol;
        _current_cost = sol.fitness();

        update_state(sol);
		update_Mksp_state(sol);
    }

   void Solver::set_current_solution(const Solution& sol,const double cost)
   {
        _current_solution = sol;
        _current_cost = cost;

        update_state(sol);
		update_Mksp_state(sol);
   }

    int Solver::independent_run () const 
	{
        return _independent_run;
    }

    float Solver::time_spent () const 
	{
        return _time_spent;
    }

    Solution Solver::best_solution () const 
	{
        return _best_solution;
    }

    Solution Solver::best_Mksp_solution () const  
	{
	    return _best_Mksp_solution;
    }


    int Solver::independent_run_best_found () const
	{
        return _independent_run_best_found;
    }

    int Solver::iteration_best_found () const 
	{
        return _iteration_best_found;
    }

    float Solver::time_best_found () const 
	{
        return _time_best_found;
    }

    double Solver::best_cost () const
	{
        return _best_cost;
    }

    double Solver::worst_cost () const 
	{
        return _worst_cost;
    }

    int Solver::nb_repetitions_best_solution () const
	{
        return _nb_repetitions_best_solution;
    }	

    Solution Solver::current_solution () const 
	{
        return _current_solution;
    }

    double Solver::current_cost () const
	{
        return _current_cost;
    }

    int Solver::current_iteration () const 
	{
        return _current_iteration;
    }

    Solution Solver::current_initial_solution () const
	{
        return _current_initial_solution;
    }

    double Solver::current_initial_cost () const
	{
        return _current_initial_cost;
    }

    float Solver::current_time_spent () const 
	{
        return _current_time_spent;
    }

    Solution Solver::current_best_solution () const
	{
        return _current_best_solution;
    }

    int Solver::current_best_solution_iteration () const 
	{
        return _current_best_solution_iteration;
    }

    float Solver::current_best_solution_time () const
	{
        return _current_best_solution_time;
    }

    double Solver::current_best_cost () const
	{
        return _current_best_cost;
    }

    double Solver::current_worst_cost () const 
	{
        return _current_worst_cost;
    }

    void Solver::update_state (const Solution& sol)
	{ 
		_current_solution = sol;
		_current_cost     = sol.fitness();
		int sign = ((_minimizing)?(1):(-1));
		
		if ( sol.is_better_than(_current_best_solution) ) 
		{
			_current_best_solution = _current_solution;
			_current_best_cost = _current_cost;
			_current_best_solution_iteration = _current_iteration;
			_current_best_solution_time = _current_time_spent;
		}
		
		if ( sol.is_better_than(_best_solution) ) 
		{   
			_best_solution = _current_solution;
			_best_cost     = _current_cost;
            _independent_run_best_found = _independent_run;
            _iteration_best_found = _current_iteration;
            _time_best_found = _time_spent;			 
			/*******************/			
			#ifdef TRACE_FIT
		    (_setup.trace())->precision(13);
		    *(_setup.trace()) << _iteration_best_found << " " 
			       << _best_solution.local_makespan << " " 
				   << _best_solution.local_avg_utilization << " "
				   << _best_solution.flowtime << endl;	    
			#endif
			/******************/
		    _nb_repetitions_best_solution = 0;
		} else _nb_repetitions_best_solution++;
		
		if ((sign * _current_cost) > (sign * _current_worst_cost))
			_current_worst_cost = _current_cost;
		
		if ((sign * _current_cost) > (sign * _worst_cost))
			_worst_cost = _current_cost;	
	}

	void  Solver::update_Mksp_state (const Solution& sol)
	/*To maintain best makespan valued solution*/
	{
		/*improvement method indicates a makespan improvement*/
		if (improvement(sol, _best_Mksp_solution))
		{
			_best_Mksp_solution = sol;
			/***********************/
			#ifdef TRACE_MKSP	
			(_setup.trace())->precision(13);
			*(_setup.trace()) << _current_iteration << " " 
				<< _best_Mksp_solution.local_makespan << " " 
				<< _best_Mksp_solution.local_avg_utilization << " "
			<< _best_Mksp_solution.flowtime << endl;	   
			#endif
			/***********************/
		}
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

    Solver_Seq::~Solver_Seq () 
	{
    }

    void Solver_Seq::run () 
	{
		_independent_run = 1;
        _gtime = used_time();
        _time_spent = 0;  
        _best_solution = Solution(_problem);
		_best_Mksp_solution = Solution(_best_solution);
        _independent_run_best_found = 0;
        _iteration_best_found = 0;
        _best_cost  = ((_minimizing)?(plus_infinity):(minus_infinity));
        _worst_cost = ((_minimizing)?(minus_infinity):(plus_infinity));
		
		int nb_independent_runs = _setup.nb_independent_runs();
		while (_independent_run <= nb_independent_runs) 
		{
			perform_one_independent_run();
			_independent_run++;
        }
    }

    void Solver_Seq::perform_one_independent_run ()	{
		_parents.start ();
		_parents.sort ();
		_parents.update();
		update_state(_parents.best_solution());
		update_Mksp_state(_parents.best_Mksp_solution());      
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
			update_Mksp_state (_current_solution);
            _1st_phase_belongs_independent_run = false;
        }

		// Generational loop (phase execution)	
        _offsprings = _parents.select();       	
        _offsprings.cross();
        update_state(_offsprings.best_solution());
        update_Mksp_state(_offsprings.best_Mksp_solution ());
        _offsprings.mutate();
        update_state(_offsprings.best_solution());	
        update_Mksp_state(_offsprings.best_Mksp_solution ());
        _parents.replace(_offsprings);
        
        // Update the global and local state
        _current_iteration++;
        _current_time_spent += used_time(_ltime);
        _time_spent += used_time(_gtime);        
    }

	/*********************************************************************/
    /* Solver_Lan_IR	                                                 */
    /*********************************************************************/
    Solver_Lan_IR::Solver_Lan_IR (const Problem& pbm, const Setup& setup)
    :   Solver(pbm,setup),
		_best_config(setup),
		_last_it(0)
    {
    }

    Solver_Lan_IR::~Solver_Lan_IR() 
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

		/*int err = MPI_Init(_setup.mpi_argc(), _setup.mpi_argv());
		  assert(err==MPI_SUCCESS);*/

		MPI_Comm_size(MPI_COMM_WORLD, &_comm_size);
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);

		if(rank==0) master();
		else slave();		
	}

	void Solver_Lan_IR::perform_one_independent_run ()
	{
	}

	void Solver_Lan_IR::perform_one_phase ()
	{
	}

	void Solver_Lan_IR::master()
	{
		Solution sol_proc(_problem);
		Solution sol_Mksp_proc(sol_proc);
		int best_ir, best_it, nb_rep_best_sol;
		float best_time, gtime;
		double worst_cost;
		int sign = (_minimizing)?(1):(-1);

		gtime = MPI_Wtime();

		int best_proc = 0;

		/*receives results from all processes and takes the best*/
		for(int proc=1; proc<_comm_size; proc++)
		{
			ipacket2 ip(proc);
			ip >> sol_proc >> sol_Mksp_proc;
			ip >> best_ir >> best_it >> best_time >> worst_cost >> nb_rep_best_sol;

			if (proc == 1 || sol_proc.is_better_than(_best_solution))
			{
				_best_solution = sol_proc;
				_best_cost = sol_proc.fitness();
				_independent_run_best_found = best_ir;				
				_iteration_best_found = best_it;
				_time_best_found = best_time;				
				_nb_repetitions_best_solution = nb_rep_best_sol;
				if (sign*worst_cost > sign*_worst_cost) 
					_worst_cost = worst_cost;
			}
			if (proc == 1 || improvement(sol_Mksp_proc, _best_Mksp_solution))
			{
				_best_Mksp_solution = sol_Mksp_proc;
				best_proc = proc;
			}
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
		_time_spent = MPI_Wtime() - gtime;
	}
		
	void Solver_Lan_IR::slave()
	{
		bool info;
		Solver_Seq solver(_problem, _setup);
		solver.run();
	
		opacket2 op;
		op << solver.best_solution() << solver.best_Mksp_solution();
		op << solver.independent_run_best_found() << solver.iteration_best_found();
		op << solver.time_best_found() << solver.worst_cost() << solver.nb_repetitions_best_solution ();
		
		op.send(0);
		
		ipacket2 ip(0);
		ip >> info;
		if (info)
		{
			opacket2 op_conf;
			op_conf << _setup << solver.current_iteration();
			op_conf.send(0);
		}
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

	void Solver_Lan_IR_DS::perform_one_independent_run ()
	{
	}

	void Solver_Lan_IR_DS::perform_one_phase ()
	{
	}

	void Solver_Lan_IR_DS::master()
	{	
		Solution sol_proc(_problem);
		Solution sol_Mksp_proc(sol_proc);
		int best_ir, best_it, nb_rep_best_sol, last_it;
		float best_time, gtime;
		double worst_cost;
		int sign = (_minimizing)?(1):(-1);

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
			ip >> sol_proc >> sol_Mksp_proc;
			ip >> best_ir >> best_it >> best_time >> worst_cost >> nb_rep_best_sol >> last_it;

			if (proc == 1 || sol_proc.is_better_than(_best_solution))
			{
				_best_solution = sol_proc;
				_best_cost = sol_proc.fitness();
				_independent_run_best_found = best_ir;				
				_iteration_best_found = best_it;
				_time_best_found = best_time;				
				_nb_repetitions_best_solution = nb_rep_best_sol;
				if (sign*worst_cost > sign*_worst_cost) 
					_worst_cost = worst_cost;
			}
			if (proc == 1 || improvement(sol_Mksp_proc, _best_Mksp_solution))
			{
				_best_Mksp_solution = sol_Mksp_proc;
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
		
		_best_solution = solver.best_solution();
		_best_Mksp_solution = solver.best_Mksp_solution();
		_best_cost = _best_solution.fitness();
		_independent_run_best_found = solver.independent_run_best_found();				
		_iteration_best_found = solver.iteration_best_found();
		_time_best_found = solver.time_best_found();				
		_nb_repetitions_best_solution = solver.nb_repetitions_best_solution ();
		_worst_cost = solver.worst_cost();
		
		/*Sending results to main Solver*/
		opacket2 op;
		op << solver.best_solution() << solver.best_Mksp_solution();
		op << solver.independent_run_best_found() << solver.iteration_best_found();
		op << solver.time_best_found() << solver.worst_cost();
		op << solver.nb_repetitions_best_solution () << solver.current_iteration();
		
		op.send(0);
	}
	bool ENERGY_CASE;
}
