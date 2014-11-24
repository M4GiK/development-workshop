#include "InitialSolutions.hh"

namespace AdHocHeuristics {

	/**
	 * Initialize class, compute etc and ready fields
	 */
    void InitialSolutions::set_problem (const vector<double>& workload_,
                                        const vector<double>& mips_,
                                        const vector<double>& ready_){
        number_tasks    =  workload_.size();
        number_machines =  mips_.size();
        ready           =  ready_;
        workload        =  workload_;
        mips            =  mips_;
        etc             = matrix<double> (number_tasks, number_machines);
        compute_etc (workload_, mips_);
    }

    /**
     * Initialize class, just store etc and ready fields
     */
    void InitialSolutions::set_problem (const matrix<double>& etc_,
                                        const vector<double>& ready_) {
        number_tasks     =  etc_.rows();
        number_machines  =  etc_.cols();
        etc   = etc_;
        ready = ready_;
    }

    /**
     * Invoke whatever heuristic from this centralized method
     */
    vector<int> InitialSolutions::heuristic (heuristic_type choice) {
        /* Pre: set_problem has been called previously */

    	/*
    	 * TODO: add game_min_min, game_min_min_v2 etc.
    	 */
        switch (choice) {
            case MinMin_h:          return min_min();                break;
            case MaxMin_h:          return max_min();                break;
            case Sufferage_h:       return sufferage();              break;
            case RelativeCost_h:    return relative_cost();          break;
            case LJFRSJFR_h:        return ljfr_sjfr();              break;
            case MCT_h:             return mct();                    break;
            case MET_h:             return met();                    break;
            case SA_h:              return sa();                     break;
            case KPB_h:             return kpb();                    break;
            case OLB_h:             return olb();                    break;
            case Random_h:          return random();                 break;
        }
        return random();
    }

    /**
     * Standard min_min,
     * @returns Some sub-optimal schedule.
     */
    vector<int> InitialSolutions::min_min () {
        vector<int>     schedule       (number_tasks);
        matrix<double>  completions    (number_tasks, number_machines);
        vector<int>     min_completion (number_tasks);
        set<int>        tasks;

        /* Initialization */
        for (int t = 0; t < number_tasks; ++t) {
            tasks.insert (t);
            for (int m = 0; m < number_machines; ++m) {
                completions[t][m] = ready[m] + etc[t][m];
        }   }

        while (!tasks.empty()) {
            /* Compute for each task, the machine which gives the
               earliest completion time for the task */
            set<int>::iterator it;
            for (it = tasks.begin(); it != tasks.end(); ++it){
                int task = *it; int min_m = 0;
                for (int m = 0; m < number_machines; ++m) {
                    if (completions[task][m] < completions[task][min_m]) {
                        min_m = m;
                }   }
                min_completion[task] = min_m;
            }

            /* Find the task that has the earliest completion time */
            it  = tasks.begin();
            int min_task = *it;
            for (it = tasks.begin(); it != tasks.end(); ++it){
                int task = *it;
                if (completions[task][min_completion[task]] <
                    completions[min_task][min_completion[min_task]]) {
                    min_task = task;
            }   }

            /* Assign best task to its best machine */
            int min_machine = min_completion[min_task];
            schedule[min_task] = min_machine;

            /* Delete best task from the set of unassigned tasks */
            tasks.erase (min_task);

            /* Update completions */
            for (it = tasks.begin(); it != tasks.end(); ++it) {
                completions[*it][min_machine]+=etc[min_task][min_machine];
        }   }
        return schedule;
    }

    /**
     * Modified min_min,
     *
     * @returns Schedule with optimized allocation of tasks for given compartment
     */
    vector<int> InitialSolutions::game_min_min_old_broken ( vector<int>& original,
    		int cmp_start, int cmp_end ) {

    	vector<int>     schedule       (number_tasks);
		matrix<double>  completions    (number_tasks, number_machines);
		vector<int>     min_completion (number_tasks);
		set<int>        tasks;
		set<int>		machines;

		for ( int t = cmp_start; t <= cmp_end; t++ ){
			machines.insert(original[t]);
		}

		/* Initialization */
		for (int t = cmp_start; t <= cmp_end; ++t) {
			tasks.insert (t);
			set<int>::iterator im;
			for (im = machines.begin(); im != machines.end(); im++) {
				int m = *im;
				completions[t][m] = ready[m] + etc[t][m];
			}
		}

		while (!tasks.empty()) {
			/* Compute for each task, the machine which gives the
			   earliest completion time for the task */
			set<int>::iterator it;
			for (it = tasks.begin(); it != tasks.end(); ++it){
				int task = *it; int min_m = 0;
				set<int>::iterator im;
				for (im = machines.begin(); im != machines.end(); im++) {
					int m = *im;
					if (completions[task][m] < completions[task][min_m]) {
						min_m = m;
					}
				}
				min_completion[task] = min_m;
			}

			/* Find the task that has the earliest completion time */
			it  = tasks.begin();
			int min_task = *it;
			for (it = tasks.begin(); it != tasks.end(); ++it){
				int task = *it;
				if (completions[task][min_completion[task]] <
					completions[min_task][min_completion[min_task]]) {
					min_task = task;
				}
			}

			/* Assign best task to its best machine */
			int min_machine = min_completion[min_task];
			schedule[min_task] = min_machine;
			original[min_task] = min_machine;

			/* Delete best task from the set of unassigned tasks */
			tasks.erase (min_task);

			/* Update completions */
			for (it = tasks.begin(); it != tasks.end(); ++it) {
				completions[*it][min_machine]+=etc[min_task][min_machine];
			}
		}
		return schedule;

	}

