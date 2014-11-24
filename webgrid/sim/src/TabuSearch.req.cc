/*************************************************************************/
/* FILE: TabuSearch.req.cc                                               */
/*************************************************************************/
#include "TabuSearch.hh"
#include "Energy.hh"

#define LARGE_HASH        100000
#define MAX_TRIES              1
#define MIN_PERTURBATION    0.05
#define MAX_PERTURBATION    0.10
#define SCAPE_PERTURBATION  0.10
#define PENALTY             1.00
#define prec               -1e-9    

skeleton TabuSearch {

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
        hashing      =  matrix<int>    (0,0);
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
        compute_hash_values();
    }

    void Problem::set_problem  (const matrix<double>& etc_info,
                                const vector<double>& ready_time_info,
                                const double          current_time) {
        assert ((etc_info.rows() > 0) && (etc_info.cols() > 0) &&
                (etc_info.cols() == (int) ready_time_info.size()) );
        etc             = etc_info;
        number_tasks    = etc_info.rows();
        number_machines = etc_info.cols();
        ready_times     = ready_time_info;
        now             = current_time;

        // This skeleton can be used independently from the simulator, and
        // thereby sometimes it may be necessary to fix the input data ...
        fix_ready_times_to_now();
        compute_hash_values();
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

    void Problem::compute_hash_values () {
        hashing = matrix<int> (number_tasks, number_machines);
        for (int i = 0; i < number_tasks; ++i) {
            for (int j = 0; j < number_machines; ++j) {
                hashing[i][j] = rand_ (1, LARGE_HASH);
            }
        }
    }

    /*********************************************************************/
    /* Solution                                                          */
    /*********************************************************************/
    Solution::Solution (Problem& pbm) : problem( pbm ) {
        hash_id        = 0;
        schedule       = vector<int> (pbm.number_tasks, 0);
        positions_     = vector<int> (pbm.number_tasks, 0);
        schedule_      = matrix<int> (pbm.number_machines, 0, 0);
        completions    = pbm.ready_times;
        sum_local_completions = plus_infinity;
        nb_machines_involved  = pbm.number_machines;
        local_makespan        = plus_infinity;
        local_avg_utilization = 0.0;
        flowtimes      = vector<double>(pbm.number_machines,plus_infinity);
        flowtime       = plus_infinity;
        energy         = plus_infinity;
        //total_makespan  = plus_infinity;
        //total_avg_utilization = 0.0;
    }

    Solution::Solution (const Solution& sol) : problem(sol.problem) {
        hash_id         = sol.hash_id;
        schedule        = sol.schedule;
        positions_      = sol.positions_;
        schedule_       = sol.schedule_;
        completions     = sol.completions;
        sum_local_completions = sol.sum_local_completions;
        nb_machines_involved  = sol.nb_machines_involved;
        local_makespan        = sol.local_makespan;
        local_avg_utilization = sol.local_avg_utilization;
        flowtimes       = sol.flowtimes;
        flowtime        = sol.flowtime;
        energy          = sol.energy;
        //total_makespan  = sol.total_makespan;
        //total_avg_utilization = sol.total_avg_utilization;
    }

    Solution::~Solution () {
    }

    Solution& Solution::operator= (const Solution& sol) {
        if (this != &sol) {
            hash_id         = sol.hash_id;
            schedule        = sol.schedule;
            positions_      = sol.positions_;
            schedule_       = sol.schedule_;
            completions     = sol.completions;
            sum_local_completions = sol.sum_local_completions;
            nb_machines_involved  = sol.nb_machines_involved;
            local_makespan        = sol.local_makespan;
            local_avg_utilization = sol.local_avg_utilization;
            flowtimes       = sol.flowtimes;
            flowtime        = sol.flowtime;
            energy          = sol.energy;
            //total_makespan  = sol.total_makespan;
            //total_avg_utilization = sol.total_avg_utilization;
        }
        return *this;
    }

    bool Solution::is_better_than (const Solution& sol) const {        
        int sign = ((problem.direction() == Minimize)? (1): (-1));
        
        if (_MULTIOBJECTIVE_) {
          /* Multiobjective:   min makespan + flowtime              */
        	if      ((sign*fitness()-sign*sol.fitness())< prec){return true; }
        	else if ((sign*fitness()-sign*sol.fitness())> 0   ){return false;}
        	else if (_ENERGY_CASE_ && (((sign* energy)-(sign* sol.energy))< prec)) {return true; }
        	else                                                                 {return false;}
        } else {
          /* Hierarchical:  min flowtime subject to min makespan    */
          /*               _fitness in that case refers to makespan */
//       if      ((sign*fitness()) < (sign*sol.fitness())) {return true ;}
//       else if ((sign*fitness()) > (sign*sol.fitness())) {return false;}
//       else if ((sign*flowtime)  < (sign*sol.flowtime))  {return true ;}
//       else                                              {return false;}
         // Since variables are not recomputed from scratch, it exisits
         // the possibility of precision error in doubles .... :/
         if      ((sign*fitness()-sign*sol.fitness())< prec){return true; }
         else if ((sign*fitness()-sign*sol.fitness())> 0   ){return false;}
         else if ((sign*flowtime-sign*sol.flowtime)  < prec){return true ;}
         else if ((sign*flowtime-sign*sol.flowtime)  > 0   ){return false;}
		 else if (_ENERGY_CASE_ && (((sign* energy)-(sign* sol.energy))< prec)) {return true; }
		 else                                                                 {return false;}
        }
    }

    bool Solution::close_to (const Solution& sol) const {
        return (*this==sol);
    }

    bool operator== (const Solution& sol1, const Solution& sol2) {
        if (sol1.hash_id != sol2.hash_id)               return false;
        for (int i = 0; i < sol1.problem.number_tasks; ++i) {
            if (sol1.schedule[i] != sol2.schedule[i])   return false;
        }                                               return true;
    }

    bool operator!= (const Solution& sol1, const Solution& sol2) {
        return !(sol1==sol2);
    }

    ostream& operator<< (ostream& os, const Solution& sol) {
        os << _repeat_("-", 33) << " SOLUTION " <<_repeat_("-", 33)<< endl;
        os << " . Number of tasks: "    << sol.problem.number_tasks;
        os << " . Number of machines: " << sol.problem.number_machines;
        
        os << endl << " . Schedule: " << endl;
        for (int i = 0; i < sol.problem.number_tasks; ++i) {
            os << _repeat_(" ", 13);
            os << "[Task: " <<i << ", Host: "<<sol.schedule[i]<< "]"<<endl;
        }
        os << " . Makespan : " << sol.local_makespan << endl;
        os << " . Avg Utilization : " << sol.local_avg_utilization << endl;
        os << " . Flowtime : " << sol.flowtime << endl;
        os << " . Energy : " << sol.energy << endl;
        os << _repeat_("-", 76) << endl;
        
        for (int i = 0; i < sol.problem.number_tasks; ++i) {
            os << sol.schedule[i] << " ";
        }        
        return os;
    }

    istream& operator>> (istream& is, Solution& sol) {
        return is;
    }

    double Solution::fitness () const {
        if (_MULTIOBJECTIVE_) {
            return (0.75*local_makespan + 0.25*flowtime/problem.number_tasks);
        } else {
            return (local_makespan);
        }
    }

    void Solution::update_fitness () {
        compute_completion_times();
        compute_makespan();
        compute_utilization();
        compute_flowtime();
        compute_hash_id ();
        compute_energy ();
    }

    double Solution::delta (const Movement& move) const {
        /* Leave this if you don't use delta function */
        //double cost1 = fitness(); apply(mov);
        //double cost2 = fitness(); unapply(mov);
        //return (cost1 - cost2);
        return 0.0;
    }

    void Solution::set_initial (TabuStorage& ts, int start_choice) {
        /* Use Initial Solutions */
        InitialSolutions is;
        is.set_problem(problem.workload,problem.mips,problem.ready_times);
        is.set_problem(problem.etc,problem.ready_times);
        schedule = is.heuristic ((heuristic_type) start_choice);
        
        build_matrix_representation ();
        update_fitness();
        ts.initialize_starting_solution(*this);
    }

    void Solution::perturbate_randomly () {
        Movement move (problem, *this);
        for (int i = 0; i < problem.number_tasks; ++i) {
            move.generate(rand_(0.50)?swap:transfer);
            apply(move);
        }
    }

    void Solution::apply (const Movement& move) {
        //if (move.is_null()) { cout << "falla" << endl; return;}  
        if (move.type == swap) {
            int task1 = move.task1_to_swap;
            int task2 = move.task2_to_swap;
            int tmp         = schedule[task1];
            schedule[task1] = schedule[task2];
            schedule[task2] = tmp;
        } else {
            int task        = move.task_to_transfer;
            int machine_dst = move.machine_to_transfer;
            schedule[task]  = machine_dst;
        }
        recompute_completion_times (move);
        recompute_makespan         (move);
        recompute_utilization      (move);
        recompute_hash_id          (move);
        recompute_flowtime         (move);
    }

    void Solution::unapply (const Movement& move) {
        Movement move2 = move;
        move2.invert();
        apply(move2);
    }

    bool Solution::aspiration (const Movement& move,
                               const TabuStorage& tstore,
                               const Solver& solver, double& flow) const {
        /* Pre: �� move has NOT been applied to Solution sol !! */
        Solution sol = *this;
        sol.apply(move);
        flow = sol.flowtime;

        bool aspirates= sol.is_better_than(solver.current_best_solution());
        return (aspirates || tstore.aspiration (move, *this));
    }

    bool Solution::local_search (TabuStorage& ts, Solution& sol) {
        /* Perform this for Solution 'sol' */
        *this = sol;

        /* Auxiliar variables */
        Movement move(problem,*this); Solution prev= *this; bool end=false;

        /* Choose best move,and apply it until no improvement is possible*/
        while(choose_best_move (problem,*this,ts,ts._solver,move) && !end){
            apply (move);
            if (!is_better_than(prev)) { unapply (move); end = true; }
            else                       { prev = *this;               }
        }
        return (is_better_than (sol));
    }

    bool Solution::iterated_local_search (TabuStorage& ts, Solution& sol){
        /* Iteratively kicks the solution, nearly in the neighborhood.  */
        /* We hope that we will escape from local minimum,while keeping */
        /* good parts of the solution.                                  */

        int nbr_tasks = problem.number_tasks;
        int min_kicks = (int) ceil ( MIN_PERTURBATION * nbr_tasks );
        int max_kicks = (int) ceil ( MAX_PERTURBATION * nbr_tasks );
        bool improves = false;
        Solution previous = sol;

        for (int try_ = 0; (try_ < MAX_TRIES) && !improves; ++try_) {
            /* Perform this for Solution 'sol' */
            *this = previous;
            /* Kick the solution, nearly in the neighborhood.         */
            /* We hope that we will escape from local minimum, while  */
            /* keeping good parts of the solution.                    */
            int kicks = rand_ (min_kicks, max_kicks);
            for (int k = 0; k < kicks; ++k) {
                Movement move (problem, *this);
                move.generate ();
                apply(move);
            }
            /* And now, perform local_search */
            local_search (ts, *this);
            if (is_better_than(previous)) improves = true;            
        }
        if (!improves) *this = previous;
        return improves;
    }

    bool Solution::local_search_extended (TabuStorage& ts, Solution& sol) {
        /* Cannot do this if there are not at least 2 machines */
        int nbr_machs = problem.number_machines;
        if (nbr_machs < 2) return false;

        /* Perform this for Solution 'sol' */
        *this = sol;

        /* Auxiliar variables */
        bool found   = false, changed = false;
        int  iter    = 0;
        Movement move1 (problem, *this);
        Movement move2 (problem, *this);

        /* Try to improve while there are possible moves */
        do {
            int ma  = rand_ (0, nbr_machs - 1 );
            int mb  = rand_ (0, nbr_machs - 1 );
            while (mb == ma) mb = rand_ (0, nbr_machs - 1);
            int tasks_a= schedule_[ma].size(); double c_a= completions[ma];
            int tasks_b= schedule_[mb].size(); double c_b= completions[mb];
            double best_mk = max (c_a, c_b);
            found = false;  int best_t1= 0; int best_t2= 0; int best_t3= 0;

            for (int t1_ = 0; t1_ < tasks_a; ++t1_) {
                int t1 = schedule_[ma][t1_];
                for (int t2_ = t1_ + 1; t2_ < tasks_a; ++t2_) {
                    int t2 = schedule_[ma][t2_];
                    for (int t3_ = 0; t3_ < tasks_b; ++t3_) {
                        int t3 = schedule_[mb][t3_];
                        //-------- Begin of the swap + transfer test ------
                        double new_c_a = c_a - problem.etc[t1][ma] -
                                 problem.etc[t2][ma] + problem.etc[t3][ma];
                        double new_c_b = c_b - problem.etc[t3][mb] +
                                 problem.etc[t1][mb] + problem.etc[t2][mb];
                        double mk = (new_c_a > new_c_b)? new_c_a : new_c_b;

                        if (mk < best_mk) {
                            best_t1 = t1; best_t2 = t2; best_t3 = t3;
                            best_mk = mk; found = true;
                        }
                        //--------- End of the swap + transfer test -------
            }   }   }
            ++iter;
            if (found) {
                move1.assign_swap     ( best_t1, best_t3 ); apply(move1);
                move2.assign_transfer ( best_t2, mb      ); apply(move2);
                changed = true; iter = 0;
            }
        } while (found && (iter < MAX_TRIES) );
        return changed;
    }

    bool Solution::reward_by_neighborhood (TabuStorage& ts, Solution& sol){
        /* Change the strategy of neighborhood exploration, in order to  */
        /* make it in a different manner (varying exploration parameters)*/

        // Backup old parameters
        old_max_load_interval = ts._solver.setup().max_load_interval();
        old_pcnt_min_loaded   = ts._solver.setup().percent_min_load();

        // Performs this for Solution 'sol'
        *this = sol;

        // Compute (aproximate) original number of movements
        vector<int> m_, l_;
        compute_most_loaded_machines (m_, old_max_load_interval);
        compute_less_loaded_machines (l_, old_pcnt_min_loaded);
        int nbr_most_loaded     = m_.size(); m_.resize(0);
        int nbr_less_loaded     = l_.size(); l_.resize(0);
        double max_nbr_swaps     = ts._solver.setup().max_nb_swaps();
        double max_nbr_transfers = ts._solver.setup().max_nb_transfers();
        int nbr_tasks           = problem.number_tasks;
        int nbr_machs           = problem.number_machines;
        double avg_tasks        = double(nbr_tasks)/nbr_machs;
        double aprox_transfers_B= nbr_most_loaded*nbr_less_loaded*avg_tasks;
        double aprox_transfersB = min(max_nbr_transfers,aprox_transfers_B);
        double aprox_swaps_B    = aprox_transfers_B * avg_tasks;
        double aprox_swapsB     = min (max_nbr_swaps, aprox_swaps_B);
        int nbr_aprox_movesB    = (int)ceil(aprox_transfersB+aprox_swapsB);

        // Sort machines by completions
        vector<int> index (nbr_machs);
        for (int m = 0; m < nbr_machs; ++m) {
            index[m] = m;
        }   quick_sort (completions, index, 0, nbr_machs - 1);

        // Compute new limits for less_loaded and most_loaded_machines, by
        // mantaining approximately the same number of movements
        int l = 0, u = nbr_machs - 1;
        double aprox_transfers_A= (l+1) * (nbr_machs - u) * avg_tasks;
        double aprox_transfersA = min(max_nbr_transfers,aprox_transfers_A);
        double aprox_swaps_A    = aprox_transfers_A * avg_tasks;
        double aprox_swapsA     = min (max_nbr_swaps, aprox_swaps_A);
        int nbr_aprox_movesA    = (int)ceil(aprox_transfersA+aprox_swapsA);

        while ((nbr_aprox_movesA <= nbr_aprox_movesB) && (l <= u)) {
            ++l; --u;
            aprox_transfers_A = (l+1) * (nbr_machs - u) * avg_tasks;
            aprox_transfersA  = min (max_nbr_transfers,aprox_transfers_A);
            aprox_swaps_A     = aprox_transfers_A * avg_tasks;
            aprox_swapsA      = min (max_nbr_swaps, aprox_swaps_A);
            nbr_aprox_movesA  = (int)ceil(aprox_transfersA+aprox_swapsA);
        }

        if (nbr_aprox_movesA > nbr_aprox_movesB)   {--l; ++u;}
        if (l < 0) {l = 0;} if (u > (nbr_machs-1)) {u = nbr_machs - 1;}

        nbr_most_loaded = nbr_machs - u;
        nbr_less_loaded = l + 1;

        // Compute new max_load_interval and new pcnt_min_load
        double new_max_load_interval= completions[index[u]] /
                                      completions[index[nbr_machs -1]];
        double new_pcnt_min_loaded  = double (nbr_less_loaded) /
                                      double (nbr_machs);

       // Set new parameters
       ts._solver.setup().set_max_load_interval(new_max_load_interval);
       ts._solver.setup().set_percent_min_load (new_pcnt_min_loaded);

       return true;
    }

    bool Solution::unreward_by_neighborhood(TabuStorage& ts,Solution& sol){
        // Restore old parameters
        ts._solver.setup().set_max_load_interval(old_max_load_interval);
        ts._solver.setup().set_percent_min_load (old_pcnt_min_loaded);
        return false;
    }

    bool Solution::reward_by_complex_moves (TabuStorage& ts,Solution& sol){
        return local_search_extended (ts, sol);
    }

    bool Solution::unreward_by_complex_moves(TabuStorage& ts,Solution& sol){
        /* Nothing to do */
        return false;
    }

    bool Solution::reward_by_iterated_ls (TabuStorage& ts, Solution& sol) {
        return iterated_local_search (ts, sol);
    }

    bool Solution::unreward_by_iterated_ls (TabuStorage& ts,Solution& sol){
        /* Nothing to do */
        return false;
    }

    bool Solution::reward_by_elite_freqs (TabuStorage& ts, Solution& sol) {
        /* This method doesn't use heuristic value (like MMAS). A task  */
        /* map is assigned with probability pw by roulette wheel, or    */
        /* with probability 1-pw to its most frequent machine. (pw=0.70)*/
        
        Solution previous = sol;
               
        int nbr_tasks = problem.number_tasks;
        int nbr_machs = problem.number_machines;
        ts.compute_freqs_bests();
    
        for (int t = 0; t < nbr_tasks; ++t) {
            int task = t; int mach  = 0;
            bool max_or_wheel = (rand_(0.70)) ? true : false;
            if (max_or_wheel) { // Select most common mapping
                for (int m = 0; m < nbr_machs; ++m) {
                    if (ts.freqs_best[task][m]>=ts.freqs_best[task][mach]){
                        mach = m;
                }   }
            } else {    // Perform roulette wheel selection on freqs_best
                double rand_point= rand_();  double part_sum= 0;  int m= 0;
                do  {
                    part_sum += ts.freqs_best[task][m]; ++m;
                } while ((part_sum < rand_point) && (m != nbr_machs));
                mach = m - 1;
            }
    
            // Change current schedule, by means of a moves.
            Movement move (problem, *this);
            move.assign_transfer (task, mach);
            if (!move.is_null()) apply(move);
        }
        
        // Apply now local (like Graham Ritchie's ACO version)
        if (!_MULTIOBJECTIVE_) {
          local_search (ts, *this);
        }
        return is_better_than (previous);
    }

    bool Solution::unreward_by_elite_freqs (TabuStorage& ts,Solution& sol){
        /* Nothing to do */
        return false;
    }

    void Solution::reward (TabuStorage& ts) {
        // Static variables are shared among all Solution objects (inside
        // a method). This template is used to distinguish between 1st
        // reward activation, last reward activation, and the rest of
        // activations ...

        static bool first_reward = true;
        static int  last_reward  = 0;
        static int  curr_iter_rw = ts._solver.current_iteration();
        
        if (curr_iter_rw >= last_reward)
            first_reward = true;
        
        if (first_reward) {
            curr_iter_rw = ts._solver.current_iteration();
            last_reward  = curr_iter_rw + ts._solver.setup().\
                                          nb_intensifications();
            first_reward = false;
            // ----When reward occurs current_best_solution is critical----
              ts.update_best_solutions(ts._solver.current_best_solution());
              *this        = ts._solver.current_best_solution();
            // ------------------------------------------------------------
            reward_by_elite_freqs     (ts, *this); // ONLY ONCE
        } else {
            //----For rest of iterations, *this cannot monopolize elite----
              ts.update_best_solutions (*this, 2);
              *this        = ts._solver.current_best_solution();
            // ------------------------------------------------------------
        }
        ++curr_iter_rw;
        
        //reward_by_iterated_ls   (ts, *this);
        //reward_by_elite_freqs   (ts, *this); 
        reward_by_complex_moves   (ts, *this);
        reward_by_neighborhood    (ts, *this);
    }

    void Solution::unreward (TabuStorage& ts) {
        //unreward_by_iterated_ls    (ts, *this);
          unreward_by_elite_freqs    (ts, *this);
          unreward_by_complex_moves  (ts, *this);
          unreward_by_neighborhood   (ts, *this);
    }

    bool Solution::penalize_task_distribution (TabuStorage& ts,
                                               Solution& sol    ){
        /* Performs influential diversification based on task   */
        /* distributions among resources (Glover, H�bscher)     */
        
        // Perform this for Solution 'sol'
        *this = sol;
        
        int nbr_machs = problem.number_machines;
        vector<double>  distr (nbr_machs);
        vector<int>     index (nbr_machs);
        int last      = nbr_machs - 1;

        // Compute task etc distribution for each machine
        for (int m = 0; m < nbr_machs; ++m) {
            index[m] = m;
            double sum_etc = 0, sum_etc_squared = 0;
            for (unsigned int t_ = 0; t_ < schedule_[m].size(); ++t_) {
                int t            = schedule_[m][t_];
                sum_etc         += problem.etc[t][m];
                sum_etc_squared += pow(problem.etc[t][m], 2);
            }
            if (sum_etc> 0) {distr[m] = sum_etc_squared/ pow(sum_etc,2);}
            else {distr[m] = plus_infinity; --last; }
        }

        // Sorts by task distribution factor
        quick_sort (distr, index, 0, nbr_machs - 1);

        // Select one machine with low task distribution and
        // another machine with high task distribution
        if (last == 0) return false;
        int first_proc  = index[0];
        int second_proc = index[last];
        vector<int> procs (2); procs[0]= second_proc; procs[1]= first_proc;

        // Compute etc submatrix for these tasks and machines
        int nbr_left_machs = procs.size(), nbr_left_maps = 0;
        for (int m = 0; m < nbr_left_machs; ++m)
            nbr_left_maps += schedule_[procs[m]].size();

        vector<int>    tasks        (nbr_left_maps);
        matrix<double> etc_         (nbr_left_maps, nbr_left_machs);
        vector<double> completions_ (nbr_left_machs);
        int i = 0;
        for (int m_ = 0; m_ < nbr_left_machs; ++m_) {
            int m = procs[m_];
            completions_[m_] = problem.ready_times[m];
            for (int t_ = schedule_[m].size() - 1; t_ >= 0; --t_) {
                int t = schedule_[m][t_];
                tasks[i] = t;
                etc_[i][m_] = problem.etc[t][m];
                ++i;
            }
        }

        // Apply a specific heuristic, if there are tasks to schedule
        vector<int> rest_schedule;
        if (etc_.rows() > 0) {
            InitialSolutions is; is.set_problem (etc_, completions_);
            rest_schedule = is.mct();
        }

        // Restore current schedule, by means of moves
        Movement move (problem, *this);
        for (int t = 0; t < nbr_left_maps; ++t) {
            int task        = tasks[t];
            int new_machine = procs[rest_schedule[t]];
            move.assign_transfer (task, new_machine);
            if (!move.is_null()) {
                apply(move);
                ts.make_tabu (move, *this);
            }
        }

        // Apply now local_search!
        //local_search            (ts, *this);
        local_search_extended   (ts, *this);

        // Clears tabu short term memory
        //ts.clear();        
        return true;
    }

    bool Solution::unpenalize_task_distribution (TabuStorage& ts,
                                                 Solution& sol   ) {
        /* Nothing to do */
        return false;
    }

    //bool Solution::penalize_by_fitness_bias(TabuStorage& ts,Solution& sol){
    //    /* Bias the evaluation of possible moves towards more used moves */
    //
    //    int nbr_tasks = problem.number_tasks;
    //    int nbr_machs = problem.number_machines;
    //
    //    // Backup original etc matrix, if not done previously
    //    if (backup_etc.rows() == 0) {
    //        backup_etc = matrix<double> (nbr_tasks, nbr_machs, 0);
    //        for (int t = 0; t < nbr_tasks; ++t) {
    //            for (int m = 0; m < nbr_machs; ++m) {
    //                backup_etc[t][m] = problem.etc[t][m];
    //    }   }   }
    //
    //    // Change etc matrix, creating a bias against frequent moves
    //    ts.compute_freqs();    //percentage
    //    //ts.compute_ifreqs(); // B. Srivastava: PENALTY + ts.ifreqs[t][m]
    //    for (int t = 0; t < nbr_tasks; ++t) {
    //        for (int m = 0; m < nbr_machs; ++m) {
    //            problem.etc[t][m] *= ( PENALTY + ts.freqs[t][m] );
    //    }   }
    //
    //    // Clear tabu structure
    //    ts.clear();
    //
    //    // Recompute new variables
    //    update_fitness();
    //    return true;
    //}

    //bool Solution::unpenalize_by_fitness_bias (TabuStorage& ts,
    //                                           Solution& sol    ){
    //    // Restore original etc matrix
    //    int nbr_tasks = problem.number_tasks;
    //    int nbr_machs = problem.number_machines;
    //    for (int t = 0; t < nbr_tasks; ++t) {
    //        for (int m = 0; m < nbr_machs; ++m) {
    //            problem.etc[t][m] = backup_etc[t][m];
    //    }   }
    //
    //    // Recompute original variables
    //    update_fitness();        
    //    return false;
    //}

    bool Solution::penalize_by_dynamicity (TabuStorage& ts, Solution& sol){
        // Tasks that are moved most, are pinned to their best mapping. The
        // objective is to compensate the tasks that have been moved most,
        // by freezing their best mapping, and therefore, other tasks will
        // be taken into account, leading to new moves
        
        // Perform this for Solution 'sol'
        *this = sol;

        // Sort tasks by dynamacity
        ts.compute_tfreqs();
        ts.compute_freqs_bests ();
        int nbr_tasks = problem.number_tasks;
        int nbr_machs = problem.number_machines;        
        vector<int> index (nbr_tasks);
        for (int t = 0; t < nbr_tasks; ++t) index[t] = t;
        quick_sort (ts.tfreqs, index, 0, nbr_tasks - 1);
                
        // Clears tabu short memory structure
        //ts.clear();
        
        // Freezes most moved tasks
        int freed_tasks = (int) ceil(nbr_tasks * rand_ (5, 10)/100.0); 
        for (int i = nbr_tasks - 1; i >= (nbr_tasks-freed_tasks); --i) {
            int task = index[i];
            int mach = schedule[task];
            for (int m = 0; m < nbr_machs; ++m) {
                if (ts.freqs_best[task][m] > ts.freqs_best[task][mach]){
                    mach = m;
                }
                //if (problem.etc[task][m] < problem.etc[task][mach]) {
                //    mach = m;
                //}                
            }
            Movement move (problem, *this);
            move.assign_transfer (task, mach);
            // Freezes task mapping
            if (!move.is_null()) apply(move);
            ts.pin (task);
        }
                                        
        // Apply now local_search!
        //local_search          (ts, *this);
        local_search_extended (ts, *this);                                            
        
        return (is_better_than (sol));
    }    
    
    bool Solution::unpenalize_by_dynamicity(TabuStorage& ts,Solution& sol){
        /* Nothing to do */
        return false;
    }    
    
    //bool Solution::penalize_by_host_dynamicity (TabuStorage& ts,
    //                                            Solution& sol    ){
    //    /* Machines whose tasks are moved most, will become freezed.    */
    //    /* This means that their tasks will have their mapping pinned.  */
    //
    //    // Peform this for Solution 'sol'
    //    *this = sol;
    //
    //    // Sort resources by dynamacity
    //    ts.compute_hfreqs();
    //    int nbr_machs = problem.number_machines;
    //    vector<int> index (nbr_machs);
    //    for (int m = 0; m < nbr_machs; ++m) index[m] = m;
    //    quick_sort (ts.hfreqs, index, 0, nbr_machs - 1);
    //    
    //    // Clears tabu short memory structure
    //    // ts.clear();
    //    
    //    vector<int> most_loaded;
    //    double max_load_intvl = ts._solver.setup().max_load_interval();
    //    compute_most_loaded_machines (most_loaded,max_load_intvl);
    //                    
    //    int picked = 0; 
    //    int nbr_problematic = int(ceil(nbr_machs * rand_(15, 25)/100));
    //    for (int m_=nbr_machs-1; (m_>=0) && (picked<nbr_problematic);--m_){
    //        int m = index[m_];                        
    //        bool is_most_loaded = false;
    //        for (unsigned int i = 0; i < most_loaded.size(); ++i) {
    //            if (most_loaded[i] == m) is_most_loaded = true;
    //        }
    //                    
    //        if (!is_most_loaded || (most_loaded.size() > 1)) {
    //            /* Pin all of its tasks */
    //            for (unsigned int t_ = 0; t_ < schedule_[m].size(); ++t_) {
    //                int t = schedule_[m][t_];
    //                ts.pin(t);
    //            }
    //            picked++;                
    //        }            
    //    }
    //                                           
    //   // Apply now local_search!
    //   //local_search          (ts, *this);
    //   local_search_extended   (ts, *this);           
    //   return true; 
    //}
    
    //bool Solution::unpenalize_by_host_dynamicity (TabuStorage& ts,
    //                                              Solution& sol    ){
    //    /* Nothing to do */
    //    return false;
    //}     
    void Solution::penalize (TabuStorage& ts) {
        // Static variables are shared among all Solution objects (inside
        // a method). This template is used to distinguish between 1st
        // reward activation, last reward activation, and the rest of
        // activations ...

        static bool first_penalize = true;
        static int  last_penalize  = 0;
        static int  curr_iter_pl   = ts._solver.current_iteration();

        if (curr_iter_pl >= last_penalize)
            first_penalize = true;

        if (first_penalize) {
            curr_iter_pl   = ts._solver.current_iteration();
            last_penalize  = curr_iter_pl + ts._solver.setup().\
                                            nb_diversifications();
            first_penalize = false;
            //---When penalize occurs current_best_solution is critical----
              ts.update_best_solutions(ts._solver.current_best_solution());
              *this        = ts._solver.current_best_solution();
            // ------------------------------------------------------------
        } else {
            //----For rest of iterations, *this cannot monopolize elite----
              ts.update_best_solutions (*this, 2);
              *this        = ts._solver.current_best_solution();
            // ------------------------------------------------------------
        }
        ++curr_iter_pl;        
        
        //penalize_task_distribution  (ts, *this);
        //penalize_by_fitness_bias    (ts, *this);
          penalize_by_dynamicity      (ts, *this);
        //penalize_by_host_dynamicity (ts, *this);
    }

    void Solution::unpenalize (TabuStorage& ts) {
        //unpenalize_task_distribution  (ts, *this);
        //unpenalize_by_fitness_bias    (ts, *this);
          unpenalize_by_dynamicity      (ts, *this);
        //unpenalize_by_host_dynamicity (ts, *this);
    }      
           
    void Solution::escape (TabuStorage& ts) {
        /*When escape occurs current_best_solution is a critical solution*/
        ts.update_best_solutions (ts._solver.current_best_solution());

        Movement move (problem, *this);
        for (int i = 0; i < problem.number_tasks; ++i) {
            if (rand_(SCAPE_PERTURBATION)) {
                move.generate(rand_(0.90)?swap:transfer);
                apply(move);
        }   }
        //ts.update_best_solutions (*this, 1); //force!
    }

    int Solution::size() const {
        return problem.number_tasks;
    }

    item Solution::operator[] (int i_th) const {
        return schedule[i_th];
    }

    int Solution::hash() const {
        return hash_id;
    }

    /************************ Auxiliar functions *************************/

    void Solution::compute_most_loaded_machines (vector<int>& machines,
                                                 double interval) {
        vector<int> indices(problem.number_machines);
        for (int i = 0; i < problem.number_machines; ++i) indices[i] = i;
        quick_sort (completions, indices, 0, problem.number_machines - 1);

        for (int i = problem.number_machines - 1; i >= 0; --i) {
            if ((completions[indices[i]] >= (interval * local_makespan)) &&
                (host_participates(indices[i])) ) {
               machines.push_back(indices[i]);
            } else if (host_participates(indices[i])) {
                break;
            }
        }
        sort (machines.begin(), machines.end()); //Sort by machine id
        //reverse        (machines.begin(), machines.end());
        //random_shuffle (machines.begin(), machines.end());
    }

    void Solution::compute_less_loaded_machines (vector<int>& machines,
                                                 double percent) {
        int nb_less_loaded = (int)ceil (percent * problem.number_machines);
        nb_less_loaded     = (nb_less_loaded > 1) ? nb_less_loaded : 1;

        vector<int> indices (problem.number_machines);
        for (int i = 0; i < problem.number_machines; ++i) indices[i] = i;
        quick_sort (completions, indices, 0, problem.number_machines - 1);

        int i = 0, k = 0;
        while ( (i < problem.number_machines) && (k < nb_less_loaded) ) {
            if (host_participates(indices[i])) {
                machines.push_back (indices[i]);
                ++k;
            }
            ++i;
        }

        sort (machines.begin(), machines.end()); // Sort by machine id
        //reverse        (machines.begin(), machines.end());
        //random_shuffle (machines.begin(), machines.end());
    }

    void Solution::compute_random_machines (vector<int>& machines,
                                            double percent) {
        vector<int> indices (problem.number_machines);
        for (int i = 0; i < problem.number_machines; ++i) indices[i] = i;
        random_shuffle (indices.begin(), indices.end());

        int nb_random = (int)ceil (percent * problem.number_machines);
        nb_random     = (nb_random > 1) ? nb_random : 1;

        for (int i = 0; i < nb_random; ++i) machines.push_back(indices[i]);

        sort (machines.begin(), machines.end()); // Sort by machine id
    }

    inline bool Solution::host_participates (int host) {
        /* Pre: completions must be computed before */
        return ((completions[host]-problem.ready_times[host]) > 0);
    }

    void Solution::build_matrix_representation () {
        schedule_ = matrix<int> (problem.number_machines, 0);
        for (int i = 0; i < problem.number_tasks; ++i) {
            positions_[i] = schedule_[schedule[i]].size();
            schedule_[schedule[i]].push_back (i);
        }
    }

    void Solution::compute_completion_times () {
        completions = problem.ready_times;
        for (int i = 0; i < problem.number_tasks; ++i) {
            completions[schedule[i]] += problem.etc[i][schedule[i]];
        }
    }

    void Solution::compute_makespan () {
        /* Pre: completions must be computed before */
        local_makespan   = minus_infinity;
        //total_makespan = minus_infinity;
        for (int i = 0; i < problem.number_machines; ++i) {
            //if (completions[i] > total_makespan) {
            //    total_makespan = completions[i];
            //}
            if (host_participates(i) && (completions[i] > local_makespan)){
                local_makespan = completions[i];
            }
        }
    }

    void Solution::compute_utilization () {
        /* Pre: completions and makespan must be computed before */
        sum_local_completions = 0;
        nb_machines_involved  = 0;
        //double sum_total_completions = 0;

        for (int i = 0; i < problem.number_machines; ++i) {
            if (host_participates(i)) {
                sum_local_completions += completions[i];
                nb_machines_involved++;
            }
            //sum_total_completions += completions[i];
        }
        local_avg_utilization = sum_local_completions / (local_makespan *
                                nb_machines_involved);
        //total_avg_utilization = sum_total_completions / (total_makespan *
        //                        problem.number_machines);
    }

    void Solution::compute_flowtime () {
        flowtime = 0;
        for (int m = 0; m < problem.number_machines; ++m) {
            int size= schedule_[m].size();

            // --- Begin of Quick Sort (sort by etc) ---
            //vector<double> etc_    (size);
            //vector<double> index   (size);
            //vector<double> reindex (size);
            //for (int i = 0; i < size; ++i) {
            //    etc_ = problem.etc[schedule_[m][i]][m];
            //    index[i]   = i;
            //    reindex[i] = schedule_[m][i];
            //}
            //quick_sort (etc_, index, 0, size-1);
            //for (int i = 0; i < size; ++i) {
            //    schedule_[m][i] = reindex[index[i]];
            //    positions_[schedule_[m][i]] = i;
            //}
            // --- End of Quick Sort (sort by etc) ---

            // --- Begin of Insertion sort (sort by etc) ---
            for (int i = 1; i < size; ++i) {
                //this is: reorder_down(m,i);
                int task   = schedule_[m][i];
                double etc = problem.etc[task][m];
                int j      = i;
                for(;(j>0) && (problem.etc[schedule_[m][j-1]][m]>etc);--j){
                    schedule_[m][j] = schedule_[m][j-1];
                    positions_[schedule_[m][j]] = j;
                }
                schedule_[m][j]  = task;
                positions_[task] = j;
            }
            // --- End of Insertion Sort (sort by etc) ---

            flowtimes[m] = compute_flowtime_for (m);
            flowtime    += flowtimes[m];
        }
    }

    double Solution::compute_flowtime_for (int machine) {
        register unsigned int size  = schedule_[machine].size();
        register double flowtime_   = 0;
        register double completion_ = problem.ready_times[machine];
        register int    mach        = machine;
        for (register unsigned int i = 0; i < size; ++i) {
            completion_  += problem.etc[schedule_[mach][i]][mach];
            flowtime_    += completion_;
        }
        return flowtime_;
    }

    void Solution::compute_hash_id () {
        hash_id = 0;
        for (int i = 0; i < problem.number_tasks; ++i) {
            hash_id += problem.hashing[i][schedule[i]];
        }
    }

    void Solution::compute_energy() {
    	if( _ENERGY_CASE_ ) {
			Energy* tmp_energy = new Energy(problem.number_machines, problem.number_tasks);
			tmp_energy->assignEnergy(problem.mips,local_makespan,completions,problem.ready_times);
			if ( _ENERGY_CASE_ == 1 )
				energy = tmp_energy->EnergyI;
			else if ( _ENERGY_CASE_ == 2 )
				energy = tmp_energy->EnergyII;
			else
				energy = tmp_energy->EnergyI;
		}
    }

    void Solution::recompute_completion_times (const Movement& move) {
        /* Pre: schedule vector has hanged as a consequence of the move */
        if (move.type == swap) {
            int task1 = move.task1_to_swap;
            int task2 = move.task2_to_swap;
            int machine1 = schedule[task2];     //task1 old machine
            int machine2 = schedule[task1];     //task2 old machine

            completions[machine1] += - problem.etc[task1][machine1]
                                     + problem.etc[task2][machine1];
            completions[machine2] += - problem.etc[task2][machine2]
                                     + problem.etc[task1][machine2];
        } else {
            int task        = move.task_to_transfer;
            int machine_dst = move.machine_to_transfer;
            int machine_src = move.previous_machine;

            completions[machine_dst] +=  problem.etc[task][machine_dst];
            completions[machine_src] += -problem.etc[task][machine_src];
        }
    }

    void Solution::recompute_makespan (const Movement& move) {
        /* Pre: recompute_completions must have been computed */
        //TODO: Integrate pri_queue [an extended LEDA (:/) p_queue]
        compute_makespan();
    }

    void Solution::recompute_utilization (const Movement& move) {
        /* Pre: completions and makespan must have been recomputed */
        if (move.type == swap) {
            int task1 = move.task1_to_swap;
            int task2 = move.task2_to_swap;
            int machine1 = schedule[task2];     //task1 old machine
            int machine2 = schedule[task1];     //task2 old machine

            sum_local_completions +=  problem.etc[task2][machine1]
                                     -problem.etc[task1][machine1]
                                     +problem.etc[task1][machine2]
                                     -problem.etc[task2][machine2];

            local_avg_utilization = sum_local_completions /
                                   (local_makespan * nb_machines_involved);

            //sum_total_completions +=  problem.etc[task2][machine1]
            //                         -problem.etc[task1][machine1]
            //                         +problem.etc[task1][machine2]
            //                         -problem.etc[task2][machine2]
            //total_avg_utilization = sum_total_completions /
            //                  (total_makespan * problem.number_machines);

        } else {
            int task        = move.task_to_transfer;
            int machine_dst = move.machine_to_transfer;
            int machine_src = move.previous_machine;

            double dst_completion_b =  completions[machine_dst]
                                     - problem.etc[task][machine_dst];

            if ((dst_completion_b -problem.ready_times[machine_dst]) <= 0){
                ++nb_machines_involved;
                sum_local_completions += problem.ready_times[machine_dst];
            }
            if (!host_participates(machine_src)) {
                --nb_machines_involved;
                sum_local_completions += -problem.ready_times[machine_src];
            }

            sum_local_completions +=  problem.etc[task][machine_dst]
                                    - problem.etc[task][machine_src];

            local_avg_utilization = sum_local_completions /
                                   (local_makespan * nb_machines_involved);

            //sum_total_completions +=  problem.etc[task][machine_dst]
            //                        - problem.etc[task][machine_src];
            //total_avg_utilization = sum_total_completions /
            //                  (total_makespan * problem.number_machines);
        }
    }

    void Solution::recompute_flowtime (const Movement& move) {
        /* Pre: schedule vector has hanged as a consequence of the move */
        if (move.type == swap) {
            int task1    = move.task1_to_swap;
            int task2    = move.task2_to_swap;
            int machine1 = schedule[task2];     //task1 old machine
            int machine2 = schedule[task1];     //task2 old machine
            int position1= positions_[task1];   //task1 old position
            int position2= positions_[task2];   //task2 old position
            int tmp      = position1;

            schedule_[machine1][position1] = task2;
            schedule_[machine2][position2] = task1;
            positions_[task1] = positions_[task2];
            positions_[task2] = tmp;
            reorder (machine1, position1, move);
            reorder (machine2, position2, move);

            flowtime += -flowtimes[machine1] -flowtimes[machine2];
            flowtimes[machine1] = compute_flowtime_for (machine1);
            flowtimes[machine2] = compute_flowtime_for (machine2);
            flowtime += flowtimes[machine1] + flowtimes[machine2];

        } else {
            int task        = move.task_to_transfer;
            int machine_dst = move.machine_to_transfer;
            int machine_src = move.previous_machine;

            schedule_[machine_dst].push_back(task);
            reorder (machine_src,positions_[task], move);
            positions_[task] = schedule_[machine_dst].size()-1;
            reorder (machine_dst,positions_[task], move);

            flowtime += -flowtimes[machine_dst] -flowtimes[machine_src];
            flowtimes[machine_dst] = compute_flowtime_for (machine_dst);
            flowtimes[machine_src] = compute_flowtime_for (machine_src);
            flowtime += flowtimes[machine_dst] + flowtimes[machine_src];
        }
    }

    void Solution::recompute_hash_id (const Movement& move) {
        if (move.type == swap) {
            int task1 = move.task1_to_swap;
            int task2 = move.task2_to_swap;
            int machine1_b = schedule[task2]; //Task1 machine (before)
            int machine2_b = schedule[task1]; //Task2 machine (before)
            int machine1_a = machine2_b;      //Task1 machine (after)
            int machine2_a = machine1_b;      //Task2 machine (after)

            hash_id += - problem.hashing[task1][machine1_b]
                       + problem.hashing[task1][machine1_a]
                       - problem.hashing[task2][machine2_b]
                       + problem.hashing[task2][machine2_a];
        } else {
            int task        = move.task_to_transfer;
            int machine_dst = move.machine_to_transfer;
            int machine_src = move.previous_machine;

            hash_id += - problem.hashing[task][machine_src]
                       + problem.hashing[task][machine_dst];
        }
    }

    void Solution::reorder(int machine, int pos, const Movement& move){
        /* performs one insertion sort pass in the apropiate direction */
        register unsigned int position = pos;
        if (move.type == swap) {
            if ((position> 0) && ((position+1)<schedule_[machine].size())){
                if (problem.etc[schedule_[machine][position  ]][machine] <
                    problem.etc[schedule_[machine][position-1]][machine]) {
                    reorder_down (machine, position);
                } else {
                    reorder_up   (machine, position);
                }
            } else {
                if (position==0) reorder_up   (machine, position);
                else             reorder_down (machine, position);
            }
        } else {
            if (machine == move.machine_to_transfer) {
                reorder_down (machine, position);
            } else {
                bring_closer (machine, position);
            }
        }
    }

    void Solution::reorder_down (int machine, int position) {
                 int    task = schedule_[machine][position];
        register int    mach = machine;
        register double etc  = problem.etc[task][mach];
        register int    i    = position;
        for (;(i>0) && (problem.etc[schedule_[mach][i-1]][mach]> etc);--i){
           schedule_[mach][i] = schedule_[mach][i-1];
           positions_[schedule_[mach][i]] = i;
        }
        schedule_[mach][i] = task;
        positions_[task]   = i;
    }

    void Solution::reorder_up (int machine, int position) {
                 int    task = schedule_[machine][position];
        register int    mach = machine;
        register int    size = schedule_[mach].size()-1;
        register double etc  = problem.etc[task][mach];
        register int    i    = position;
        for (;(i<size)&&(etc>problem.etc[schedule_[mach][i+1]][mach]);++i){
            schedule_[mach][i] = schedule_[mach][i+1];
            positions_[schedule_[mach][i]] = i;
        }
        schedule_[mach][i] = task;
        positions_[task]   = i;
    }

    void Solution::bring_closer (int machine, int position) {
        register int mach = machine;
        register int size = schedule_[mach].size() - 1;
        register int i    = position;
        for (; i < size; ++i) {
            schedule_[mach][i] = schedule_[mach][i+1];
            positions_[schedule_[mach][i]] = i;
        }
        schedule_[mach].resize (size);
    }

    /*********************************************************************/
    /* Movement                                                          */
    /*********************************************************************/

    /* OJO: A Movement is fixed to a Solution */

    Movement::Movement (const Problem& pbm, Solution& sol)
    :   _problem(pbm),
        _solution(sol),
        _tabulife(0)
    {
        /* Null movement */
        type          = swap;
        task1_to_swap = 0;
        task2_to_swap = 0;
    }

    Movement::Movement (const Movement& move)
    :   _problem(move._problem),
        _solution(move._solution)
    {
        _tabulife           =  move._tabulife;
        type                =  move.type;
        task_to_transfer    =  move.task_to_transfer;
        machine_to_transfer =  move.machine_to_transfer;
        previous_machine    =  move.previous_machine;
        task1_to_swap       =  move.task1_to_swap;
        task2_to_swap       =  move.task2_to_swap;
    }

    Movement::~Movement ()
    {
    }

    Movement& Movement::operator= (const Movement& move) {
        if (this!=&move) {
            _tabulife           =  move._tabulife;
            type                =  move.type;
            task_to_transfer    =  move.task_to_transfer;
            machine_to_transfer =  move.machine_to_transfer;
            previous_machine    =  move.previous_machine;
            task1_to_swap       =  move.task1_to_swap;
            task2_to_swap       =  move.task2_to_swap;
        }
        return *this;
    }

    bool operator== (const Movement& move1, const Movement& move2) {
       if (move1.type == move2.type) {
            /* Don't care about tasks source machines */
         if (move1.type == transfer) {
            return ((move1.task_to_transfer==move2.task_to_transfer) &&
                   (move1.machine_to_transfer==move2.machine_to_transfer));
                   //&&(move1.previous_machine==move2.previous_machine)
         } else {
            return (( (move1.task1_to_swap == move2.task1_to_swap)&&
                      (move1.task2_to_swap == move2.task2_to_swap)  )||
                    ( (move1.task1_to_swap == move2.task1_to_swap)&&
                      (move1.task2_to_swap == move2.task2_to_swap)  ));
            }
       } else return false;
    }

    bool operator!= (const Movement& move1, const Movement& move2) {
        return !(move1==move2);
    }

    ostream& operator<< (ostream& os, const Movement& move) {
        os << _repeat_("-",33) << " MOVEMENT " << _repeat_("-",33) << endl;
        if (move.type == swap) {
            os << "swap tasks " << move.task1_to_swap << " and "
                                 << move.task2_to_swap << " . " << endl;
        } else {
            os << "move task " << move.task_to_transfer   << " to machine "
                               << move.machine_to_transfer << " . " <<endl;
        }
        os << _repeat_("-",76);
        return os;
    }

    istream& operator>> (istream& is, Movement& move) {
        return is;
    }

    void Movement::generate (move_type type_) {
        /* Randomly generate a Movement */
        if (type_ == swap) {  /* Generally it's better to swap */
            /* Swap movement */
            type = swap;
            task1_to_swap = rand_ (0, _problem.number_tasks-1);
            task2_to_swap = rand_ (0, _problem.number_tasks-1);
            while ( _solution.schedule[task2_to_swap] ==
                    _solution.schedule[task1_to_swap]  ){
                task2_to_swap  = rand_ (0, _problem.number_tasks-1);
            }
        } else {
            /* Transfer movement */
            type = transfer;
            task_to_transfer = rand_ (0, _problem.number_tasks-1);
            previous_machine = _solution.schedule[task_to_transfer];
            machine_to_transfer = rand_ (0, _problem.number_machines-1);
            while (machine_to_transfer == previous_machine){
                machine_to_transfer = rand_ (0,_problem.number_machines-1);
            }
        }
        //set_tabulife(0);
    }

    void Movement::invert () {
        if (type == swap) {
            /* Nothing to do */
        } else {
            /* To invert a transfer, the movement must have been applied */
            //assert (_solution.schedule[task_to_transfer] ==
            //        machine_to_transfer);
            int tmp             = machine_to_transfer;
            machine_to_transfer = previous_machine;
            previous_machine    = tmp;
        }
    }

    int Movement::tabulife () const {
        return _tabulife;
    }

    void Movement::set_tabulife (int i) {
        assert(i>=0);
        _tabulife = i;
    }

    void Movement::assign_transfer(int task, int machine, int tabulife) {
        //assert (_solution.schedule[task] != machine);
        type = transfer;
        task_to_transfer    = task;
        machine_to_transfer = machine;
        previous_machine    = _solution.schedule[task_to_transfer];
        //set_tabulife (tabulife);
    }

    void Movement::assign_swap (int task1, int task2, int tabulife) {
        //assert (_solution.schedule[task1] != _solution.schedule[task2]);
        type = swap;
        task1_to_swap  = task1;
        task2_to_swap  = task2;
        //set_tabulife (tabulife);
    }

    bool Movement::is_null () const {
        if ((_problem.number_machines== 0) || (_problem.number_tasks== 0)) 
            return true;
        if (type == swap) {
            return (_solution.schedule[task1_to_swap] ==
                    _solution.schedule[task2_to_swap]    );
        } else {
            return (_solution.schedule[task_to_transfer] ==
                    machine_to_transfer                     );
        }
    }


    /*********************************************************************/
    /* TabuStorage                                                       */
    /*********************************************************************/
    TabuStorage::TabuStorage (Solver& s)
    :   _solver(s)
    {
        tl = matrix<int> ( s.problem().number_tasks,
                           s.problem().number_machines,
                           (int) minus_infinity         );
        th = vector<bool> ( s.setup().tabu_size(), false );
        nbr_best_sols = 0;

              
        nbr_moves = 0;
        //frequency  = matrix<int> ( s.problem().number_tasks,
        //                           s.problem().number_machines, 0 );
        //ifrequency = matrix<int> ( s.problem().number_tasks,
        //                           s.problem().number_machines, 0 );
        //
        tfrequency = vector<int> ( s.problem().number_tasks, 0 );
        
        //hfrequency = vector<int> ( s.problem().number_machines, 0 );

        freqs_best = matrix<double> ( s.problem().number_tasks,
                                      s.problem().number_machines, 0 );
        //freqs      = matrix<double> ( s.problem().number_tasks,
        //                              s.problem().number_machines, 0 );                                        
        //ifreqs     = matrix<double> ( s.problem().number_tasks,
        //                              s.problem().number_machines, 0);                                      
        
        tfreqs     = vector<double> ( s.problem().number_tasks, 0);
        
        //hfreqs     = vector<double> ( s.problem().number_machines, 0);
    }

    TabuStorage::~TabuStorage () {
    }

    bool TabuStorage::is_in_tabu_storage (const Movement& move,
                                          const Solution& sol) const {
        return is_tabu (move, sol);
    }

    bool TabuStorage::is_tabu (const Movement& move,
                               const Solution& sol) const{
        /* Pre: �� move has NOT been applied to Solution sol !! */
        Solution sol2 = sol;
        //if (move.type == swap) {
        //    int task1 = move.task1_to_swap;
        //    int task2 = move.task2_to_swap;
        //    int tmp   = sol2.schedule[task1];
        //    sol2.schedule[task1] = sol2.schedule[task2];
        //    sol2.schedule[task2] = tmp;
        //} else {
        //    sol2.schedule[move.task_to_transfer]= move.machine_to_transfer;
        //}

        sol2.recompute_hash_id (move);
        if (th[sol2.hash()%th.size()]) return true;

        if (move.type == swap) {
            int task1     = move.task1_to_swap;
            int task2     = move.task2_to_swap;
            int machine1  = sol.schedule[task2]; //task1 new machine
            int machine2  = sol.schedule[task1]; //task2 new machine
            int iteration = _solver.current_iteration();
            int max_tabu  = _solver.setup().max_tabu_status();

            bool t1 = ((tl[task1][machine1] + max_tabu) >= iteration);
            bool t2 = ((tl[task2][machine2] + max_tabu) >= iteration);
            if (t1 && t2) return true;
        } else {
            int task        = move.task_to_transfer;
            int machine_dst = move.machine_to_transfer;
            int iteration   = _solver.current_iteration();
            int max_tabu    = _solver.setup().max_tabu_status();

            bool t1 = ((tl[task][machine_dst] + max_tabu) >= iteration);
            if (t1) return true;
        }
        return false;
    }

    bool TabuStorage::aspiration (const Movement& move, 
                                  const Solution& sol) const {
        /* Pre: �� move has NOT been applied to Solution sol !! */
        if (move.type == swap) {
            int task1     = move.task1_to_swap;
            int task2     = move.task2_to_swap;
            int machine1  = sol.schedule[task2]; //task1 new machine
            int machine2  = sol.schedule[task1]; //task2 new machine
            int iteration = _solver.current_iteration();
            
            int A     = _solver.setup().aspiration_value();
            int maxim = max(tl[task1][machine1]+A, tl[task2][machine2]+A);
            return    (maxim <= iteration);
        } else {
            int task        = move.task_to_transfer;
            int machine_dst = move.machine_to_transfer;
            int iteration   = _solver.current_iteration();
            
            int A           = _solver.setup().aspiration_value();
            return ((tl[task][machine_dst] + A) <= iteration);            
        }                                                                   
    }
                                
    void TabuStorage::make_tabu (Movement& move, const Solution& sol) {
        /*   Pre: �� move has been applied to Solution sol !!       */
        /*   (due to skeleton structure, it occurs always this way) */

        /* Update tabu structure */
        Movement move2 = move;   move2.invert();
        Solution sol2 = sol;     sol2.recompute_hash_id(move2);
        th[sol2.hash()%th.size()] = true;
        //th[sol.hash()%th.size()]  = true;

        if (move.type == swap) {
            int task1     = move.task1_to_swap;
            int task2     = move.task2_to_swap;
            int machine1  = sol.schedule[task2];     // task1 old machine
            int machine2  = sol.schedule[task1];     // task2 old machine
            int iteration = _solver.current_iteration();

            tl[task1][machine1] = iteration;
            tl[task2][machine2] = iteration;

            //int nbr_tasks = sol.schedule.size();
            //for (int t = 0; t < nbr_tasks; ++t) {
            //    ++frequency[t][sol.schedule[t]];
            //}
            //++ifrequency[task1][machine1];
            //++ifrequency[task2][machine2];
            ++tfrequency[task1];
            ++tfrequency[task2];
            //++hfrequency[machine1];
            //++hfrequency[machine2];
            nbr_moves += 2;
        } else {
            int task        = move.task_to_transfer;
            int machine_src = move.previous_machine;
            int iteration   = _solver.current_iteration();
            tl[task][machine_src] = iteration;

            //int nbr_tasks = sol.schedule.size();
            //for (int t = 0; t < nbr_tasks; ++t) {
            //    frequency[t][sol.schedule[t]]++;
            //}
            //++ifrequency[task][machine_src];
            //++hfrequency[machine_src];
            //++hfrequency[move.machine_to_transfer];
            ++tfrequency[task];
            ++nbr_moves;
        }
    }

    void TabuStorage::make_tabu_inv (Movement& move, const Solution& sol) {
        move.invert();
        make_tabu(move, sol);
    }

    void TabuStorage::update () {
    }

    int TabuStorage::size () const {
        return _solver.setup().tabu_size();
    }

    void TabuStorage::initialize_starting_solution (const Solution& sol) {
        //int nbr_tasks    = _solver.problem().number_tasks;
        //int nbr_machines = _solver.problem().number_machines;
        //
        //for (int t = 0; t < nbr_tasks; ++t) {
        //    for (int m = 0; m < nbr_machines; ++m) {
        //        frequency[t][m] = 0;
        //    }
        //    frequency[t][sol.schedule[t]] = 1;
        //}
        update_best_solutions (sol, 1); //Force to store sol
    }

    void TabuStorage::clear () {
        /* This clears tabu short term memory */
        for (int t = 0; t < tl.rows(); ++t) {
            for (int m = 0; m < tl.cols(); ++m) {
                tl[t][m] = (int) minus_infinity;
            }
        }
    }

    void TabuStorage::clear (int t) {
        /* This method frees all tabu restrictions about task t */
        int nbr_machines = _solver.problem().number_machines;
        for (int m = 0; m < nbr_machines; ++m) {
            tl[t][m] = (int) minus_infinity;
        }
    }

    void TabuStorage::clear (int t, int m) {
        /* This clears a specific attribute from tabu short term memory */
        tl[t][m] = (int) minus_infinity;
    }

    void TabuStorage::pin (int t) {
        /* t can't return back to any machine for some iters. */
        int nbr_machines = _solver.problem().number_machines;
        for (int m = 0; m < nbr_machines; ++m) {
            tl[t][m] = _solver.current_iteration();
        }
    }

    void TabuStorage::update_best_solutions (const Solution& sol,
                                             int option) {
        // Options are: 0 => default, 1 => always saves sol (force),
        //              2 => replaces last solution with sol if better

        /* Update tabu long-term memory structure (critical events) */
        if (nbr_best_sols > 0) {
            if (option == 1) {
                // Force sol to be in elite set of Solution
                if (nbr_best_sols == _solver.setup().elite_size()) {
                    best_sols.pop_front(); --nbr_best_sols;
                }
                best_sols.push_back(sol);
                ++nbr_best_sols;
                //best_sols[0] = sol;
            } else if (option == 2) {
                //Store sol in the last used position, replacing the last
                //best solution, only if it's better (avoid convergence)
                if (sol.is_better_than(best_sols[nbr_best_sols-1]) ) {
                    best_sols[nbr_best_sols-1] = sol;
                }
            } else {
                // Default behaviour, sol will be in elite set, if it's
                // better than last best solution
                if (sol.is_better_than(best_sols[nbr_best_sols-1]) ) {
                    if (nbr_best_sols == _solver.setup().elite_size()) {
                        best_sols.pop_front(); --nbr_best_sols;
                    }
                    best_sols.push_back(sol);
                    ++nbr_best_sols;
                }
            }
        } else {
            best_sols.push_back(sol);
            ++nbr_best_sols;
        }
    }

    void TabuStorage::compute_freqs_bests () {
        int nbr_tasks = _solver.problem().number_tasks;
        int nbr_machs = _solver.problem().number_machines;

        for (int t = 0; t < nbr_tasks; ++t) {
            for (int m = 0; m < nbr_machs; ++m) {
                freqs_best[t][m] = 0;
        }   }

        for (int s = 0; s < nbr_best_sols; ++s) {
            vector<int>& sch = best_sols[s].schedule;
            for (int t = 0; t < nbr_tasks; ++t) {
                ++freqs_best[t][sch[t]];
        }   }

        for (int t = 0; t < nbr_tasks; ++t) {
            for (int m = 0; m < nbr_machs; ++m) {
               if (nbr_best_sols > 0)
                 freqs_best[t][m] = double(freqs_best[t][m])/nbr_best_sols;
               else
                 freqs_best[t][m] = 0;
        }   }
    }

    //void TabuStorage::compute_freqs () {
    //    int nbr_tasks = _solver.problem().number_tasks;
    //    int nbr_machs = _solver.problem().number_machines;
    //
    //    for (int t = 0; t < nbr_tasks; ++t) {
    //        double sum_freqs = 0;
    //        for (int m = 0; m < nbr_machs; ++m) {
    //            freqs[t][m] = frequency[t][m];
    //            sum_freqs  += freqs[t][m];
    //        }
    //        for (int m = 0; m < nbr_machs; ++m) {
    //            if (sum_freqs > 0) freqs[t][m] = freqs[t][m] / sum_freqs;
    //            else               freqs[t][m] = 0;
    //            
    //    }   }
    //}
    
    //void TabuStorage::compute_ifreqs () {
    //    int nbr_tasks = _solver.problem().number_tasks;
    //    int nbr_machs = _solver.problem().number_machines;
    //    
    //    for (int t = 0; t < nbr_tasks; ++t) {
    //        double sum_ifreqs = 0;
    //        for (int m = 0; m < nbr_machs; ++m) {
    //            ifreqs[t][m] = ifrequency[t][m];
    //            sum_ifreqs  += ifreqs[t][m];
    //        }
    //        for (int m = 0; m < nbr_machs; ++m) {
    //            if (sum_ifreqs > 0) ifreqs[t][m] = ifreqs[t][m]/sum_ifreqs;
    //            else                ifreqs[t][m] = 0;
    //        }
    //    }
    //}
    
    void TabuStorage::compute_tfreqs () {
        int nbr_tasks = _solver.problem().number_tasks;
                
        for (int t = 0; t < nbr_tasks; ++t) {
            if (nbr_moves > 0) tfreqs[t] = tfrequency[t]/double(nbr_moves);
            else               tfreqs[t] = 0;
        }        
    }    
    
    //void TabuStorage::compute_hfreqs () {
    //    int nbr_machs = _solver.problem().number_machines;
    //    
    //    double sum_hfreqs = 0;
    //    for (int m = 0; m < nbr_machs; ++m) {
    //        hfreqs[m]   = hfrequency[m];
    //        sum_hfreqs += hfreqs[m];            
    //    }        
    //    
    //    for (int m = 0; m < nbr_machs; ++m) {
    //        if (sum_hfreqs > 0)     hfreqs[m] = hfreqs[m] / sum_hfreqs;
    //        else                    hfreqs[m] = 0;
    //    }        
    //}    

    /*********************************************************************/
    /* External Methods                                                  */
    /*********************************************************************/
    bool choose_best_move (const Problem& pbm, const Solution& sol,
                           const TabuStorage& ts, const Solver& solver,
                           Movement& move) {
        if (_MULTIOBJECTIVE_) {
            return choose_best_move_S (pbm,sol,ts,solver,move);
        } else {
            return choose_best_move_H (pbm,sol,ts,solver,move);
        }
    }

    /*----choose_best_move FOR HIERARCHICAL MULTICRITERIA OPTIMIZATION---*/
    bool choose_best_move_H (const Problem& pbm, const Solution& sol,
                             const TabuStorage& ts, const Solver& solver,
                             Movement& move) {
        /* Variables to store the current best neighbor (and solution) */
        double best_mk = plus_infinity; //steepest descent + mildest ascent
        double best_fw = plus_infinity; //steepest descent + mildest ascent
        bool   found   = false;
        bool   swap    = false;
        int    task1 = 0, task2 = 0, machine_dst = 0;

        /* Auxiliar variables */
        Solution curr_sol        = sol;
        Movement curr_move       = move;
        const matrix<int>& sched = sol.schedule_;

        /* To speed the exploration of the neighborhood, we only select  */
        /* a subset of all the machines, and try to transfer and swap    */
        /* tasks, from machines mostly loaded to machines least loaded.  */
        /* It is common to set most_loaded_machines with the most loaded */
        /* machine, and less_loaded_machines with all the machines.      */
        /* We ASSUME that the vectors most/less_loaded_machines are both */
        /* sorted by machine id, in order to ease the check of duplicated*/
        /* swaps, since two machines may be present in both vectors.     */

        /* Compute most loaded machines and less loaded machines */
        vector<int> most_loaded, less_loaded;
        double max_load_intvl = solver.setup().max_load_interval();
        curr_sol.compute_most_loaded_machines (most_loaded,max_load_intvl);
        double min_load_prcnt = solver.setup().percent_min_load();
        curr_sol.compute_less_loaded_machines (less_loaded,min_load_prcnt);

        /* Initialize dictionaries */
        vector<bool> most_loaded_d (pbm.number_machines, false);
        vector<bool> less_loaded_d (pbm.number_machines, false);
        for (unsigned int i = 0; i < most_loaded.size(); ++i)
            most_loaded_d[most_loaded[i]] = true;
        for (unsigned int i = 0; i < less_loaded.size(); ++i)
            less_loaded_d[less_loaded[i]] = true;

        /* Initialize maximum number of movements, to speed the search  */
        /* by reducing the neighborhood size. By default, max_nb_swaps  */
        /* and max_nb_transfers are both initialized by plus_infinite   */
        float max_nb_swaps     = solver.setup().max_nb_swaps ();
        float max_nb_transfers = solver.setup().max_nb_transfers ();
        int   nb_swaps         = 0;
        int   nb_transfers     = 0;

        /* ------------------ Try to transfer -------------------------- */
        for (unsigned int m1_ = 0; m1_ < most_loaded.size(); ++m1_) {
           int m1 = most_loaded[m1_];
           for (unsigned int t1_ = 0; t1_ < sched[m1].size(); ++t1_) {
               int t1 = sched[m1][t1_];
               for (unsigned int m2_ = 0; m2_ < less_loaded.size(); ++m2_){
                    int m2 = less_loaded[m2_];
                    if (m2 != m1) {
                        if ((++nb_transfers)> max_nb_transfers) goto swapp;
                        // --------- Begin of the transfer test ---------
                        double completion1 = curr_sol.completions[m1];
                        double completion2 = curr_sol.completions[m2];
                        completion1 += -pbm.etc[t1][m1];
                        completion2 +=  pbm.etc[t1][m2];
                        double mk = (completion1 > completion2) ?
                                     completion1 : completion2  ;
                        if ((mk <= best_mk)) {// && (mk <= m_makespan) ) {
                            curr_move.assign_transfer (t1, m2);
                            double fw = plus_infinity;
                            if (!ts.is_tabu (curr_move, sol) ||
                                 sol.aspiration(curr_move, ts,solver, fw)){
                                if ((mk < best_mk) || (fw < best_fw)) {
                                   best_mk = mk; best_fw = fw; found= true;
                                   swap= false; task1= t1; machine_dst= m2;
                                }
                            }
                        }
                        // --------- End of the transfer test ----------
                    }
               }
           }
        }
        /* -------------------- Try to swap ---------------------------- */
        swapp:
        for (unsigned int m1_ = 0; m1_ < most_loaded.size(); ++m1_) {
           int m1 = most_loaded[m1_];
           for (unsigned int t1_ = 0; t1_ < sched[m1].size(); ++t1_) {
              int t1 = sched[m1][t1_];
              for (unsigned int m2_ = 0; m2_ < less_loaded.size(); ++m2_) {
                int m2 = less_loaded[m2_];
                if ((!most_loaded_d[m2])||(!less_loaded_d[m1])||(m1 < m2)){
                    for (unsigned int t2_=0; t2_< sched[m2].size(); ++t2_){
                      if ((++nb_swaps) > max_nb_swaps) goto end;
                      // --------- Begin of the swap test ---------
                      int t2 = sched[m2][t2_];
                      double completion1 = curr_sol.completions[m1];
                      double completion2 = curr_sol.completions[m2];
                      completion1 += -pbm.etc[t1][m1] + pbm.etc[t2][m1];
                      completion2 += -pbm.etc[t2][m2] + pbm.etc[t1][m2];
                      double mk = (completion1 > completion2) ?
                                   completion1 : completion2  ;
                      if ((mk <= best_mk)) {// && (mk <= m_makespan)) {
                         curr_move.assign_swap (t1, t2);
                         double fw = plus_infinity;
                         if (!ts.is_tabu (curr_move, sol) ||
                              sol.aspiration(curr_move, ts, solver, fw) ) {
                            if ((mk < best_mk) || (fw < best_fw)) {
                                best_mk = mk; best_fw = fw; found = true;
                                swap = true; task1 = t1; task2 = t2;
                            }
                         }
                      }
                      // --------- End of the swap test ----------
                   }
                }
              }
           }
        }
        end:
        if (found && swap ) move.assign_swap (task1, task2);
        if (found && !swap) move.assign_transfer (task1, machine_dst);
        return found;
    }

    /*----choose_best_move FOR SIMULTANEOUS MULTICRITERIA OPTIMIZATION---*/
    bool choose_best_move_S (const Problem& pbm, const Solution& sol,
                           const TabuStorage& ts, const Solver& solver,
                           Movement& move) {
        
        /* Variables to store the current best neighbor (and solution) */
        bool   found   = false;
        bool   swap    = false;
        int    task1 = 0, task2 = 0, machine_dst = 0;

        /* Auxiliar variables */
        //steepest descent + mildest ascent
        double best_fitness      = plus_infinity;
        Solution curr_sol        = sol;
        Movement curr_move       = move;
        const matrix<int>& sched = sol.schedule_;

        /* To speed the exploration of the neighborhood, we only select  */
        /* a subset of all the machines, and try to transfer and swap    */
        /* tasks, from machines mostly loaded to machines least loaded.  */
        /* It is common to set most_loaded_machines with the most loaded */
        /* machine, and less_loaded_machines with all the machines.      */
        /* We ASSUME that the vectors most/less_loaded_machines are both */
        /* sorted by machine id, in order to ease the check of duplicated*/
        /* swaps, since two machines may be present in both vectors.     */

        /* Compute most loaded machines and less loaded machines */
        vector<int> most_loaded, less_loaded;
        double max_load_intvl = solver.setup().max_load_interval();
        curr_sol.compute_most_loaded_machines (most_loaded,max_load_intvl);
        double min_load_prcnt = solver.setup().percent_min_load();
        curr_sol.compute_less_loaded_machines (less_loaded,min_load_prcnt);

        /* Initialize dictionaries */
        vector<bool> most_loaded_d (pbm.number_machines, false);
        vector<bool> less_loaded_d (pbm.number_machines, false);
        for (unsigned int i = 0; i < most_loaded.size(); ++i)
            most_loaded_d[most_loaded[i]] = true;
        for (unsigned int i = 0; i < less_loaded.size(); ++i)
            less_loaded_d[less_loaded[i]] = true;

        /* Initialize maximum number of movements, to speed the search  */
        /* by reducing the neighborhood size. By default, max_nb_swaps  */
        /* and max_nb_transfers are both initialized by plus_infinite   */
        float max_nb_swaps     = solver.setup().max_nb_swaps ();
        float max_nb_transfers = solver.setup().max_nb_transfers ();
        int   nb_swaps         = 0;
        int   nb_transfers     = 0;

        /* ------------------ Try to transfer -------------------------- */
        for (unsigned int m1_ = 0; m1_ < most_loaded.size(); ++m1_) {
           int m1 = most_loaded[m1_];
           for (unsigned int t1_ = 0; t1_ < sched[m1].size(); ++t1_) {
               int t1 = sched[m1][t1_];
               for (unsigned int m2_ = 0; m2_ < less_loaded.size(); ++m2_){
                  int m2 = less_loaded[m2_];
                  if (m2 != m1) {
                      if ((++nb_transfers)> max_nb_transfers) goto swapp;
                      // ------- Begin of the transfer test -------
                      curr_move.assign_transfer (t1, m2);
                      //double fw;
                      if (!ts.is_tabu(curr_move, curr_sol)){//||
                         //curr_sol.aspiration(curr_move, ts, solver, fw)){
                         curr_sol.apply (curr_move);
                         if (curr_sol.fitness() < best_fitness ) {
                             best_fitness = curr_sol.fitness(); found=true;
                             swap = false; task1 = t1; machine_dst = m2;
                         }
                         curr_sol.unapply (curr_move);
                      }
                      // -------- End of the transfer test --------
                  }
               }
           }
        }
        /* -------------------- Try to swap ---------------------------- */
        swapp:
        for (unsigned int m1_ = 0; m1_ < most_loaded.size(); ++m1_) {
           int m1 = most_loaded[m1_];
           for (unsigned int t1_ = 0; t1_ < sched[m1].size(); ++t1_) {
              int t1 = sched[m1][t1_];
              for (unsigned int m2_ = 0; m2_ < less_loaded.size(); ++m2_) {
                int m2 = less_loaded[m2_];
                if ((!most_loaded_d[m2])||(!less_loaded_d[m1])||(m1 < m2)){
                    for (unsigned int t2_=0; t2_< sched[m2].size(); ++t2_){
                      if ((++nb_swaps) > max_nb_swaps) goto end;
                      // --------- Begin of the swap test ---------
                      int t2 = sched[m2][t2_];
                      curr_move.assign_swap (t1, t2);
                      //double fw;
                      if (!ts.is_tabu(curr_move, curr_sol)){ //||
                         //curr_sol.aspiration(curr_move, ts, solver, fw)){
                        curr_sol.apply (curr_move);
                        if (curr_sol.fitness() < best_fitness ) {
                            best_fitness = curr_sol.fitness(); found=true;
                            swap = true; task1 = t1; task2 = t2;
                        }
                        curr_sol.unapply (curr_move);
                      }
                      // --------- End of the swap test -----------
                   }
                }
              }
           }
        }
        end:
        if (found && swap ) move.assign_swap (task1, task2);
        if (found && !swap) move.assign_transfer (task1, machine_dst);
        return found;
    }

    bool choose_best_move_from_partition (const Problem& pbm,
        const Solution& sol, const vector<item> partition,
        const TabuStorage& ts, const Solver& solver, Movement& move) {
        return false;
    }


    bool terminateQ (const Solver& solver) {
        return ( ( solver.current_iteration()  >=
                   solver.setup().nb_iterations() )    ||
                 ( solver.time_spent() >=
                   solver.setup().max_time_to_spend())
               );
    }
}
