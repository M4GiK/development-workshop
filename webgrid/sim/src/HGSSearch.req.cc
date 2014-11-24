/*
 * HGSSearch.req.cc
 *
 *  Created on: 2010-01-14
 *      Author: LukkoL
 */

#include "HGSSearch.hh"
#include "InitialSolutions.hh"
#include "Energy.hh"
#include "GAParams.hh"
#include <math.h>
#include <algorithm>
using namespace std;



bool compare_solutions (const HGSSearch::Solution* i,
			            const HGSSearch::Solution* j) {
	return i->is_better_than(*j);
}


skeleton HGSSearch {

	void start_ljfrsjfr (Population& pop);
	void start_mct (Population& pop);
	void start_random (Population& pop);
	void start_all (Population& pop);
	void start_minmin (Population& pop);
	void start_rc (Population& pop);
	void start_kpb (Population& pop);
	void start_all2 (Population& pop);
	Population select_random (const Population& pop);
	Population select_best (const Population& pop);
	Population select_linear_ranking (const Population& pop);
	Population select_exp_ranking (const Population& pop);
	Population select_binary_tournament (const Population& pop);
	Population select_n_tournament (const Population& pop);
	void mutate_move (const Solver& solver, Solution& sol);
	void mutate_swap (const Solver& solver, Solution& sol);
	void mutate_both (const Solver& solver, Solution& sol);
	void mutate_rebalancing (const Solver& solver, Solution& sol);

	void select_enviroment(Population& pop);
    /*********************************************************************/
    /* Problem                                                           */
    /*********************************************************************/
    Problem::Problem () {
        number_tasks    = 0;
        number_machines = 0;
        workload     =  vector<double> (0);
        mips         =  vector<double> (0);
        now          =  0.0;
        ready_times  =  vector<double> (0);
        etc          =  matrix<double> (0,0);
    }

    Problem::~Problem () {
    }

    ostream& operator<< (ostream& os, const Problem& pbm) {
        os << _repeat_ ("-",28) << " PROBLEM DEFINITION "
           << _repeat_ ("-",28) << endl;
        os << " . Number of tasks: "   << pbm.number_tasks << endl;
        os << " . Workload for each task: ";
        for (int i = 0; i < pbm.number_tasks; ++i) {
            os << pbm.workload[i] << " ";
        }   os << endl;

        os << " . Number of machines: " << pbm.number_machines << endl;
        os << " . Mips for each machine: ";
        for (int i = 0; i < pbm.number_machines; ++i) {
            os << pbm.mips[i] << " ";
        }   os << endl;

        os << " . Current simulation time: " << pbm.now << endl;
        os << " . Ready-time for each machine: ";
        for (int i = 0; i < pbm.number_machines; ++i) {
            os << pbm.ready_times[i] << " ";
        }   os << endl;

        os << _repeat_ ("-", 76) << endl;
        return os;
    }

    istream& operator>> (istream& is, Problem& pbm) {
        return is;
    }

    Direction Problem::direction () const {
        return Minimize;
    }

    void Problem::set_problem (const vector<double>& task_info,
                               const vector<double>& machine_info,
                               const vector<double>& ready_time_info,
                               const double          current_time) {
        assert ( (machine_info.size() == ready_time_info.size()) &&
                 (task_info.size()>0) && (machine_info.size()>0) &&
                 (now >= 0.0) );
        number_tasks    =  task_info.size();
        number_machines =  machine_info.size();
        workload        =  task_info;
        mips            =  machine_info;
        now             =  current_time;
        ready_times     =  ready_time_info;

        // This skeleton can be used independently from the simulator, and
        // thereby sometimes it may be necessary to fix the input data ...
        fix_ready_times_to_now();
        compute_etc();
    }

    void Problem::set_problem  (const matrix<double>& etc_info,
                                const vector<double>& ready_time_info,
                                const double          current_time) {
        assert ((etc_info.rows() > 0) && (etc_info.cols() > 0) &&
                (etc_info.cols() == (int) ready_time_info.size()) );


        etc				= etc_info;
        number_tasks    = etc_info.rows();
        number_machines = etc_info.cols();
        ready_times     = ready_time_info;
        now             = current_time;

        // This skeleton can be used independently from the simulator, and
        // thereby sometimes it may be necessary to fix the input data ...
        fix_ready_times_to_now();
    }

    void Problem::compute_etc ()  {
        etc = matrix<double> (number_tasks, number_machines);
        for (int i = 0; i < number_tasks; ++i) {
            for (int j = 0; j < number_machines; ++j) {
                etc[i][j] = workload[i]/mips[j];
            }
        }
    }

    void Problem::fix_ready_times_to_now () {
        for (int i = 0; i < number_machines; ++i) {
            if (ready_times[i] < now) ready_times[i] = now;
        }
    }

    /*********************************************************************/
    /* Solution                                                          */
    /*********************************************************************/
    typedef void (*MutateFunctionsType) (const Solver&, Solution&);

    Solution::Solution (const Problem& pbm) : problem (pbm)
    {
        schedule        = vector<int> (pbm.number_tasks, 0);
        completion      = pbm.ready_times;
        //total_makespan  = plus_infinity;
        local_makespan  = plus_infinity;
        //total_avg_utilization = 0.0;
        local_avg_utilization = 0.0;
        flowtime        = plus_infinity;
        energy			= plus_infinity;
        _feasible       = true;
        _fitness        = (pbm.direction()==Minimize) ? plus_infinity
                                                      : minus_infinity;
    }

    Solution::Solution (const Solution& sol) : problem(sol.problem) {
        schedule        = sol.schedule;
        completion      = sol.completion;
        //total_makespan  = sol.total_makespan;
        local_makespan  = sol.local_makespan;
        //total_avg_utilization = sol.total_avg_utilization;
        local_avg_utilization = sol.local_avg_utilization;
        flowtime        = sol.flowtime;
        energy			= sol.energy;
        _feasible       = sol._feasible;
        _fitness        = sol._fitness;
    }

    Solution::~Solution () {
    }

    Solution& Solution::operator= (const Solution& sol) {
        if (this != &sol) {
            schedule        = sol.schedule;
            completion      = sol.completion;
            //total_makespan  = sol.total_makespan;
            local_makespan  = sol.local_makespan;
            //total_avg_utilization = sol.total_avg_utilization;
            local_avg_utilization = sol.local_avg_utilization;
            flowtime        = sol.flowtime;
            energy			= sol.energy;
            _feasible       = sol._feasible;
            _fitness        = sol._fitness;
        }
        return *this;
    }

    bool Solution::is_better_than (const Solution& sol) const {
		int sign = ((problem.direction() == Minimize)?(1):(-1));

    	#ifdef MULTIOBJECTIVE
            /* Simultaneous:   min makespan + flowtime                */
			if      ((sign* _fitness) < (sign* sol._fitness))              return true;
			else if ((sign* _fitness) > (sign* sol._fitness))              return false;
			else if (HGS_ENERGY_CASE && ((sign* energy) < (sign* sol.energy))) return true;
			else                                                               return false;
    	#else
    	    /* Hierarchical:  min flowtime subject to min makespan    */
    	    /*               _fitness is that case refers to makespan */
			if      ((sign* _fitness) < (sign* sol._fitness)) return true;
			else if ((sign* _fitness) > (sign* sol._fitness)) return false;
			else if ((sign* flowtime) < (sign* sol.flowtime)) return true;
			else if ((sign* flowtime) > (sign* sol.flowtime)) return false;
			else if (HGS_ENERGY_CASE && ((sign* energy) < (sign* sol.energy))) return true;
			else                                                               return false;
    	#endif
	}

    bool operator== (const Solution& sol1, const Solution& sol2) {
        if((sol1.problem.number_tasks    != sol2.problem.number_tasks)   ||
           (sol1.problem.number_machines != sol2.problem.number_machines)||
           (sol1.fitness()               != sol2.fitness()) ) return false;
        for (int i = 0; i < sol1.problem.number_tasks; ++i) {
            if (sol1.schedule[i] != sol2.schedule[i])   return false;
        } return true;
    }

    bool operator!= (const Solution& sol1, const Solution& sol2) {
        return !(sol1==sol2);
    }

    ostream& operator<< (ostream& os, const Solution& sol) {
        os << _repeat_("-", 33) << " SOLUTION " <<_repeat_("-", 33) <<endl;
        os <<" . Number of tasks: "   << sol.problem.number_tasks   <<endl;
        os <<" . Number of machines: "<< sol.problem.number_machines<<endl;

        os << endl << " . Schedule: " << endl;
        for (int i = 0; i < sol.problem.number_tasks; ++i) {
            os << _repeat_(" ", 13);
            os << "[Task: " <<i<< " ,Host: "<< sol.schedule[i]<< "]"<<endl;
        }
        os << " . Makespan : " << sol.local_makespan << endl;
        os << " . Avg Utilization : " << sol.local_avg_utilization << endl;
        os << " . Flowtime : " << sol.flowtime << endl;
        os << " . Energy : " << sol.energy << endl;
        os << _repeat_("-", 76) << endl;
        return os;
    }

    istream& operator>> (istream& is, Solution& sol) {
        return is;
    }

    void Solution::set_initial () {
        heuristic(AdHocHeuristics::Random_h);
        update_feasible();
    }

    double Solution::fitness () const {
        return _fitness;
    }

    void Solution::update_fitness () {
        /* Compute machine completion times */
        completion = problem.ready_times;
        for (int i = 0; i < problem.number_tasks; ++i) {
            completion[schedule[i]]+= problem.etc[i][schedule[i]];
        }

        /* Transform schedule representation */
        matrix<int> schedule2;
        transform_schedule_representation (schedule2);

        /* Compute makespans, sum_completions and utilizations */
        //double sum_total_completion = 0;
        double sum_local_completion = 0;
        int    local_machines       = 0;

        //total_makespan  =  minus_infinity;
        local_makespan  =  minus_infinity;

        for (int i = 0; i < problem.number_machines; ++i) {
            //if (completion[i] > total_makespan) {
            //    total_makespan = completion[i];
            //}
            if (schedule2[i].size() > 0) {
                sum_local_completion += completion[i];
                local_machines++;
                if (completion[i] > local_makespan) {
                    local_makespan = completion[i];
            }   }
            //sum_total_completion += completion[i];
        }
        //total_avg_utilization = sum_total_completion / (total_makespan *
        //                        problem.number_machines);
        local_avg_utilization = sum_local_completion / (local_makespan *
                                local_machines);

        /* Compute minimum flowtime of the schedule (sorting by etc) */
        flowtime = 0;
        vector<double> etc_tasks;
        for (int i = 0; i < problem.number_machines; ++i) {
            etc_tasks.resize (schedule2[i].size());
            for (unsigned int j = 0; j < schedule2[i].size(); ++j) {
                etc_tasks[j] = problem.etc[schedule2[i][j]][i];
            }
            sort (etc_tasks.begin(), etc_tasks.end());

            double completion = problem.ready_times[i];
            for (unsigned int j = 0; j < etc_tasks.size(); ++j) {
                completion += etc_tasks[j];
                flowtime   += completion;
            }
        }

        if( HGS_ENERGY_CASE ) {
			Energy* tmp_energy = new Energy(problem.number_machines, problem.number_tasks);
			tmp_energy->assignEnergy(problem.mips,local_makespan,completion,problem.ready_times);
			if ( HGS_ENERGY_CASE == 1 )
				energy = tmp_energy->EnergyI;
			else if ( HGS_ENERGY_CASE == 2 )
				energy = tmp_energy->EnergyII;
			else
				energy = tmp_energy->EnergyI;
		}

        /* Compute machine completion time error sum from local makespan */
        //double avg_non_problem_completion  = 0;
        //int    non_problem_machines_number = 0;
		//double error_sum = 0;
        //for (int i = 0; i < problem.number_machines; ++i) {
        //	if ((schedule2[i].size()>0) && (completion[i]<local_makespan)){
		//		non_problem_machines_number++;
		//		avg_non_problem_completion += completion[i];
		//}	}
        //if (non_problem_machines_number > 0)
        //	avg_non_problem_completion /= non_problem_machines_number;
        //
        //for (int i = 0; i < problem.number_machines; ++i) {
        //	if ((schedule2[i].size()>0) && (completion[i]<local_makespan)){
        //		error_sum += abs(completion[i]-avg_non_problem_completion);
		//	}
        //}

        #ifdef MULTIOBJECTIVE
            /* Simultaneous optimization */
		    // TODO: �Use multipliers for both criterias? 0.7*mk + 0.3*fw
		    // TODO: �Use mean flowtime? (because nbr_machines may be huge)
		    _fitness = local_makespan ;
        #else
            /* Hierarchical optimization.Returns most important criteria */
            _fitness = local_makespan;
        #endif
    }

    bool Solution::feasible () const {
        return _feasible;
    }

    void Solution::update_feasible () {
    }

    void Solution::mutate (const Solver& solver) {
        static MutateFunctionsType MutateFunctions[] =
        {   &mutate_move,  &mutate_swap,  &mutate_both,
            &mutate_rebalancing
        };
        assert( (solver.setup().mutate_choice()>=0) &&
                (solver.setup().mutate_choice()<=3)    );
        if(solver.setup().mutation_size() > 0 && solver.setup().mutate_choice() == MutateSwap)
        {
        	mutate_swap_hgs(solver,*this);
        }
        else
        	MutateFunctions[solver.setup().mutate_choice()] (solver, *this);

        update_feasible();
        update_fitness();
    }

    /**************** INITIAL SOLUTIONS given possibilities **************/
    void Solution::heuristic (heuristic_type choice) {
        /* Use Initial Solutions */
        InitialSolutions is;
        is.set_problem(problem.workload,problem.mips,problem.ready_times);
        is.set_problem(problem.etc,problem.ready_times);
        schedule = is.heuristic(choice);

        update_feasible();
        update_fitness();
    }

    void Solution::perturbate_randomly () {
        for (int i = 0; i < problem.number_tasks; ++i) {
            if (rand_(0.15)) {
                schedule[i] = rand_(0, problem.number_machines - 1);
        }   }
        update_feasible();
        update_fitness();
    }

    /************************ Auxiliar functions *************************/
    void Solution::compute_most_loaded_machines (matrix<int>& sched,
                                                 vector<int>& machines) {
        for (int i = 0; i < problem.number_machines; ++i) {
            if ((completion[i] == local_makespan) && (sched[i].size() > 0))
                machines.push_back (i);
        }
        random_shuffle ( machines.begin() , machines.end() );
    }

    void Solution::transform_schedule_representation (matrix<int>& sched) {
        sched = matrix<int> (problem.number_machines, 0);
        for (int i = 0; i < problem.number_tasks; ++i) {
            sched[schedule[i]].push_back (i);
        }
    }

    void Solution::transform_schedule_representation (vector<int>& perm,
                                                      vector<int>& limits){
        matrix<int> sched;
        transform_schedule_representation (sched);

        perm   = vector<int> (problem.number_tasks, 0); int task_index = 0;
        limits = vector<int> (problem.number_machines, 0);
        for (int i = 0; i < problem.number_machines; ++i) {
        	// Be aware of: Maybe it is better not to random_shuffle them
        	//random_shuffle (sched[i].begin(), sched[i].end());
            for (unsigned int j = 0; j < sched[i].size(); ++j) {
                perm[task_index] = sched[i][j];
                ++task_index;
            }
            limits[i] = task_index;
        }
    }

    void Solution::apply_schedule_representation(const matrix<int>& sched){
        for (int i = 0; i < problem.number_machines; ++i) {
            for (unsigned int j = 0; j < sched[i].size(); ++j) {
                schedule[sched[i][j]] = i;
            }
        }
    }

    void Solution::apply_schedule_representation(const vector<int>& perm,
                                                const vector<int>& limits){
        int last_pos = 0;
        for (int i = 0; i < problem.number_machines; ++i) {
            for (int pos = last_pos; pos < limits[i]; ++pos) {
                schedule[perm[pos]] = i;
            }
            last_pos = limits[i];
        }
    }

    int Solution::select_task_from_another_machine (int machine,
                                                 const matrix<int>& sched){
        vector<int> machines;
        for (int i = 0; i < problem.number_machines; ++i) {
            if ( (i != machine) && (sched[i].size() > 0))
                 machines.push_back(i);
        }

        int task = -1;
        if (machines.size() > 0) {
            random_shuffle (machines.begin(), machines.end());
            int mach     = machines.front();
            int t        = rand_ (0, sched[mach].size() - 1);
            task         = sched[mach][t];
        }
        return task;
    }

    void Solution::get_shuffled_tasks (vector<int>& tasks) {
        tasks.resize(problem.number_tasks);
        for (int i = 0; i < problem.number_tasks; ++i) {
            tasks[i] = i;
        }
        random_shuffle (tasks.begin(), tasks.end());
    }

    void Solution::get_shuffled_machines (vector<int>& machines) {
        machines.resize(problem.number_machines);
        for (int i = 0; i < problem.number_machines; ++i) {
            machines[i] = i;
        }
        random_shuffle (machines.begin(), machines.end());
    }

    /*********************************************************************/
    /* Population: Make it more efficient. Tip: Solution copying, but be */
    /*             careful of shared pointers and delete operator.       */
    /*********************************************************************/
    typedef Population (*SelectFunctionsType) (const Population&);
    typedef void       (*StartFunctionsType)  (Population&);

    Population::Population (const Solver& solver)
    :   _solver(solver),
        _problem(solver.problem()),
        _setup(solver.setup()),
        _solutions(_setup.population_size()),
        _best_solution(NULL),
        _best_fitness(((_problem.direction()==Minimize)? plus_infinity :
                                                         minus_infinity )),
        _worst_fitness(-_best_fitness)
    {
        for (unsigned int i=0; i<_solutions.size(); ++i)
            _solutions[i] = new Solution(_problem);
    }

    Population::Population (const Solver& solver, const int size)
    :   _solver(solver),
        _problem(solver.problem()),
        _setup(solver.setup()),
        _solutions(size),
        _best_solution(NULL),
        _best_fitness(((_problem.direction()==Minimize)? plus_infinity :
                                                         minus_infinity )),
        _worst_fitness(-_best_fitness)
    {
        for (unsigned int i=0; i<_solutions.size(); ++i)
            _solutions[i] = new Solution(_problem);
    }


    Population::Population (const Population& pop)
    :   _solver(pop._solver),
        _problem(pop._problem),
        _setup(pop._setup)
    {
        _solutions.resize (pop.size());
        for (int i = 0; i < pop.size(); ++i) {
            _solutions[i]  = new Solution(_problem);
            *_solutions[i] = *pop._solutions[i];
        }
        _best_solution = pop._best_solution;
        _best_fitness = pop._best_fitness;
        _worst_fitness = pop._worst_fitness;

    }

    Population::~Population() {
        for (unsigned int i=0; i<_solutions.size(); ++i) {
            delete _solutions[i];
        }
        _solutions.resize(0);
    }

    Solution& Population::best_solution () const {
        return *_best_solution;
    }

    double Population::best_fitness () const {
        return _best_fitness;
    }

    double Population::worst_fitness () const {
        return _worst_fitness;
    }

    void Population::start () {
        static StartFunctionsType StartFunctions[] =
        {   &start_ljfrsjfr, &start_mct, &start_random, &start_all,
            &start_minmin, &start_rc, &start_kpb, &start_all2
        };
        assert((_setup.start_choice()>=0) && (_setup.start_choice()<=7));

        return StartFunctions[_setup.start_choice()] (*this);
    }

	void Population::update () {
        int sign  = ((_problem.direction()==Minimize)?(1):(-1));
        int size1 = size();
        if (size1 == 0) return;

        _best_solution = _solutions[0];
        _best_fitness  = _worst_fitness = _solutions[0]->fitness();

        for (int i = 1; i < size1; ++i) {
        	double f = _solutions[i]->fitness();
            if (_solutions[i]->is_better_than(*_best_solution) ) {
                _best_fitness  = f;
                _best_solution = _solutions[i];
            }
            if ((sign * f) > (sign * _worst_fitness)) {
                _worst_fitness = f;
            }
        }
    }

    void Population::sort ()  {
    	std::sort(_solutions.begin(), _solutions.end(), compare_solutions);
    }

    int Population::size () const {
        return _solutions.size();
    }

    void Population::keep_best (const int n) {
        int size1 = size();
        if (n <= 0) {
            for (int i = 0; i < size1; ++i) delete _solutions[i];
            _solutions.resize(0);
        } else if (n < size1) {
            for (int i = n; i < size1; ++i) {
                delete _solutions[i];
            }
            _solutions.resize (n);
            _worst_fitness = _solutions[n-1]->fitness();
        }
    }

    void Population::quit_worst (const int n) {
        keep_best(size() - n);
    }

    Population Population::select() const{
        static SelectFunctionsType SelectFunctions[] =
        {   &select_random, &select_best, &select_linear_ranking,
            &select_exp_ranking, &select_binary_tournament,
            &select_n_tournament
        };
        assert((_setup.select_choice()>=0) && (_setup.select_choice()<=5));
        return SelectFunctions[_setup.select_choice()] (*this);
    }

    void Population::cross () {
        random_shuffle (_solutions.begin(), _solutions.end());
        for (int i=0; i < size() / 2; ++i) {
            if (rand_(_setup.cross_probability()))
                crossover(_solver,*_solutions[2*i],*_solutions[2*i+1]);
        }
        update();
    }

    void Population::mutate () {
        for (int i=0; i<size(); ++i) {
            if (rand_(_setup.mutate_probability()))
                _solutions[i]->mutate(_solver);
        }
        update();
    }

    void Population::replace (Population& pop) {
        int size1 = size ();
        int size2 = pop.size ();

        if (size2 < size1) {
            if (_setup.replace_only_if_better() == false) {
                quit_worst (size2);
                *this += pop;
                sort();
            } else {
                *this += pop;
                sort();
                keep_best(size1);
            }
        } else {
            if (_setup.replace_generational() == false) {
                if (_setup.replace_only_if_better() == false) {
                    pop.sort();
                    pop.keep_best(size1);
                    *this = pop;
                } else {
                    *this += pop;
                    sort();
                    keep_best (size1);
                }
            } else {
                pop.sort();
                pop.keep_best (size1);
                *this = pop;
            }
        }
    }

    Population& Population::operator= (const Population& pop) {
        if (this != &pop) {
            int size1  = size();
            int size2 = pop.size();

            if (size2 == 0) {
                return *this;
            } else if (size1 >= size2) {
                for (int i = size2; i < size1; ++i) delete _solutions[i];
                _solutions.resize(size2);
            } else {
                _solutions.resize(size2);
                for (int i = size1; i < size2; ++i) {
                    _solutions[i] = new Solution(_problem);
                }
            }
            for (int i = 0; i < size2; ++i) {
                *_solutions[i] = *pop._solutions[i];
            }

            _best_solution = pop._best_solution;
            _best_fitness  = pop._best_fitness;
            _worst_fitness = pop._worst_fitness;
        }
        return *this;
    }

    Population& Population::operator+= (const Population& pop) {
        int size1 = size ();
        int size2 = pop.size ();
        if (size1 == 0) return (*this = pop);
        if (size2 == 0) return *this;

        int size  = size1 + size2;
        _solutions.resize(size);
        for (int i = size1; i < size; ++i) {
            _solutions[i]  = new Solution(_problem);
            *_solutions[i] = *pop._solutions[i-size1];
        }

        int sign = ((_problem.direction()==Minimize)?(1):(-1));
        if (pop._best_solution->is_better_than(*_best_solution)) {
            _best_fitness  = pop._best_fitness;
            _best_solution = pop._best_solution;
        }
        if ((sign * pop._worst_fitness) > (sign * _worst_fitness)) {
            _worst_fitness = pop._worst_fitness;
        }
        return *this;
    }

    ostream& operator<< (ostream& os, const Population& pop) {
        os << _repeat_("-",33) << "POPULATION" << _repeat_("-",33) << endl;
        os << "    Population size: " << pop.size() << " solutions"<< endl;
        for (int i = 0; i < pop.size(); ++i) {
            os << _repeat_(" ", 8) << "(solution " << i << ") : " << endl;
            os << *pop._solutions[i];
        }
        os << _repeat_ ("-", 76);
        return os;
    }

    istream& operator>> (istream& is, Population& pop) {
        //int size;
        //is >> size;
        //pop._solutions.resize (size);
        //
        //for (int i=0; i<size; ++i) {
        //    pop._solutions[i] = new Solution (pop._problem);
        //    is >> *pop._solutions[i];
        //}
        return is;
    }


    /************* STARTING POPULATION given possibilities ***************/
    void start_ljfrsjfr (Population& pop) {
        pop._solutions[0]->heuristic(AdHocHeuristics::LJFRSJFR_h);
        for (int i = 1; i < pop.size(); ++i) {
            pop._solutions[i]->heuristic(AdHocHeuristics::LJFRSJFR_h);
            pop._solutions[i]->perturbate_randomly();
        }
    }

    void start_mct (Population& pop) {
        pop._solutions[0]->heuristic(AdHocHeuristics::MCT_h);
        for (int i = 1; i < pop.size(); ++i) {
            pop._solutions[i]->heuristic(AdHocHeuristics::MCT_h);
            pop._solutions[i]->perturbate_randomly();
        }
    }

    void start_random (Population& pop) {
        for (int i = 0; i < pop.size(); ++i) {
            pop._solutions[i]->heuristic(AdHocHeuristics::Random_h);
        }
    }

    void start_all (Population& pop) {
        assert (pop.size() >= 2);
        pop._solutions[0]->heuristic(AdHocHeuristics::LJFRSJFR_h);
        pop._solutions[1]->heuristic(AdHocHeuristics::MCT_h);
        for (int i = 2; i < pop.size(); ++i) {
            pop._solutions[i]->heuristic(AdHocHeuristics::Random_h);
        }
    }

    void start_minmin (Population& pop) {
        pop._solutions[0]->heuristic(AdHocHeuristics::MinMin_h);
        for (int i = 1; i < pop.size(); ++i) {
            pop._solutions[i]->heuristic(AdHocHeuristics::MinMin_h);
            pop._solutions[i]->perturbate_randomly();
        }
    }

    void start_rc (Population& pop) {
        pop._solutions[0]->heuristic(AdHocHeuristics::RelativeCost_h);
        for (int i = 1; i < pop.size(); ++i) {
            pop._solutions[i]->heuristic(AdHocHeuristics::RelativeCost_h);
            pop._solutions[i]->perturbate_randomly();
        }
    }

    void start_kpb (Population& pop) {
        pop._solutions[0]->heuristic(AdHocHeuristics::KPB_h);
        for (int i = 1; i < pop.size(); ++i) {
            pop._solutions[i]->heuristic(AdHocHeuristics::KPB_h);
            pop._solutions[i]->perturbate_randomly();
        }
    }

    void start_all2 (Population& pop) {
        assert (pop.size() >= 5);
        pop._solutions[0]->heuristic(AdHocHeuristics::LJFRSJFR_h);
        pop._solutions[1]->heuristic(AdHocHeuristics::MCT_h);
        pop._solutions[2]->heuristic(AdHocHeuristics::MinMin_h);
        pop._solutions[3]->heuristic(AdHocHeuristics::RelativeCost_h);
        pop._solutions[4]->heuristic(AdHocHeuristics::KPB_h);
        for (int i = 5; i < pop.size(); ++i) {
            pop._solutions[i]->heuristic(AdHocHeuristics::Random_h);
        }
    }

    /***************** SELECT given possibilities ************************/

    Population select_random (const Population& pop) {
        int size = 2 * pop._setup.intermediate_population_size();
        Population interpop(pop._solver, size);

        for (int i=0; i<size; ++i) {
            int r = rand_ (0, pop._setup.population_size()-1);
            *interpop._solutions[i] = *pop._solutions[r];
        }
        return interpop;
    }

    Population select_best (const Population& pop) {
        int size = 2 * pop._setup.intermediate_population_size();
        Population interpop(pop._solver, size);

        for (int i=0; i<size; ++i) {
            int index = i % pop._setup.population_size();
            *interpop._solutions[i] = *pop._solutions[index];
        }
        return interpop;
    }

    Population select_linear_ranking (const Population& pop) {
        int size = 2 * pop._setup.intermediate_population_size();
        Population interpop(pop._solver, size);

        static vector<int> fitness_ranking;
        static int fitness_ranking_sum;

        if (pop._solver.current_iteration() == 0) {
            fitness_ranking.resize (pop._setup.population_size());
            fitness_ranking_sum = 0;
            for (int i = 0; i <pop._setup.population_size(); ++i) {
                fitness_ranking_sum += (int) (i+1);
                fitness_ranking[i] = fitness_ranking_sum +1;
            }
        }

        for (int i = 0; i<size; ++i) {
            int rnd    = rand_(1,fitness_ranking_sum);
            int index1 = binary_locate (fitness_ranking, rnd) + 1;
            int index2 = pop._setup.population_size()-1;
            *interpop._solutions[i] = *pop._solutions[index2-index1];
        }
        return interpop;
    }

    Population select_exp_ranking (const Population& pop) {
        int size = 2 * pop._setup.intermediate_population_size();
        Population interpop (pop._solver, size);

        static vector<int> fitness_ranking;
        static int fitness_ranking_sum;

        if (pop._solver.current_iteration() == 0) {
            fitness_ranking.resize (pop._setup.population_size());
            fitness_ranking_sum  = 0;
            for (int i=0; i<pop._setup.population_size(); ++i) {
                fitness_ranking_sum += (int) ceil(exp(i+1));
                fitness_ranking[i] = fitness_ranking_sum +1;
            }

            // select_extra_parameter refers in this case to the maximum
            // exponential ranking probability
            double normalize = pop._setup.select_extra_parameter()
                    * fitness_ranking[pop._setup.population_size()-1]
                    / exp(pop._setup.population_size());

            for (int i=0 ; i<pop._setup.population_size(); ++i) {
                fitness_ranking[i]= (int) ceil ( (double)normalize *
                                                 fitness_ranking[i]  );
            }
        }

        for (int i=0; i<size; ++i) {
            int rnd   = rand_(1,fitness_ranking_sum);
            int index1 = binary_locate (fitness_ranking, rnd) + 1;
            int index2 = pop._setup.population_size()-1;
            *interpop._solutions[i] = *pop._solutions[index2-index1];
        }

        return interpop;
    }

    Population select_binary_tournament (const Population& pop) {
        if (pop._setup.population_size() < 2)
            return select_best (pop);

        int size = 2 * pop._setup.intermediate_population_size();
        Population interpop(pop._solver,size);

        for (int i=0; i<size; ++i) {
            int one = rand_(0, pop._setup.population_size()-1);
            int two = rand_(0, pop._setup.population_size()-1);
            while (two==one) two= rand_(0, pop._setup.population_size()-1);

            int best, worst;
            if (pop._solutions[one]->is_better_than(*pop._solutions[two])){
                best  = one; worst = two;
            } else {
                best  = two;  worst = one;
            }

            // select_extra_parameter refers in this case  to the
            // tournament probability
            if (rand_(pop._setup.select_extra_parameter()))
                *interpop._solutions[i] = *pop._solutions[best];
            else
                *interpop._solutions[i] = *pop._solutions[worst];
        }
        return interpop;
    }

    Population select_n_tournament (const Population& pop)  {
        if ( pop._setup.population_size() <
             floor(pop._setup.select_extra_parameter())
           ) return select_best(pop);

        int size = 2 * pop._setup.intermediate_population_size();
        vector<int> tournament ((int)pop._setup.select_extra_parameter());
        Population interpop (pop._solver,size);

        for (int i=0; i<size; ++i) {
            // select_extra_parameter refers in this case to the
            // tournament size
            for (int k=0;k<floor(pop._setup.select_extra_parameter());++k){
                int ind = rand_(0,pop._setup.population_size()-1);
                int m   = 0;
                bool nequal = true;
                while ((m<k) && nequal) {
                    if (tournament[m] == ind) nequal = false;
                    ++m;
                }
                if (nequal) tournament[k] = ind;
                else --k;
            }

            int best = tournament[0];
            for (int k=1; k < pop._setup.select_extra_parameter(); ++k) {
                if ( pop._solutions[tournament[k]]->is_better_than (
                								 *pop._solutions[best]) ){
                    best = tournament[k];
                }
            }
            *interpop._solutions[i] = *pop._solutions[best];
        }
        return interpop;
    }

    /***************** CROSSOVER given possibilities *********************/

    void crossover_one_point (const Solver& solver, Solution& sol1,
                              Solution& sol2) {
        if (solver.problem().number_tasks == 1) return;

        int cut = rand_ (1, solver.problem().number_tasks - 1);

        for (int i = 0; i < cut; ++i) {
            int tmp = sol1.schedule[i];
            sol1.schedule[i] = sol2.schedule[i];
            sol2.schedule[i] = tmp;
        }
    }

    void crossover_two_points (const Solver& solver, Solution& sol1,
                              Solution& sol2) {
        if (solver.problem().number_tasks == 1) return;

        int cut1 = rand_ (0, solver.problem().number_tasks);
        int cut2 = rand_ (0, solver.problem().number_tasks);

        while ((abs(cut1-cut2) % solver.problem().number_tasks) == 0)
            cut2 = rand_ (0, solver.problem().number_tasks);

        int min = (cut1 < cut2) ? cut1 : cut2;
        int max = (cut1 > cut2) ? cut1 : cut2;

        for (int i = min; i < max; ++i) {
            int tmp = sol1.schedule[i];
            sol1.schedule[i] = sol2.schedule[i];
            sol2.schedule[i] = tmp;
        }
    }

    void crossover_uniform (const Solver& solver, Solution& sol1,
                            Solution& sol2) {
        for (int i = 0; i < solver.problem().number_tasks; ++i) {
            bool change = rand_(0.5);
            if (change) {
                int tmp = sol1.schedule[i];
                sol1.schedule[i] = sol2.schedule[i];
                sol2.schedule[i] = tmp;
            }
        }
    }

    void crossover_fitness_based (const Solver& solver, Solution& sol1,
                                  Solution& sol2) {
        /* Fitness function must be included in [0,+inf) or in (-inf,0] */
        double fitness1 = sol1.fitness();
        double fitness2 = sol2.fitness();

        double sum_fitness = abs(fitness1) + abs(fitness2);
        double p_bernoulli;

        if (sum_fitness == 0) {
            p_bernoulli = 0.5;
        } else {
            p_bernoulli = abs(fitness1) / sum_fitness;
            bool generous = rand_(solver.setup().cross_extra_parameter());
            if (generous) {
                if (p_bernoulli < (1 - p_bernoulli))
                    p_bernoulli =  1 - p_bernoulli;
            } else {
                if (p_bernoulli > (1 - p_bernoulli))
                    p_bernoulli =  1 - p_bernoulli;
            }
        }

        for (int i = 0; i < solver.problem().number_tasks; ++i) {
            bool change = rand_ (p_bernoulli);
            if (change) {
                int tmp = sol1.schedule[i];
                sol1.schedule[i] = sol2.schedule[i];
                sol2.schedule[i] = tmp;
            }
        }
    }

    void crossover_pmx (const Solver& solver, Solution& sol1,
    					Solution& sol2) {
		if (solver.problem().number_tasks == 1) return;

		vector<int> i_genes, i_limits;
	    vector<int> j_genes, j_limits;
	    sol1.transform_schedule_representation (i_genes, i_limits);
	    sol2.transform_schedule_representation (j_genes, j_limits);

	   	// CODE BEGINS HERE
	   	int number_tasks = solver.problem().number_tasks;
        int cut1 = rand_ (0, number_tasks);
        int cut2 = rand_ (0, number_tasks);
        while (cut2 == cut1) cut2 = rand_ (0, number_tasks);

        int min = (cut1<cut2) ? cut1 : cut2;
        int max = (cut1<cut2) ? cut2 : cut1;
        int temp;

        vector<int> inv_i(number_tasks);
        vector<int> inv_j(number_tasks);

        for (int k = 0; k<number_tasks; ++k) {
            inv_i[i_genes[k]] = k;
            inv_j[j_genes[k]] = k;
        }

        // swap i_genes[k] and j_genes[k] in and j
        for (int k = min; k<max; ++k) {
            temp = i_genes[k];
            i_genes[k] = j_genes[k];
            i_genes[inv_i[j_genes[k]]] = temp;
            j_genes[inv_j[temp]] = j_genes[k];
            j_genes[k] = temp;

            //inv_i.swap(i_genes[k],j_genes[k]);
            temp = inv_i[i_genes[k]];
            inv_i[i_genes[k]] = inv_i[j_genes[k]];
            inv_i[j_genes[k]] = temp;

            //inv_j.swap(i_genes[k],j_genes[k]);
            temp = inv_j[i_genes[k]];
            inv_j[i_genes[k]] = inv_j[j_genes[k]];
            inv_j[j_genes[k]] = temp;
        }
		// CODE ENDS HERE

	    sol1.apply_schedule_representation (i_genes, i_limits);
	    sol2.apply_schedule_representation (j_genes, j_limits);
	}

    void crossover_ox (const Solver& solver, Solution& sol1,
    					Solution& sol2) {
		if (solver.problem().number_tasks == 1) return;

		vector<int> i_genes, i_limits;
	    vector<int> j_genes, j_limits;
	    sol1.transform_schedule_representation (i_genes, i_limits);
	    sol2.transform_schedule_representation (j_genes, j_limits);

	    // CODE BEGINS HERE
	    int number_tasks = solver.problem().number_tasks;
        int cut1 = rand_ (0, number_tasks);
        int cut2 = rand_ (0, number_tasks);
        while ((abs(cut1-cut2) % number_tasks) == 0)
        	cut2 = rand_ (0, number_tasks);

        int min = (cut1<cut2) ? cut1 : cut2;
        int max = (cut1<cut2) ? cut2 : cut1;
        int index;

        vector<int> inv_i(number_tasks);
        vector<int> inv_j(number_tasks);

        for (int k = 0; k<number_tasks; ++k) {
            inv_i[i_genes[k]] = k;
            inv_j[j_genes[k]] = k;
        }

        // save cut zone
        vector<int> cut_i(max-min);
        vector<int> cut_j(max-min);

        for (int k = 0; k<max-min; ++k) {
            cut_i[k] = i_genes[min+k];
            cut_j[k] = j_genes[min+k];
        }

        // erase elements in i
        for (int k = min; k<max; ++k)
            i_genes[inv_i[cut_j[k-min]]] = -1;

        // empty between cuts in i
        index = 0;
        for (int k = 0; k<number_tasks; ++k) {
            if (i_genes[(max+k) % number_tasks] != -1) {
                inv_i[index] = i_genes[(max+k) % number_tasks];
                ++index;
            }
        }

        assert (index == number_tasks-max+min);
        for (int k = 0; k<number_tasks-max+min; ++k)
            i_genes[(max+k) % number_tasks] = inv_i[k];

        // write between cuts in i
        for (int k = min; k<max; ++k)
            i_genes[k] = cut_j[k-min];

        // erase elements in j
        for (int k = min; k<max; ++k)
            j_genes[inv_j[cut_i[k-min]]] = -1;

        // empty between cuts in j
        index = 0;
        for (int k = 0; k<number_tasks; ++k) {
            if (j_genes[(max+k) % number_tasks] != -1) {
                inv_j[index] = j_genes[(max+k) % number_tasks];
                ++index;
            }
        }

        assert (index == number_tasks-max+min);
        for (int k = 0; k<number_tasks-max+min; ++k)
            j_genes[(max+k) % number_tasks] = inv_j[k];

        // write between cuts in j
        for (int k = min; k<max; ++k)
            j_genes[k] = cut_i[k-min];
		// CODE ENDS HERE

	    sol1.apply_schedule_representation (i_genes, i_limits);
	    sol2.apply_schedule_representation (j_genes, j_limits);
	}

    void crossover_cx (const Solver& solver, Solution& sol1,
    					Solution& sol2) {
		vector<int> i_genes, i_limits;
	    vector<int> j_genes, j_limits;
	    sol1.transform_schedule_representation (i_genes, i_limits);
	    sol2.transform_schedule_representation (j_genes, j_limits);

	    // CODE BEGINS HERE
	    int number_tasks = solver.problem().number_tasks;
        vector<int> inv_i(number_tasks);
        vector<int> inv_j(number_tasks);

        for (int k = 0; k<number_tasks; ++k) {
            inv_i[i_genes[k]] = k;
            inv_j[j_genes[k]] = k;
        }

        // perform the cycle
        vector<int> cycle(number_tasks);
        cycle[0] = 0;
        int index = 0;
        while (inv_j[i_genes[cycle[index]]] != 0) {
            ++index;
            cycle[index] = inv_j[i_genes[cycle[index-1]]];
        }

        // write offsprings
        int temp;
        for (int k = 0; k<=index; ++k) {
            temp = i_genes[cycle[k]];
            i_genes[cycle[k]] = j_genes[cycle[k]];
            j_genes[cycle[k]] = temp;
        }
		// CODE ENDS HERE

	    sol1.apply_schedule_representation (i_genes, i_limits);
	    sol2.apply_schedule_representation (j_genes, j_limits);
	}

    /***************** MUTATE given possibilities ************************/

    void mutate_move (const Solver& solver, Solution& sol) {
        if (solver.problem().number_machines == 1) return;

        /* Randomly select a task from a machine and moves it to another */
        int task = rand_ (0, solver.problem().number_tasks - 1);
        int machine_src = sol.schedule[task];
        int machine_dst = rand_ (0, solver.problem().number_machines - 1);
        while (machine_dst == machine_src)
            machine_dst = rand_ (0, solver.problem().number_machines - 1);

        // Perform Move (task, machine_dst);
        sol.schedule[task] = machine_dst;
    }

    void mutate_swap (const Solver& solver, Solution& sol) {
        /* Transform schedule representation */
        matrix<int> schedule;
        sol.transform_schedule_representation (schedule);

        /* Randomly selects a task */
        int task1    = rand_ (0, solver.problem().number_tasks - 1);
        int machine = sol.schedule[task1];

        /* Randomly select a task from another machine, and swaps them */
        int task2= sol.select_task_from_another_machine (machine,schedule);
        if (task2 != -1) {
            // Perform Swap (task1, task2);
            int tmp = sol.schedule[task1];
            sol.schedule[task1] = sol.schedule[task2];
            sol.schedule[task2] = tmp;
        }
    }

    void mutate_both (const Solver& solver, Solution& sol) {
        bool move = rand_ (solver.setup().mutate_extra_parameter());
        if (move) {
            mutate_move (solver, sol);
        } else {
            mutate_swap (solver, sol);
        }
    }

    void mutate_rebalancing (const Solver& solver, Solution& sol) {
        /* ---- REBALANCE PROCESS ---- */
        bool found = false;

        // -- Transform schedule representation
        matrix<int> schedule;
        sol.transform_schedule_representation (schedule);

        // -- Randomly select a machine that is loaded the most
        vector<int> most_loaded_machines;
        sol.compute_most_loaded_machines (schedule, most_loaded_machines);
        int machine1 = most_loaded_machines.front();
        int machine2;

        // -- Randomly select a task (task2) from another machine
        int task1;
        int task2= sol.select_task_from_another_machine(machine1,schedule);

        if (task2 != -1) {
            // -- Randomly find a task in machine1 with more etc than task2
            // -- Moreover, whith probability mutate_extra_parameter(),
            // -- makespan for machine2 must be <= than the makespan
            machine2          = sol.schedule[task2];
            vector<int> tasks = schedule[machine1];
            random_shuffle (tasks.begin(), tasks.end());

            unsigned int i = 0;
            bool strong_b = rand_(solver.setup().mutate_extra_parameter());
            while ( (i < tasks.size()) && (!found) ) {
                task1 = tasks[i];
                if ( solver.problem().etc[task1][machine1]    >=
                     solver.problem().etc[task2][machine1] ) {
                    if (strong_b) {
                        double completion2 = sol.completion[machine2] -
                            solver.problem().etc[task2][machine2]     +
                            solver.problem().etc[task1][machine2];
                        if (completion2 <= sol.local_makespan) {
                            found = true;
                            int tmp = sol.schedule[task1];
                            sol.schedule[task1] = sol.schedule[task2];
                            sol.schedule[task2] = tmp;
                        }
                    } else {
                        found = true;
                        int tmp = sol.schedule[task1];
                        sol.schedule[task1] = sol.schedule[task2];
                        sol.schedule[task2] = tmp;
                }   }
                ++i;
        }   }
        if (found) return;

        // -- Transform schedule representation
        sol.transform_schedule_representation (schedule);

        // -- Randomly select a machine that is loaded the most
        sol.compute_most_loaded_machines (schedule, most_loaded_machines);
        machine1 = most_loaded_machines.front();

        // -- Compute tasks from machine1
        vector<int> tasks = schedule[machine1];
        random_shuffle (tasks.begin(), tasks.end());

        // -- Try to move a task (once) to reduce the completion time
        unsigned int i = 0; int j = 0;
        while ( (i < tasks.size()) && (!found) ) {
            task1 = tasks[i];  j = 0;
            while ( (j < sol.problem.number_machines) && (!found) ) {
                if (j != machine1) {
                    double completion2  =  sol.completion[j] +
                                           sol.problem.etc[task1][j];
                    if (completion2 <= sol.local_makespan) {
                        found = true;
                        // Perform Move (task1, j);
                        sol.schedule[task1] = j;
                    }
                }
                ++j;
            }
            ++i;
        }

        /* ---- IF REBALANCING FAILS ---- */
        if (!found || rand_ (0.15)) {
            if (rand_(0.50)) {
                mutate_swap (solver,sol);
            } else {
                mutate_move (solver, sol);
            }
        }
    }

    /*********************************************************************/
    /* External Methods                                                  */
    /*********************************************************************/
    typedef void (*CrossFunctionsType) (const Solver& s, Solution& i,
                                                         Solution& j);

    void crossover (const Solver& solver, Solution& sol1, Solution& sol2) {
        static CrossFunctionsType CrossFunctions[] =
        {  &crossover_one_point, &crossover_two_points, &crossover_uniform,
           &crossover_fitness_based, &crossover_pmx, &crossover_ox,
           &crossover_cx
        };
        assert ( (solver.setup().cross_choice()>=0) &&
                 (solver.setup().cross_choice()<=6)    );
        CrossFunctions[solver.setup().cross_choice()] (solver, sol1, sol2);

        sol1.update_feasible();
        sol2.update_feasible();
        sol1.update_fitness();
        sol2.update_fitness();
    }

    bool terminateQ (const Solver& solver) {
		return ( ( solver.current_iteration()      >=
				   solver.setup().nb_evolution_steps() )      ||
				 ((solver.setup().nb_repetitions_to_end()>0)&&
				  (solver.nb_repetitions_best_solution() >=
				   solver.setup().nb_repetitions_to_end()) )  ||
				 ( solver.time_spent() >=
				   solver.setup().max_time_to_spend())
			   );
    }
//*******************************************************************
//*******************************************************************

    typedef void (*EnviromentFunctionsType)(Population& pop, const Solution& best, double env_lenght);
    void select_enviroment_1(Population& pop, const Solution& best, double env_lenght);
    void select_enviroment_2(Population& pop, const Solution& best, double env_lenght);

    //***********************************************************************
    void mutate_swap_hgs (const Solver& solver, Solution& sol)
    {
    	/* Transform schedule representation */
    	matrix<int> schedule;
    	sol.transform_schedule_representation (schedule);

    	/* Randomly selects a task */
    	vector<int> used(solver.problem().number_tasks,0);
    	int task1;
    	for(int i=0;i<solver.setup().mutation_size(); i++)
    	{
    		do
    		{
    			task1 = rand_ (0, solver.problem().number_tasks - 1);
    		}while(used[task1]);

    		used[task1] = 1;
    		int machine = sol.schedule[task1];

    		/* Randomly select a task from another machine, and swaps them */
    		int task2= sol.select_task_from_another_machine (machine,schedule);
    		if (task2 != -1)
    		{
    			// Perform Swap (task1, task2);
    			int tmp = sol.schedule[task1];
    			sol.schedule[task1] = sol.schedule[task2];
    			sol.schedule[task2] = tmp;
    		}
    	}
    }
    //***********************************************************************
    vector<int> Solution::solution()
    {
    	return this->schedule;
    }
    //**************************************
    void Solution::set_initial(const Solution& best_solution, double env_length, int env_type)
    {
//    	InitialSolutions is;
//    	is.set_problem(problem.workload,problem.mips,problem.ready_times);
//    	is.set_problem(problem.etc,problem.ready_times);

    	schedule = best_solution.schedule;//rozwiązaniem początkowym jest punkt wypuszczenia gałęzi

    	update_feasible();
    	update_fitness();

    }
    //***************************************
    void Solution::enviroment(const Solution& best, double env_lenght, enviroment_type choise)
    {
    	InitialSolutions is;
    	is.set_problem(problem.workload, problem.mips, problem.ready_times);
    	is.set_problem(problem.etc, problem.ready_times);

    	schedule = is.enviroment(best.schedule, env_lenght, choise);
    	update_feasible();
    	update_fitness();
    }
    //************************************************************************
    void Population::start_epoch(const Solution& best_solution)
        {
        	static EnviromentFunctionsType env_select[]=
        	{
        		&select_enviroment_1, &select_enviroment_2
        	};

        	assert((_setup.enviroment_choice()>=0) && (_setup.enviroment_choice()<=1));
        	return env_select[_setup.enviroment_choice()](*this, best_solution ,_setup.enviroment_length());
        }
    //***********************************************************************
    void Population::mutate_hgs()
    {
    	for (int i=0; i<size(); ++i)
    	{
    		if(_solver.setup().mutate_choice() == MutateSwap)
    		{
    			if (rand_(_setup.mutate_probability()))
    				_solutions[i]->mutate(_solver);
    		}
    		else
    		{
    			for(int j=0;j<_setup.mutation_size();j++)
    			{
    				if (rand_(_setup.mutate_probability()))
    					_solutions[i]->mutate(_solver);
    			}
    		}
    		update();
    	}
    }
    //***********************************************************************
    void select_enviroment_1(Population& pop, const Solution& best, double env_lenght)
    {

    	pop._solutions[0] = new Solution(best);
    	for(int i=1;i<pop.size();++i)
    		pop._solutions[i]->enviroment(best, env_lenght, AdHocHeuristics::env_basic);
    }
    //**********************************************************************
    void select_enviroment_2(Population& pop, const Solution& best, double env_lenght)
    {
    	pop._solutions[0] = new Solution(best);
    	for(int i=1;i<pop.size();++i)
    		pop._solutions[i]->enviroment(best, env_lenght, AdHocHeuristics::env_minimalizing);
    }

    //**********************************************************************
    Population connect_populations(const Population& pop1, const Population& pop2)
    {
    	Population pop(pop1);//nie ma znaczenia czy pop1, pop2 -> ta sama gałąź, ustawienia są identyczne
    	for(int i=0;i<(int)pop._solutions.size();i++)
    	{
    		if(rand_() > 0.5)
    			pop._solutions[i] = new Solution(*pop1._solutions[i]);
    		else
    			pop._solutions[i] = new Solution(*pop2._solutions[i]);
    	}
    	return pop;
    }
    //**********************************************************************
}