    /**
	 * Modified min_min,
	 *
	 * @returns Schedule with optimized allocation of tasks for given compartment
	 */
	vector<int> InitialSolutions::game_min_min_fixed ( vector<int>& original,
			int cmp_start, int cmp_end ) {

		static vector<int>     schedule       (number_tasks);
		static matrix<double>  completions    (number_tasks, number_machines);
		static vector<int>     min_completion (number_tasks);
		set<int>        tasks;
		static vector<double>  initial_completions; // ready_with_other_tasks


		initial_completions = ready;


		for ( int t = 0; t < number_tasks; t++){
			schedule[t] = original[t];
			min_completion[t] = 0;
			for ( int m = 0; m < number_machines; m++ ){
				completions[t][m] = 0;
			}
		}

		/* Init ready vector */
		for (int t = 0; t < number_tasks; ++t) {
			if( t < cmp_start || cmp_end < t ){
				// Not our task
				int machine_nr = original[t];
				initial_completions[machine_nr] += etc[t][machine_nr];
			}
		}

		/* Initialization */
		for (int t = cmp_start; t <= cmp_end; ++t) {
			tasks.insert (t);
			for (int m = 0; m < number_machines; ++m) {
				completions[t][m] = initial_completions[m] + etc[t][m];
		}   }

		while (!tasks.empty()) {
			/* Compute for each task, the machine which gives the
			   earliest completion time for the task */
			set<int>::iterator it;
			for (it = tasks.begin(); it != tasks.end(); ++it){
				int task = *it; int min_m = 0;
				for (int m = 0; m < number_machines; ++m) {
					if (completions[task][m] < completions[task][min_m]) {
						min_m = m;
				}   }
				min_completion[task] = min_m;
			}

			/* Find the task that has the earliest completion time */
			it  = tasks.begin();
			int min_task = *it;
			for (it = tasks.begin(); it != tasks.end(); ++it){
				int task = *it;
				if (completions[task][min_completion[task]] <
					completions[min_task][min_completion[min_task]]) {
					min_task = task;
			}   }

			/* Assign best task to its best machine */
			int min_machine = min_completion[min_task];

			schedule[min_task] = min_machine;
			original[min_task] = min_machine;

			/* Delete best task from the set of unassigned tasks */
			tasks.erase (min_task);

			/* Update completions */
			for (it = tasks.begin(); it != tasks.end(); ++it) {
				completions[*it][min_machine]+=etc[min_task][min_machine];
		}   }
		return schedule;

	}

	double calc_task_cost( int t, int m,
			const matrix<double>&  completions,
			const vector<double>& initial_completions,
			double makespan ){

		double result_task_cost =
				completions[t][m] * (
						( 1.0 / initial_completions[m] ) +
						(
								( 1 - ( initial_completions[m] / makespan ) )
								* ( 1.0 / makespan )
						)
				);

		return result_task_cost;

	}

	/**
	 * Modified min_min,
	 *
	 * @returns Schedule with optimized allocation of tasks for given compartment
	 */
	vector<int> InitialSolutions::game_min_min_v2 ( vector<int>& original,
			int cmp_start, int cmp_end ) {

		vector<int>     schedule       (number_tasks);
		matrix<double>  completions    (number_tasks, number_machines);
		matrix<double>  task_costs    (number_tasks, number_machines);
		vector<int>     min_task_cost (number_tasks);
		set<int>        tasks;
		vector<double>  initial_completions = ready;

		for ( int t = 0; t < number_tasks; t++){
			schedule[t] = original[t];
		}

		/* Init ready vector */
		for (int t = 0; t < number_tasks; ++t) {
			if( t < cmp_start || cmp_end < t ){
				// Not our task
				int machine_nr = original[t];
				initial_completions[machine_nr] += etc[t][machine_nr];
			}
		}


		/* Calculate makespan */
		double makespan = 0;
		for ( int m = 0; m < number_machines; m++ ){
			if ( initial_completions[m] > makespan ){
				makespan = initial_completions[m];
			}
		}

		/* Initialization */
		for (int t = cmp_start; t <= cmp_end; ++t) {
			tasks.insert (t);
			for (int m = 0; m < number_machines; ++m) {
				completions[t][m] = initial_completions[m] + etc[t][m];
				task_costs[t][m] = calc_task_cost(t,m, completions,
						initial_completions, makespan );
		}   }

		while (!tasks.empty()) {
			/* Compute for each task, the machine which gives the
			   earliest completion time for the task */
			set<int>::iterator it;
			for (it = tasks.begin(); it != tasks.end(); ++it){
				int task = *it; int min_m = 0;
				for (int m = 0; m < number_machines; ++m) {
					if (task_costs[task][m] < task_costs[task][min_m]) {
						min_m = m;
				}   }
				min_task_cost[task] = min_m;
			}

			/* Find the task that has the earliest completion time */
			it  = tasks.begin();
			int min_task = *it;
			for (it = tasks.begin(); it != tasks.end(); ++it){
				int task = *it;
				if (task_costs[task][min_task_cost[task]] <
					task_costs[min_task][min_task_cost[min_task]]) {
					min_task = task;
			}   }

			/* Assign best task to its best machine */
			int min_machine = min_task_cost[min_task];

			schedule[min_task] = min_machine;
			original[min_task] = min_machine;

			/* Delete best task from the set of unassigned tasks */
			tasks.erase (min_task);

			/* Update completions */
			for (it = tasks.begin(); it != tasks.end(); ++it) {
				completions[*it][min_machine]+=etc[min_task][min_machine];
				initial_completions[min_machine] += etc[min_task][min_machine];
				if ( initial_completions[min_machine] > makespan ){
					makespan = initial_completions[min_machine];
				}
				task_costs[*it][min_machine] = calc_task_cost(
						*it,min_machine, completions,
						initial_completions, makespan );

		}   }
		return schedule;

	}

    vector<int> InitialSolutions::max_min () {
        vector<int>     schedule       (number_tasks);
        matrix<double>  completions    (number_tasks, number_machines);
        vector<int>     min_completion (number_tasks);
        set<int>        tasks;

        /* Initialization */
        for (int t = 0; t < number_tasks; ++t) {
            tasks.insert (t);
            for (int m = 0; m < number_machines; ++m) {
                completions[t][m] = ready[m] + etc[t][m];
        }   }

        while (!tasks.empty()) {
            /* Compute for each task, the machine which gives the
               earliest completion time for the task */
            set<int>::iterator it;
            for (it = tasks.begin(); it != tasks.end(); ++it){
                int task = *it; int min_m = 0;
                for (int m = 0; m < number_machines; ++m) {
                    if (completions[task][m] < completions[task][min_m]) {
                        min_m = m;
                }   }
                min_completion[task] = min_m;
            }

            /* Find the task that has the maximum completion time */
            it  = tasks.begin();
            int max_task = *it;
            for (it = tasks.begin(); it != tasks.end(); ++it){
                int task = *it;
                if (completions[task][min_completion[task]] >
                    completions[max_task][min_completion[max_task]]) {
                    max_task = task;
            }   }

            /* Assign best task to its best machine */
            int min_machine = min_completion[max_task];
            schedule[max_task] = min_machine;

            /* Delete best task from the set of unassigned tasks */
            tasks.erase (max_task);

            /* Update completions */
            for (it = tasks.begin(); it != tasks.end(); ++it) {
                completions[*it][min_machine]+=etc[max_task][min_machine];
        }   }
        return schedule;
    }

    vector<int> InitialSolutions::game_max_min (vector<int>& original, int cmp_start, int cmp_end) {
            vector<int>     schedule       (number_tasks);
            matrix<double>  completions    (number_tasks, number_machines);
            vector<int>     min_completion (number_tasks);
            set<int>        tasks;
            set<int>		machines;

			for ( int t = cmp_start; t <= cmp_end; t++ ){
				machines.insert(original[t]);
			}

            /* Initialization */
            for (int t = cmp_start; t <= cmp_end; ++t) {
                tasks.insert (t);
                set<int>::iterator im;
				for (im = machines.begin(); im != machines.end(); im++) {
					int m = *im;
                    completions[t][m] = ready[m] + etc[t][m];
            }   }

            while (!tasks.empty()) {
                /* Compute for each task, the machine which gives the
                   earliest completion time for the task */
                set<int>::iterator it;
                for (it = tasks.begin(); it != tasks.end(); ++it){
                    int task = *it; int min_m = 0;
                    set<int>::iterator im;
					for (im = machines.begin(); im != machines.end(); im++) {
						int m = *im;
                        if (completions[task][m] < completions[task][min_m]) {
                            min_m = m;
                    }   }
                    min_completion[task] = min_m;
                }

                /* Find the task that has the maximum completion time */
                it  = tasks.begin();
                int max_task = *it;
                for (it = tasks.begin(); it != tasks.end(); ++it){
                    int task = *it;
                    if (completions[task][min_completion[task]] >
                        completions[max_task][min_completion[max_task]]) {
                        max_task = task;
                }   }

                /* Assign best task to its best machine */
                int min_machine = min_completion[max_task];
                schedule[max_task] = min_machine;
                original[max_task] = min_machine;

                /* Delete best task from the set of unassigned tasks */
                tasks.erase (max_task);

                /* Update completions */
                for (it = tasks.begin(); it != tasks.end(); ++it) {
                    completions[*it][min_machine]+=etc[max_task][min_machine];
            }   }
            return schedule;
        }

    vector<int> InitialSolutions::sufferage () {
        vector<int>     schedule          (number_tasks);
        matrix<double>  completions       (number_tasks, number_machines);
        vector<bool>    machines_assigned (number_machines);
        vector<int>     task_assigned     (number_machines);
        vector<double>  sufferages        (number_tasks);
        set<int>        tasks;
        set<int>        assigned;

        /* Initialization */
        for (int t = 0; t < number_tasks; ++t) {
            tasks.insert (t);
            for (int m = 0; m < number_machines; ++m) {
                completions[t][m] = ready[m] + etc[t][m];
        }   }

        while (!tasks.empty()) {
            /* Mark all machines as unassigned */
            for (int m = 0; m < number_machines; ++m) {
                machines_assigned[m] = false;
            }

            set<int>::iterator it1;
            for (it1 = tasks.begin(); it1 != tasks.end(); ++it1) {
                int task = *it1;
                /* Compute bests machines and sufferage value for 'task' */
                double first_best= plus_infinity; int first_min_machine= 0;
                double second_best = first_best; int second_min_machine= 0;
                for (int m = 0; m < number_machines; ++m) {
                    if (completions[task][m] <= first_best) {
                        second_best = first_best;
                        first_best  = completions[task][m];
                        second_min_machine = first_min_machine;
                        first_min_machine  = m;
                    }else if (completions[task][m] < second_best) {
                        second_best = completions[task][m];
                        second_min_machine = m;
                    }
                }

                double suffer = completions[task][second_min_machine]
                               -completions[task][first_min_machine];

                /* If machine is free, assign the task */
                if (!machines_assigned[first_min_machine]) {
                    machines_assigned[first_min_machine] = true;
                    schedule[task]                   = first_min_machine;
                    task_assigned[first_min_machine] = task;
                    sufferages[task]                 = suffer;
                    assigned.insert (task);
                /* If machine is busy, the task with most sufferage wins.
                   Old task won't be considered until next iteration     */
                } else {
                    int previous_task = task_assigned[first_min_machine];
                    if ( sufferages[previous_task] < suffer) {
                        schedule[task]                 = first_min_machine;
                        task_assigned[first_min_machine] = task;
                        sufferages[task]                 = suffer;
                        assigned.erase  (previous_task);
                        assigned.insert (task);
            }   }   }

            /* Delete scheduled tasks from the set 'tasks' */
            set<int>::iterator it2;
            for (it2 = assigned.begin(); it2 != assigned.end(); ++it2) {
                tasks.erase (*it2);
            }

            /* Update completions */
            for (it2 = assigned.begin(); it2 != assigned.end(); ++it2) {
                int task = *it2; int machine = schedule[task];
                for (it1 = tasks.begin(); it1 != tasks.end(); ++it1) {
                    completions[*it1][machine] += etc[task][machine];
                }
            }
            assigned.clear();
        }
        return schedule;
    }

    vector<int> InitialSolutions::game_sufferage (vector<int>& original, int cmp_start, int cmp_end) {
		vector<int>     schedule          (number_tasks);
		matrix<double>  completions       (number_tasks, number_machines);
		vector<bool>    machines_assigned (number_machines);
		vector<int>     task_assigned     (number_machines);
		vector<double>  sufferages        (number_tasks);
		set<int>        tasks;
		set<int>        assigned;
		set<int>		machines;

		for ( int t = cmp_start; t <= cmp_end; t++ ){
			machines.insert(original[t]);
		}

		/* Initialization */
		for (int t = cmp_start; t < cmp_end; ++t) {
			tasks.insert (t);
			set<int>::iterator im;
			for (im = machines.begin(); im != machines.end(); im++) {
				int m = *im;
				completions[t][m] = ready[m] + etc[t][m];
		}   }

		while (!tasks.empty()) {
			/* Mark all machines as unassigned */
			set<int>::iterator im;
			for (im = machines.begin(); im != machines.end(); im++) {
				int m = *im;
				machines_assigned[m] = false;
			}

			set<int>::iterator it1;
			for (it1 = tasks.begin(); it1 != tasks.end(); ++it1) {
				int task = *it1;
				/* Compute bests machines and sufferage value for 'task' */
				double first_best= plus_infinity; int first_min_machine= 0;
				double second_best = first_best; int second_min_machine= 0;
				set<int>::iterator im;
				for (im = machines.begin(); im != machines.end(); im++) {
					int m = *im;
					if (completions[task][m] <= first_best) {
						second_best = first_best;
						first_best  = completions[task][m];
						second_min_machine = first_min_machine;
						first_min_machine  = m;
					}else if (completions[task][m] < second_best) {
						second_best = completions[task][m];
						second_min_machine = m;
					}
				}

				double suffer = completions[task][second_min_machine]
							   -completions[task][first_min_machine];

				/* If machine is free, assign the task */
				if (!machines_assigned[first_min_machine]) {
					machines_assigned[first_min_machine] = true;
					schedule[task]                   = first_min_machine;
					original[task]                   = first_min_machine;
					task_assigned[first_min_machine] = task;
					sufferages[task]                 = suffer;
					assigned.insert (task);
				/* If machine is busy, the task with most sufferage wins.
				   Old task won't be considered until next iteration     */
				} else {
					int previous_task = task_assigned[first_min_machine];
					if ( sufferages[previous_task] < suffer) {
						schedule[task]                 = first_min_machine;
						original[task]                 = first_min_machine;
						task_assigned[first_min_machine] = task;
						sufferages[task]                 = suffer;
						assigned.erase  (previous_task);
						assigned.insert (task);
			}   }   }

			/* Delete scheduled tasks from the set 'tasks' */
			set<int>::iterator it2;
			for (it2 = assigned.begin(); it2 != assigned.end(); ++it2) {
				tasks.erase (*it2);
			}

			/* Update completions */
			for (it2 = assigned.begin(); it2 != assigned.end(); ++it2) {
				int task = *it2; int machine = schedule[task];
				for (it1 = tasks.begin(); it1 != tasks.end(); ++it1) {
					completions[*it1][machine] += etc[task][machine];
				}
			}
			assigned.clear();
		}
		return schedule;
	}


//  /*   Andreas Lochbihler "Relative Cost" version. Static relative  */
//  /*   index does not consider etc[t][m], but only etc_avg[t]       */
//  vector<int> InitialSolutions::relative_cost (double x) {
//      vector<int>     schedule         (number_tasks);
//      vector<double>  completions      (number_machines);
//      vector<double>  etc_avg          (number_tasks);
//      set<int>        tasks;
//
//      /* Compute task size (etc_avg) to the power of x */
//      completions = ready;
//      for (int t = 0; t < number_tasks; ++t) {
//          tasks.insert (t);
//          etc_avg[t] = 0.0;
//          for (int m = 0; m < number_machines; ++m) {
//              etc_avg[t] += etc[t][m];
//          }   etc_avg[t] = pow(etc_avg[t] / number_machines, x);
//      }
//
//      while (!tasks.empty()) {
//          double min_cost    = plus_infinity;
//          int    best_task    = 0;
//          int    best_machine = 0;
//          set<int>::iterator    it;
//
//          /* Find best task and best machine */
//          for (it = tasks.begin(); it != tasks.end(); ++it){
//              int task              = *it;
//              double completion_avg = 0;
//              double min_completion = plus_infinity;
//              int    min_machine    = 0;
//
//              /* Compute completion_avg , the minimum_completion time  */
//              /* and the machine that gives the minimum completion time*/
//              for (int m = 0; m < number_machines; ++m) {
//                  completion_avg += completions[m] + etc[task][m];
//                  if (min_completion > completions[m] + etc[task][m]) {
//                      min_completion = completions[m] + etc[task][m];
//                      min_machine = m;
//                  }
//              } completion_avg = completion_avg / number_machines;
//
//              /* Update best_task, best_machine, and best_cost info    */
//              /* that minimizes :  static_cost^x * dynamic_cost        */
//              double cost= etc_avg[task]*(min_completion/completion_avg);
//              if (cost < min_cost) {
//                  min_cost     = cost;
//                  best_task    = task;
//                  best_machine = min_machine;
//              }
//          }
//          /* Map task, and update completion time for the used machine */
//          schedule[best_task]        = best_machine;
//          completions[best_machine] += etc[best_task][best_machine];
//          tasks.erase(best_task);
//      }
//      return schedule;
//  }

    /*  Min-You Wu and Wei Shu original "Relative Cost" algorithm  */
     vector<int> InitialSolutions::relative_cost (double x) {
        vector<int>     schedule        (number_tasks);
        matrix<double>  completions     (number_tasks, number_machines);
        vector<double>  etc_avg         (number_tasks);
        matrix<double>  rc_static       (number_tasks, number_machines);
        vector<int>     min_completion  (number_tasks);
        vector<double>  completions_avg (number_tasks);
        vector<double>  rc_dynamic      (number_tasks);
        set<int>        tasks;

        /* Initialization: computes acumulative completions, average etc */
        /* and static relative cost                                      */
        assert( (x >= 0) && (x <= 1) );
        for (int t = 0; t < number_tasks; ++t) {
            tasks.insert (t);
            etc_avg[t] = 0;
            for (int m = 0; m < number_machines; ++m) {
                completions[t][m] = ready[m] + etc[t][m];
                etc_avg[t] += etc[t][m];
            }   etc_avg[t]  = etc_avg[t] / number_machines;
        }
        for (int t = 0; t < number_tasks; ++t) {
            for (int m = 0; m < number_machines; ++m) {
                rc_static[t][m] = etc[t][m] / etc_avg[t];
        }   }

        while (!tasks.empty()) {
            set<int>::iterator it;
            for (it = tasks.begin(); it != tasks.end(); ++it) {
                int task = *it;
                /* Compute completions_avg for each task and the machine */
                /* that gives the earliest completion time, in order to  */
                /* compute dynamic relative cost                         */
                completions_avg[task] = 0; int min_machine = 0;
                for (int m = 0; m < number_machines; ++m) {
                    if ( completions[task][m] <
                         completions[task][min_machine] )  min_machine = m;
                    completions_avg[task] += completions[task][m];
                }   completions_avg[task] /= number_machines;
                min_completion[task] = min_machine;
                rc_dynamic[task] = completions[task][min_machine] /
                                   completions_avg[task];
            }

            /* Select the task that minimizes (rc_dynamic^x) * rc_static */
            /* (the machine considered is the one that gives the minimum */
            /* completion time)                                          */
            it = tasks.begin();
            int min_task = *it;
            double best_cost = rc_dynamic[min_task] *
                      pow(rc_static[min_task][min_completion[min_task]],x);
            for (it = tasks.begin(); it != tasks.end(); ++it) {
                int task = *it;
                double cost = rc_dynamic[task] *
                              pow(rc_static[task][min_completion[task]],x);
                if (cost < best_cost) {min_task = task; best_cost = cost; }
            }

            /* Assign best task to its best machine */
            int min_machine = min_completion[min_task];
            schedule[min_task] = min_machine;

            /* Delete best task from the set of unassigned tasks */
            tasks.erase (min_task);

            /* Update completions */
            for (it = tasks.begin(); it != tasks.end(); ++it) {
                completions[*it][min_machine]+=etc[min_task][min_machine];
        }   }
        return schedule;
     }

     vector<int> InitialSolutions::ljfr_sjfr () {
        vector<double> completions (number_machines);
        vector<int>    schedule    (number_tasks);
        vector<int>    indices     (number_tasks);
        int  l      =  0;
        int  u      =  number_tasks - 1;
        bool ljfr   =  true;
        completions =  ready;

        /* Sort tasks in ascending order of workload */
        for (int i = 0; i < number_tasks; ++i) indices[i] = i;
        quick_sort (workload, indices, 0, number_tasks-1);

        /* Alternate LJFR-SJFR after the machines are firstly allocated */
        /* tasks following LJFR heuristic.                              */
        for (int i = 0; i < number_tasks; ++i)  {
            int m = 0;
            for (int j = 1; j < number_machines; ++j) {
                if (completions[j] < completions[m])             m = j;
                else if ( (completions[j] == completions[m]) &&
                          (mips[j] > mips[m])  )                 m = j;
            }
            if (ljfr) { /* longest job - fastest resource */
                completions[m] += etc[indices[u]][m];
                schedule[indices[u]] = m;
                --u;
            } else {    /* shortest job - fastest resource */
                completions[m] += etc[indices[l]][m];
                schedule[indices[l]] = m;
                ++l;
            }
            if (i >= number_machines) ljfr = !ljfr;
        }
        return schedule;
    }

//  vector<int> InitialSolutions::mct () { // Task order is not randomized
//      /* Also known as Fast-greedy */
//      vector<double> completions (number_machines);
//      vector<int>    schedule    (number_tasks);
//
//      /* Initialization */
//      for (int m = 0; m < number_machines; ++m)
//          completions[m] = ready[m];
//
//      for (int t = 0; t < number_tasks; ++t) {
//          /* Find the machine that gives minimum completion time */
//          int best_machine = 0;
//          for (int m = 0; m < number_machines; ++m) {
//              if ( (completions[m] + etc[t][m]) <
//                   (completions[best_machine] + etc[t][best_machine]) ) {
//                  best_machine = m;
//          }   }
//
//          /* Assign task to its best machine */
//          schedule[t] = best_machine;
//
//          /* Update completion */
//          completions[best_machine] += etc[t][best_machine];
//      }
//      return schedule;
//  }

    vector<int> InitialSolutions::mct () { // Task order is randomized
        /* Also known as Fast-greedy. */
        vector<double> completions (number_machines);
        vector<int>    schedule    (number_tasks);
        vector<int>    tasks       (number_tasks);

        for (int i = 0; i < number_tasks; ++i) { tasks[i] = i; }
        random_shuffle (tasks.begin(), tasks.end());

        /* Initialization */
        for (int m = 0; m < number_machines; ++m)
            completions[m] = ready[m];

        for (int t = 0; t < number_tasks; ++t) {
            /* Find the machine that gives minimum completion time */
            int task         = tasks[t];
            int best_machine = 0;
            for (int m = 0; m < number_machines; ++m) {
                if ( (completions[m] + etc[task][m]) <
                     (completions[best_machine]+etc[task][best_machine])) {
                    best_machine = m;
            }   }

            /* Assign task to its best machine */
            schedule[task] = best_machine;

            /* Update completion */
            completions[best_machine] += etc[task][best_machine];
        }
        return schedule;
    }

    vector<int> InitialSolutions::met () {
        /* Also known as LBA (limited best assignment) and UDA (user  */
        /* directed assignment)                                       */
        vector<int> schedule (number_tasks);

        for (int t = 0; t < number_tasks; ++t) {
            /* Find the machine that gives the minimum execution time */
            int best_machine = 0;
            for (int m = 0; m < number_machines; ++m) {
                if (etc[t][m] < etc[t][best_machine]) {
                    best_machine = m;
            }   }

            /* Assign task to its best machine */
            schedule[t] = best_machine;
        }
        return schedule;
    }

    vector<int> InitialSolutions::sa (double r_low, double r_high) {
        vector<double>  completions  (number_machines);
        vector<int>     schedule     (number_tasks);
        assert ((r_low >= 0.0)  && (r_low <= 1.0)  &&
                (r_high >= 0.0) && (r_high <= 1.0) && (r_low < r_high));

        /* Initialization */
        for (int m = 0; m < number_machines; ++m) {
            completions[m] = ready[m];
        }

        /* Compute initial load balance index */
        double r_min = completions[0], r_max = completions[0];
        for (int m = 0; m < number_machines; ++m) {
            if (completions[m] < r_min) r_min = completions[m];
            if (completions[m] > r_max) r_max = completions[m];
        }
        double r = (r_max <= 0.0) ? 0.0 : (r_min / r_max);
        bool up  = true;
        if      (r <= r_low ) up = true;
        else if (r >= r_high) up = false;


        for (int t = 0; t < number_tasks; ++t) {
            /* Compute load balance index */
            r_min = completions[0], r_max = completions[0];
            for (int m = 0; m < number_machines; ++m) {
                if (completions[m] < r_min) r_min = completions[m];
                if (completions[m] > r_max) r_max = completions[m];
            }
            r = (r_max <= 0.0) ? 0.0 : (r_min / r_max);

            /* Use MCT until r is >= r_high,then use MET until r <= r_low*/
            if (up  && (r >= r_high)) up = false;
            else if (!up && (r <= r_low )) up = true;
            int best_machine = 0;
            if (up) {
                /* Minimum Completion Time */
                for (int m = 0; m < number_machines; ++m) {
                    if ((completions[m] + etc[t][m]) <
                        (completions[best_machine]+ etc[t][best_machine])){
                        best_machine = m;
                }   }
            } else {
                /* Minimum Execution Time */
                for (int m = 0; m < number_machines; ++m) {
                    if (etc[t][m] < etc[t][best_machine]) {
                        best_machine = m;
                }   }
            }
            /* Assign task to its best machine, update completions */
            schedule[t] = best_machine;
            completions[best_machine] += etc[t][best_machine];
        }
        return schedule;
    }

    vector<int> InitialSolutions::kpb (double percent) {
        vector<double> completions    (number_machines);
        vector<int>    schedule       (number_tasks);
        vector<int>    best_machines;
        assert ( (percent > 0) && (percent <= 1.0) &&
                 (ceil(percent * number_machines) >= 1) &&
                 (ceil(percent * number_machines) <= number_machines) );

        /* Initialization */
        int k  =  (int) ceil(percent * number_machines);
        best_machines.resize(k);

        for (int m = 0; m < number_machines; ++m) {
            completions[m] = ready[m];
        }

        for (int t = 0; t < number_tasks; ++t) {
            /* Select k percent of all the machines that give  */
            /* the task the best execution time                */

            vector<int> index (number_machines);
            for (int m = 0; m < number_machines; ++m) index[m] = m;
            quick_sort(etc[t], index, 0, number_machines - 1);

            for (int m = 0; m < k; ++m) best_machines[m] = index[m];

            /* Select the machine from best_machines with earliest c.t. */
            int best_machine = best_machines[0];
            for (int i = 0; i < k; ++i) {
                int mach = best_machines[i];
                if ( (completions[mach] + etc[t][mach]) <
                     (completions[best_machine] + etc[t][best_machine]) ) {
                    best_machine = mach;
            }   }

            /* Assign task to its best machine, update completions */
            schedule[t] = best_machine;
            completions[best_machine] += etc[t][best_machine];
        }
        return schedule;
    }

    vector<int> InitialSolutions::olb () {
        vector<double>  completions  (number_machines);
        vector<int>     schedule     (number_tasks);

        /* Initialization */
        for (int m = 0; m < number_machines; ++m) {
            completions[m] = ready[m];
        }

        for (int t = 0; t < number_tasks; ++t) {
            /* Find earliest available machine */
            int best_machine = 0;
            for (int m = 0; m < number_machines; ++m) {
                if (completions[m] < completions[best_machine]) {
                    best_machine = m;
                }
                //else if (completions[m] == completions[best_machine]) {
                //    if (etc[t][m] < etc[t][best_machine]) {
                //        best_machine = m;
                //    }
                //}
            }
            /* Assign task to its best machine, update completions */
            schedule[t] = best_machine;
            completions[best_machine] += etc[t][best_machine];
        }
        return schedule;
    }

    vector<int> InitialSolutions::random () {
        vector<int>    schedule       (number_tasks);
        for (int t = 0; t < number_tasks; ++t) {
            schedule[t] = rand_ (0, number_machines - 1);
        }
        return schedule;
    }

    double InitialSolutions::makespan (vector<int>& schedule) {
        vector<double> completions = completion(schedule);

        double max_completion = completions[0];
        for (int m = 0; m < number_machines; ++m) {
            if ( ((completions[m] - ready[m]) > 0 ) &&
                  (completions[m] > max_completion)) {
                max_completion = completions[m];
        }   }
        return max_completion;
    }

    double InitialSolutions::utilization (vector<int>& schedule) {
        double mk = makespan(schedule);
        vector<double> completions = completion(schedule);

        double sum_completions  = 0;
        int    nb_mach_involved = 0;
        for (int m = 0; m < number_machines; ++m) {
            if ( (completions[m]-ready[m]) > 0) {
                sum_completions += completions[m];
                nb_mach_involved++;
            }
        }
        return (sum_completions/(mk * nb_mach_involved));
    }

    double InitialSolutions::flowtime (vector<int>& schedule) {
        matrix<int> schedule_;
        double flowtime  = 0;
        double flowtime_ = 0;

        schedule.resize(number_machines);
        for (int t = 0; t < number_tasks; ++t) {
            schedule_[schedule[t]].push_back(t);
        }

        for (int m = 0; m < number_machines; ++m) {
            /* Sort tasks from machine m according to its etc */
            int size = schedule_[m].size();
            for (int i = 1; i < size; ++i) {
                int task    = schedule_[m][i];
                double etc_ = etc[task][m];
                int j       = i;
                for(; (j > 0) && (etc[schedule_[m][j-1]][m] > etc_);--j){
                    schedule_[m][j] = schedule_[m][j-1];
                }
                schedule_[m][j]  = task;
            }
            /* Compute flowtime for machine m */
            flowtime_ = 0.0;
            double completion_ = ready[m];
            for (unsigned int i = 0; i < schedule_[m].size(); ++i) {
                completion_  += etc[schedule_[m][i]][m];
                flowtime_    += completion_;
            }
            flowtime += flowtime_;
        }
        return flowtime;
    }

    double InitialSolutions::match_prox (vector<int>& schedule) {
        vector<int> ideal_schedule = met ();
        double sum_etc= 0.0, sum_etc_ideal = 0.0;

        for (int t = 0; t < number_tasks; ++t) {
            sum_etc       += etc[t][schedule[t]];
            sum_etc_ideal += etc[t][ideal_schedule[t]];
        }
        return sum_etc_ideal/sum_etc;
    }

    vector<double> InitialSolutions::completion(vector<int>& schedule) {
        vector<double> completions (number_machines);
        completions = ready;

        for (int t = 0; t < number_tasks; ++t) {
            completions[schedule[t]] += etc[t][schedule[t]];
        }
        return completions;
    }

    /**
     * Calculates ETC matrix from workload and mips vectors.
     */
    void InitialSolutions::compute_etc (const vector<double>& workload,
                                        const vector<double>& mips) {
        for (int i = 0; i < number_tasks; ++i) {
            for (int j = 0; j < number_machines; ++j) {
                etc[i][j] = workload[i]/mips[j];
            }
        }
    }
//******************************************************************
    //************************************************
    vector<int> InitialSolutions::enviroment(const vector<int> &best_sol, int env_length, enviroment_type choise)
    {
    	if(env_length >= number_tasks)
    		env_length = number_tasks;
    	else if(env_length < 0)
    		env_length = 0;

    	switch(choise)
    	{
			case env_basic: return enviroment_basic(best_sol,env_length); break;
			case env_minimalizing: return enviroment_min(best_sol,env_length); break;
    	}
    	return enviroment_basic(best_sol,env_length);
    }
    //************************************************
    vector<int> InitialSolutions::enviroment_basic (const vector<int>& best_sol, int env_length)
    {
    	vector<int> schedule(number_tasks);

    	for(int i=0;i<env_length;i++)//copy first x positions from best solution
    		schedule[i] = best_sol[i];

    	for(int i=env_length;i<number_tasks;i++)//fill enviroment(rest positions)
    		schedule[i] = rand_(0,number_machines-1);

    	return schedule;
    }
    //***********************************************
    vector<int> InitialSolutions::enviroment_min (const vector<int>& best_sol, int env_length)
    {
    	vector<int> schedule(number_tasks), used(number_tasks,0);
    	vector<double> completion = ready;

    	for(int i=0;i<(int)best_sol.size();i++)
    	{
    		completion[best_sol[i]] += etc[i][best_sol[i]];
    	}

    	int min; double min_value;
    	for(int i=0;i<env_length;i++)
    	{
    		for(int j=0;j<(int)completion.size();j++)
    		{
    			if(!used[j])
    			{
    				min_value = completion[j];
    				min = j;
    				break;
    			}
    		}

    		for(int j=0;j<(int)completion.size();j++)
    		{
    			if(completion[j] < min_value && !used[j])
    			{
    				min = j;
    				min_value = completion[j];
    			}
    		}
    		schedule[min] = best_sol[min];
    		used[min] = 1;
    	}

    	for(int i=0;i<(int)best_sol.size();i++)
    	{
    		if(!used[i])
    		{
    			schedule[i] = rand_(0,number_machines-1);
    			used[i] = 1;
    		}
    	}
    	return schedule;
    }
    //****************************************************************
}//namespace AdHocHeuristics
