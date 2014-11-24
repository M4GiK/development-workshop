// FILE: Memetic.req.cc
#include "Memetic.hh"
#include "Energy.hh"
#include <math.h>

using namespace std;

bool compare_solutions (const Memetic::Solution* i, const Memetic::Solution* j) 
{
	return i->is_better_than(*j);
}

skeleton Memetic 
{

    int rand_n (int n)
    {
            return rand_(n);
    }

    int min(int a, int b)
    {
        if(a<b)
            return a;
        else
            return b;
    }
        /************* STARTING POPULATION given possibilities ***************/
    void start_ljfrsjfr (Population& pop)
	{
		pop._solutions[0]->heuristic(AdHocHeuristics::LJFRSJFR_h);
        for (int i = 1; i < pop.size(); ++i)
		{
            pop._solutions[i]->heuristic(AdHocHeuristics::LJFRSJFR_h);
            pop._solutions[i]->perturbate_randomly();
        }
    }

    void start_mct (Population& pop)
	{
        pop._solutions[0]->heuristic(AdHocHeuristics::MCT_h);
        for (int i = 1; i < pop.size(); ++i)
		{
            pop._solutions[i]->heuristic(AdHocHeuristics::MCT_h);
            pop._solutions[i]->perturbate_randomly();
        }
    }

    void start_random (Population& pop)
	{
        for (int i = 0; i < pop.size(); ++i)
		{
            pop._solutions[i]->heuristic(AdHocHeuristics::Random_h);
        }
    }

    void start_all (Population& pop)
	{
        assert (pop.size() >= 2);
        pop._solutions[0]->heuristic(AdHocHeuristics::LJFRSJFR_h);
        pop._solutions[1]->heuristic(AdHocHeuristics::MCT_h);
        for (int i = 2; i < pop.size(); ++i)
		{
            pop._solutions[i]->heuristic(AdHocHeuristics::Random_h);
        }
    }

    void start_minmin (Population& pop)
	{
        pop._solutions[0]->heuristic(AdHocHeuristics::MinMin_h);
        for (int i = 1; i < pop.size(); ++i) {
            pop._solutions[i]->heuristic(AdHocHeuristics::MinMin_h);
            pop._solutions[i]->perturbate_randomly();
        }
    }

    void start_rc (Population& pop)
	{
        pop._solutions[0]->heuristic(AdHocHeuristics::RelativeCost_h);
        for (int i = 1; i < pop.size(); ++i)
		{
            pop._solutions[i]->heuristic(AdHocHeuristics::RelativeCost_h);
            pop._solutions[i]->perturbate_randomly();
        }
    }

    void start_kpb (Population& pop)
	{
        pop._solutions[0]->heuristic(AdHocHeuristics::KPB_h);
        for (int i = 1; i < pop.size(); ++i)
		{
            pop._solutions[i]->heuristic(AdHocHeuristics::KPB_h);
            pop._solutions[i]->perturbate_randomly();
        }
    }

    void start_all2 (Population& pop)
	{
        assert (pop.size() >= 5);
        pop._solutions[0]->heuristic(AdHocHeuristics::LJFRSJFR_h);
        pop._solutions[1]->heuristic(AdHocHeuristics::MCT_h);
        pop._solutions[2]->heuristic(AdHocHeuristics::MinMin_h);
        pop._solutions[3]->heuristic(AdHocHeuristics::RelativeCost_h);
        pop._solutions[4]->heuristic(AdHocHeuristics::KPB_h);
        for (int i = 5; i < pop.size(); ++i)
		{
			pop._solutions[i]->heuristic(AdHocHeuristics::Random_h);
        }
    }

	/***************** SELECT given possibilities ************************/
    void select_random (Population& pop)
    {
        int size = pop._solver.setup().population_size();
		int size2 = pop.size();
        Population interpop(pop._solver);

        for (int i=0; i<size; ++i)
		{
			int r = rand_(0, size2-1);
            *interpop._solutions[i] = *pop._solutions[r];
        }
        pop = interpop;
    }

    void select_best (Population& pop)
    {
        int size = pop._solver.setup().population_size();
		int size2 = pop.size();
        Population interpop(pop._solver);

        for (int i=0; i<size; ++i)
		{
            int index = i % size2;
            *interpop._solutions[i] = *pop._solutions[index];
        }
        pop = interpop;
    }

    void select_linear_ranking (Population& pop)
	/*Attention:population size may vary during the same generation*/
	{
        int size = pop._solver.setup().population_size();
		int size2 = pop.size();
        Population interpop(pop._solver);

        static vector<int> fitness_ranking (0);
        static int fitness_ranking_sum = 0;

        if (fitness_ranking.size() != (unsigned int) size2)
		{
			fitness_ranking.resize(size2);
			fitness_ranking_sum = 0;
            for (int i = 0; i < size2; ++i)
			{
                fitness_ranking_sum += (int) (i+1);
                fitness_ranking[i] = fitness_ranking_sum +1;
            }
        }

        for (int i = 0; i<size; ++i)
		{
            int rnd    = rand_(1,fitness_ranking_sum);
            int index1 = binary_locate (fitness_ranking, rnd) + 1;
            int index2 = size2-1;
            *interpop._solutions[i] = *pop._solutions[index2-index1];
        }
        pop = interpop;
    }

    void select_exp_ranking (Population& pop)
	/*Attention:population size may vary during the same generation*/
	{
		int size = pop._solver.setup().population_size();
		int size2 = pop.size();
		Population interpop (pop._solver);

		static vector<double> fitness_ranking (0);
        static double fitness_ranking_sum = 0;

        if (fitness_ranking.size() != (unsigned int) size2)
		{
			fitness_ranking.resize(size2);
			fitness_ranking_sum = 0;
            for (int i=0; i<size2; ++i)
		    {
                fitness_ranking_sum += (double) ceil(exp(i+1));
                fitness_ranking[i] = fitness_ranking_sum +1;
            }

            // select_extra_parameter refers in this case to the maximum
            // exponential ranking probability
            double normalize = pop._solver.setup().select_extra_parameter()
                    * fitness_ranking[size2-1]
                    / exp(size2);

            for (int i=0 ; i<size2; ++i)
			{
				fitness_ranking[i]= (double) ceil ( (double)normalize * fitness_ranking[i]  );
            }
        }

        for (int i=0; i<size; ++i)
		{
			double rnd   = rand_(1,fitness_ranking_sum);
			int index1 = binary_locate (fitness_ranking, rnd) + 1;;
			int index2 = size2-1;
            *interpop._solutions[i] = *pop._solutions[index2-index1];
		}
		pop = interpop;
	}

    void select_binary_tournament (Population& pop)
	{
		if (pop.size() < 2)
			return select_best (pop);

        int size = pop._solver.setup().population_size();
		int size2 = pop.size();
        Population interpop(pop._solver);

        for (int i=0; i<size; ++i)
		{
            int one = rand_(0, size2-1);
            int two = rand_(0, size2-1);
            while (two==one) two= rand_(0, size2-1);

            int best, worst;
            if (pop._solutions[one]->is_better_than(*pop._solutions[two]))
		    {
                best  = one; worst = two;
            }
			else
			{
                best  = two;  worst = one;
            }

            // select_extra_parameter refers in this case  to the
            // tournament probability
			if (rand_(pop._solver.setup().select_extra_parameter()))
				*interpop._solutions[i] = *pop._solutions[best];
			else *interpop._solutions[i] = *pop._solutions[worst];
		}
        pop = interpop;
    }

    void select_n_tournament (Population& pop)
    {
        if ( pop.size() <
             floor(pop._solver.setup().select_extra_parameter())
           ) return select_best(pop);

        int size = pop._solver.setup().population_size();
		int size2 = pop.size();
        vector<int> tournament ((int)pop._solver.setup().select_extra_parameter());
        Population interpop (pop._solver);

        for (int i=0; i<size; ++i)
		{
            // select_extra_parameter refers in this case to the
            // tournament size
            for (int k=0;k<floor(pop._solver.setup().select_extra_parameter());++k)
		    {
                int ind = rand_(0,size2-1);
                int m   = 0;
                bool nequal = true;
                while ((m<k) && nequal)
				{
                    if (tournament[m] == ind) nequal = false;
                    ++m;
                }
                if (nequal) tournament[k] = ind;
                else --k;
            }

            int best = tournament[0];
            for (int k=1; k < floor(pop._solver.setup().select_extra_parameter()); ++k)
		    {
				if ( pop._solutions[tournament[k]]->is_better_than (*pop._solutions[best]))
				{
					best = tournament[k];
                }
            }
            *interpop._solutions[i] = *pop._solutions[best];
        }
        pop = interpop;
    }

	/***************** MERGE and MUTATE SELECTION given possibilities ************************/
	vector<Solution> random_sel (const Population& pop, int n, float extra_p)
    {
		vector<Solution> selected(n, Solution(pop._solver.problem()));
	    for(int i=0; i<n; i++)
			selected[i] = *pop._solutions[rand_(0,pop._solutions.size()-1)];

		return selected;
    }

	vector<Solution> best_sel (const Population& pop, int n, float extra_p)
    /*PRE: pop is ordered by fitness value*/
	{
		int size = pop.size();
		vector<Solution> selected(n, Solution(pop._solver.problem()));

        for (int i=0; i<n; i++)
		{
            int index = i % size;
            selected[i] = *pop._solutions[index];
        }
		return selected;
    }

    vector<Solution> linear_ranking_sel (const Population& pop, int n, float extra_p)
    /*PRE: pop is ordered by fitness value*/
	{
		int size = pop.size();
		vector<Solution> selected(n, Solution(pop._solver.problem()));

        static vector<int> fitness_ranking (0);
        static int fitness_ranking_sum = 0;

		if (fitness_ranking.size() != pop._solutions.size())
		{
			fitness_ranking.resize(size);
			fitness_ranking_sum = 0;
			for (int i = 0; i < size; i++)
			{
				 fitness_ranking_sum += (int) (i+1);
	             fitness_ranking[i] = fitness_ranking_sum +1;
		    }
		}

        for (int i = 0; i<n; i++)
		{
            int rnd    = rand_(1,fitness_ranking_sum);
            int index1 = binary_locate (fitness_ranking, rnd) + 1;
            int index2 = size-1;
            selected[i] = *pop._solutions[index2-index1];
        }
		return selected;
    }

    vector<Solution> exp_ranking_sel (const Population& pop, int n, float extra_p)
	/*PRE: pop is ordered by fitness value*/
	{
		int size = pop.size();
		vector<Solution> selected(n, Solution(pop._solver.problem()));

        static vector<double> fitness_ranking (0);
        static double fitness_ranking_sum = 0;

		if (fitness_ranking.size() != pop._solutions.size())
		{
			fitness_ranking.resize(size);
			fitness_ranking_sum = 0;
			for (int i=0; i<size; i++)
			{
				fitness_ranking_sum += (double) ceil(exp(i+1));
	            fitness_ranking[i] = fitness_ranking_sum +1;
		    }
		}
        // extra_p refers in this case to the maximum
        // exponential ranking probability
        double normalize = extra_p
		    * fitness_ranking[size-1]
			/ exp(size);

         for (int i=0 ; i<size; i++)
		 {
			fitness_ranking[i]= (double) ceil ( (double)normalize *
                                            fitness_ranking[i]  );
         }

        for (int i=0; i<n; i++)
		{
            double rnd   = rand_(1,fitness_ranking_sum);
            int index1 = binary_locate (fitness_ranking, rnd) + 1;
            int index2 = size-1;
            selected[i] = *pop._solutions[index2-index1];
        }

		return selected;
    }

    vector<Solution> binary_tournament_sel (const Population& pop, int n, float extra_p)
    {
		int size = pop.size();
		vector<Solution> selected(n, Solution(pop._solver.problem()));

		if (size <2)
		{
			for (int i=0; i<n; i++)
			{
				selected[i] = *pop._solutions[0];
			}
			return selected;
		}

        for (int i=0; i<n; i++)
		{
            int one = rand_(0, size-1);
            int two = rand_(0, size-1);
            while (two==one) two= rand_(0, size-1);

            int best, worst;

            if (pop._solutions[one]->is_better_than(*pop._solutions[two]))
		    {
                best  = one; worst = two;
            }
			else
			{
                best  = two;  worst = one;
            }

            // extra_p refers in this case  to the
            // tournament probability
			if (rand_(extra_p))
                selected[i] = *pop._solutions[best];
            else
                selected[i] = *pop._solutions[worst];
		}
        return selected;
    }

    vector<Solution> n_tournament_sel (const Population& pop, int n, float extra_p)
    {
        if ( pop.size() < floor(pop._solver.setup().select_extra_parameter()))
			return binary_tournament_sel(pop,n,extra_p);

		int size = pop.size();
        vector<int> tournament ((int)extra_p);
		vector<Solution> selected(n, Solution(pop._solver.problem()));

        for (int i=0; i<n; i++)
		{
            // extra_p refers in this case to the
            // tournament size
            for (int k=0; k<floor(extra_p); k++)
			{
                int ind = rand_(0,size-1);
                int m = 0;
                bool nequal = true;
                while ((m<k) && nequal)
				{
                    if (tournament[m] == ind) nequal = false;
                    ++m;
                }
                if (nequal) tournament[k] = ind;
                else --k;
            }

            int best = tournament[0];
            for (int k=1; k < extra_p; k++)
		    {
                if ( pop._solutions[tournament[k]]->is_better_than (*pop._solutions[best]))
				{
                    best = tournament[k];
                }
            }
            selected[i] = *pop._solutions[best];
        }
		return selected;
    }

	// *********************************************************************
	// Local Search Engine
	// *********************************************************************

	typedef void (*LocalSearchFunctionsType) (const Solver& solver, Solution& sol);

	Local_Search_Engine::Local_Search_Engine (const Solver& solver)
		:_solver(solver)
	{
	}

	Local_Search_Engine::~Local_Search_Engine ()
	{
	}

        void local_move (const Solver& solver, Solution& sol)
	{
		if (solver.problem().number_machines == 1) return;
		/* Randomly select a task from a machine and moves it to another */
		int task = rand_(0, solver.problem().number_tasks - 1);
		int machine_src = sol.schedule[task];
		int machine_dst = rand_(0, solver.problem().number_machines - 1);

		while (machine_dst == machine_src)
			machine_dst = rand_(0, solver.problem().number_machines - 1);
		sol.perform_move (task, machine_dst);
	}

	void steepest_local_move (const Solver& solver, Solution& sol)
	{
		Solution sol_aux(sol);

		/* Randomly select a task from a machine*/
		int task = rand_(0, solver.problem().number_tasks - 1);
		int machine = sol.schedule[task];

		for (int i=0; i<solver.problem().number_machines;i++)
		{
			if (i != machine)
			{
				sol_aux.perform_move (task, i);
				if (LS_improvement(sol_aux,sol))
				{
					sol = sol_aux;
					sol.compute_flowtime();
				}
			}
		}
	}

	void local_swap (const Solver& solver, Solution& sol)
	{
		/* Transform schedule representation */
		matrix<int> schedule;
		sol.transform_schedule_representation (schedule);

		/* Randomly selects a task */
		int task1    = rand_(0, solver.problem().number_tasks - 1);
		int machine = sol.schedule[task1];

		/* Randomly select a task from another machine, and swaps them */
		int task2= sol.select_task_from_another_machine(machine,schedule);
		if (task2 != -1)
			sol.perform_swap (task1, task2);
	}

	void steepest_local_swap (const Solver& solver, Solution& sol)
	{
		Solution sol_aux(sol);

		/* Randomly select a task from a machine*/
		int task = rand_(0, solver.problem().number_tasks - 1);
		int machine = sol.schedule[task];

		for (int i=0; i<solver.problem().number_tasks;i++)
		{
			if ((i != task) && (sol_aux.schedule[i] != machine))
			{
				sol_aux.perform_swap (task, i);
				if (LS_improvement(sol_aux,sol))
				{
					sol = sol_aux;
					sol.compute_flowtime();
				}
				sol_aux.perform_swap (task, i);
			}
		}
	}

	void local_rebalance (const Solver& solver, Solution& sol)
	{
		/* ---- REBALANCE PROCESS ---- */
		bool found = false;

		// -- Transform schedule representation
		matrix<int> schedule;
		sol.transform_schedule_representation (schedule);

		// -- Randomly select a machine that is loaded the most
		vector<int> most_loaded_machines;
		sol.compute_most_loaded_machines (schedule, most_loaded_machines);
		int machine1 =  most_loaded_machines.front();
		int machine2;

		// -- Randomly select a task (task2) from another machine
		int task1;
		int task2= sol.select_task_from_another_machine(machine1,schedule);
		if (task2 != -1)
		{
			// -- Randomly find a task in machine1 with more etc than task2
			// -- Moreover, whith probability lsearch_extra_parameter(),
			// -- makespan for machine2 must be <= than the makespan
			machine2          = sol.schedule[task2];
			vector<int> tasks = schedule[machine1];
			random_shuffle (tasks.begin(), tasks.end(), rand_n);

			unsigned int i = 0;
			bool strong_b = (rand_(solver.setup().lsearch_extra_parameter()));
			while ( (i < tasks.size()) && (!found) )
			{
				task1 = tasks[i];
				if ( solver.problem().etc[task1][machine1] >= solver.problem().etc[task2][machine1] )
				{
					if (strong_b)
					{
						double completion2 = sol.completion[machine2] -
							solver.problem().etc[task2][machine2] +
							solver.problem().etc[task1][machine2];
						if (completion2 <= sol.local_makespan)
						{
							found = true;
							sol.perform_swap (task1, task2);
						}
					}
					else
					{
						found = true;
						sol.perform_swap (task1, task2);
					}
				}
				++i;
			}
		}
		if (!found)
		{
			// -- Transform schedule representation
			sol.transform_schedule_representation (schedule);

			// -- Randomly select a machine that is loaded the most
			sol.compute_most_loaded_machines (schedule, most_loaded_machines);
			machine1 = most_loaded_machines.front();

			// -- Compute tasks from machine1
			vector<int> tasks = schedule[machine1];
			random_shuffle (tasks.begin(), tasks.end(), rand_n);

			// -- Try to move a task (once) to reduce the completion time
			unsigned int i = 0; int j = 0;
			while ( (i < tasks.size()) && (!found) )
			{
				task1 = tasks[i];
				j = 0;
				while ( (j < sol._problem.number_machines) && (!found) )
				{
					if (j != machine1)
					{
						double completion2  =  sol.completion[j]+ sol._problem.etc[task1][j];
						if (completion2 <= sol.local_makespan)
						{
							found = true;
							sol.perform_move(task1, j);
						}
					}
					++j;
				}
				++i;
			}

			/* ---- IF REBALANCING FAILS ---- */
			if (!found)
			{
				if (rand_(0.50))
					local_swap (solver,sol);
				else
					local_move (solver, sol);
			}
		}
	}

	void deep_local_rebalance (const Solver& solver, Solution& sol)
	/*Similar to local rebalance, but it tries to choose the best decision in every step*/
	{
		/* ---- REBALANCE PROCESS ---- */
		bool found = false;

		// -- Transform schedule representation
		matrix<int> schedule;
		sol.transform_schedule_representation (schedule);

		// -- Randomly select a machine that is loaded the most
		vector<int> most_loaded_machines;
		sol.compute_most_loaded_machines (schedule, most_loaded_machines);

		int machine1 =  most_loaded_machines.front();
		int machine2;
		int task1;

		// -- finds a task from another machine with the minimum etc in machine1
		int task2 = sol.select_best_task_from_another_machine(machine1,schedule);
		vector<int> tasks = schedule[machine1];

		if (task2 != -1)
		{
			// -- finds the task from machine1 whith the minimum etc for the machine in which is assigned task2
			task1 = sol.select_best_task_for_this_machine(sol.schedule[task2], tasks);
			machine2 = sol.schedule[task2];

			bool strong_b = (rand_(solver.setup().lsearch_extra_parameter()));
			if (solver.problem().etc[task1][machine1] >= solver.problem().etc[task2][machine1])
			{
				if (strong_b)
				{
					double completion2 = sol.completion[machine2] -
						solver.problem().etc[task2][machine2]     +
						solver.problem().etc[task1][machine2];

					if (completion2 <= sol.local_makespan)
					{
						found = true;
						sol.perform_swap (task1, task2);
					}
				}
				else
				{
					found = true;
					sol.perform_swap (task1, task2);
				}
			}
		}
		if (!found)
		/*Performs the best movement of a task in machine1*/
		{
			random_shuffle(tasks.begin(), tasks.end(), rand_n);
			task1 = tasks.front();
			Solution sol_aux(sol);

			for (int i=0; i<solver.problem().number_machines;i++)
			{
				if (i != machine1)
				{
					sol_aux.perform_move(task1, i);
					if (LS_improvement(sol_aux,sol))
					{
						sol_aux.compute_flowtime();
						sol = sol_aux;
					}
				}
			}
		}
	}

	void local_mct_move (const Solver& solver, Solution& sol)
	{
		int task = rand_(0, solver.problem().number_tasks-1);
		double mct = plus_infinity;
		int m=0;
		double completion;

		for (int i=0; i<solver.problem().number_machines; i++)
		{
			if (i != sol.schedule[task])
			{
				completion = sol.completion[i] + solver.problem().etc[task][i];
				if (completion < mct)
				{
					mct = completion;
					m = i;
				}
			}
		}
		sol.perform_move(task, m);
	}

	void local_mct_swap (const Solver& solver, Solution& sol)
	{
		/* Randomly select a task from a machine*/
		int task1 = rand_(0, solver.problem().number_tasks - 1);
		int task2 = -1;
		int machine1 = sol.schedule[task1];
		int machine2;
		double cmpl = plus_infinity;
		double c1, c2;

		/*Search for the most reducing mct swap*/
		for (int i=0; i<solver.problem().number_tasks;i++)
		{
			if ((i != task1) && (sol.schedule[i] != machine1))
			{
				machine2 = sol.schedule[i];
				c1 = sol.completion[machine1] - solver.problem().etc[task1][machine1]
					+ solver.problem().etc[i][machine1];
				c2 = sol.completion[machine2] - solver.problem().etc[i][machine2]
					+ solver.problem().etc[task1][machine2];
				if (max(c1,c2) < cmpl)
				{
					task2 = i;
					cmpl = max(c1,c2);
				}
			}
		}

		if (task2 == -1)
		{
			/*All tasks are scheduled at the same machine*/
			/*A random movement is performed*/
			local_move(solver, sol);
		}
		else sol.perform_swap(task1, task2);
	}

	void local_mft_move (const Solver& solver, Solution& sol)
	{
		/* Randomly select a task from a machine*/
		int task = rand_(0, solver.problem().number_tasks - 1);
		int machine1 = sol.schedule[task];
		int machine2 = -1;
		double ftime = plus_infinity;
		double flowtime;

		for (int i=0; i<solver.problem().number_machines; i++)
		{
			if (i != machine1)
			{
				sol.schedule[task] = i;
				flowtime = sol.get_flowtime(i);
				if (flowtime < ftime)
				{
					ftime = flowtime;
					machine2 = i;
				}
			}
		}
		if (machine2 != -1)
			sol.perform_move(task,machine2);
	}

	void local_mft_swap (const Solver& solver, Solution& sol)
	{
		/* Randomly select a task from a machine*/
		int task1 = rand_(0, solver.problem().number_tasks - 1);
		int task2 = -1;
		int machine1 = sol.schedule[task1];
		int machine2;
		double sum_ftime = plus_infinity;
		double flowtime;

		for (int i=0; i<solver.problem().number_tasks; i++)
		{
			machine2 = sol.schedule[i];
			if (machine1 != machine2)
			{
				sol.schedule[task1] = machine2;
				sol.schedule[i] = machine1;
				flowtime = sol.get_flowtime(machine1) + sol.get_flowtime(machine2);
				if (flowtime < sum_ftime)
				{
					task2 = i;
					sum_ftime = flowtime;
				}
				sol.schedule[task1] = machine1;
				sol.schedule[i] = machine2;
			}
		}

		if (task2 == -1)
		{
			/*All tasks are scheduled at the same machine*/
			/*A random movement is performed*/
			local_move(solver, sol);
		}
		else sol.perform_swap (task1, task2);
	}

	void local_f_rebalance (const Solver& solver, Solution& sol)
	{
		/* ---- REBALANCE PROCESS ---- */
		bool found = false;

		// -- Transform schedule representation
		matrix<int> schedule;
		sol.transform_schedule_representation (schedule);

		// -- Select a machine that is loaded the most (considering flowtime)
		double max_fl = minus_infinity;
		double flowtime = 0;
		double old_flowtime2=0;
		int machine1 = 0;
		int machine2 = 0;

		/*Search for the most loaded machine*/
		for (int i=0; i<solver.problem().number_machines; i++)
		{
			flowtime = sol.get_flowtime(i);
			if (flowtime > max_fl)
			{
				machine1 = i;
				max_fl = flowtime;
			}
		}

		vector<int> tasks = schedule[machine1];
		random_shuffle (tasks.begin(), tasks.end(), rand_n);

		// -- Randomly select a task (task2) from another machine
		int task1;
		int task2= sol.select_task_from_another_machine(machine1,schedule);

		if (task2 != -1)
		{
			machine2 = sol.schedule[task2];
			old_flowtime2 = sol.get_flowtime(machine2);
			// -- Randomly find a task in machine1 that if changed for task2
			// -- will reduce flowtime in machine1.
			// -- Moreover, whith probability lsearch_extra_parameter(),
			// -- flowtime(machine2)+flowtime(machine1) must be lower than max_fl+old_flowtime(machine2)

			unsigned int i = 0;
			bool strong_b = (rand_(solver.setup().lsearch_extra_parameter()));
			while ( (i < tasks.size()) && (!found) )
			{
				task1 = tasks[i];
				/*Swap is checked*/
				sol.perform_swap(task1, task2);
				flowtime = sol.get_flowtime(machine1);

				if (flowtime < max_fl)
				{
					if (strong_b)
					{
						double flowtime2 = sol.get_flowtime(machine2);
						if (flowtime2+flowtime <= old_flowtime2+max_fl)
							found = true;
						else sol.perform_swap(task1, task2);
					}
					else found = true;
				}
				else sol.perform_swap(task1, task2);
				++i;
			}
		}
		if (!found)
		/*The most flowtime reducing movement is performed*/
		{
			random_shuffle (tasks.begin(), tasks.end(), rand_n);
			task1 = tasks.front();
			double ftime_diff = 0;
			double old_fl = 0;
			machine2 = -1;

			for (int i=0; i<solver.problem().number_machines; i++)
			{
				if (i != machine1)
				{
					old_fl = sol.get_flowtime(i);
					sol.schedule[task1] = i;
					flowtime = sol.get_flowtime(i);

					if (old_fl - flowtime > ftime_diff)
					{
						ftime_diff = old_fl - flowtime;
						machine2 = i;
					}
				}
			}
			if (machine2 != -1) sol.perform_move(task1, machine2);
		}
	}

	void local_empt_ressource_rebalance (const Solver& solver, Solution& sol)
	{
		// -- select a machine that is loaded the least
		double min_compl = plus_infinity;
		int machine1 = 0;
		for (int i=0; i<solver.problem().number_machines; i++)
		{
			if (sol.completion[i] < min_compl)
			{
				min_compl = sol.completion[i];
				machine1 = i;
			}
		}

		/* ---- REBALANCE PROCESS ---- */
		bool found = false;

		// -- Transform schedule representation
		matrix<int> schedule;
		sol.transform_schedule_representation (schedule);

		int machine2;

		// -- Randomly select a task (task2) from another machine
		int task1;
		int task2= sol.select_task_from_another_machine(machine1,schedule);

		if (task2 != -1)
		{
			// -- Randomly find a task in machine1 with less etc in machine2 than task2
			// -- Moreover, whith probability lsearch_extra_parameter(),
			// -- completion time for machine1 must be <= than the makespan
			machine2 = sol.schedule[task2];
			vector<int> tasks = schedule[machine1];

			/*machine1 may have no task assigned*/
			if (tasks.size() > 0)
			{
				random_shuffle (tasks.begin(), tasks.end(), rand_n);
				unsigned int i = 0;
				bool strong_b = (rand_(solver.setup().lsearch_extra_parameter()));

				while ( (i < tasks.size()) && (!found) )
				{
					task1 = tasks[i];
					if ( solver.problem().etc[task1][machine2] <  solver.problem().etc[task2][machine2] )
					{
						if (strong_b)
						{
							double completion1 = sol.completion[machine1] -
								solver.problem().etc[task1][machine1]     +
								solver.problem().etc[task2][machine1];
							if (completion1 <= sol.local_makespan)
							{
								found = true;
								sol.perform_swap(task1, task2);
							}
						}
						else
						{
							found = true;
							sol.perform_swap(task1, task2);
						}
					}
					++i;
				}
			}
		}
		if (!found)
		{
			/*In case all tasks are assigned to machine1 (because of ready_times from others)
			 *nothing is done*/
			if (task2 != -1) sol.perform_move(task2, machine1);
		}
	}

	void local_empt_ressource_fl_rebalance (const Solver& solver, Solution& sol)
	{
		// -- select a machine that is loaded the least considering flowtime
		double min_flowtime = plus_infinity;
		int machine1 = 0;

		for (int i=0; i<solver.problem().number_machines; i++)
		{
			double fl_act = sol.get_flowtime(i);
			if (fl_act < min_flowtime)
			{
				min_flowtime = fl_act;
				machine1 = i;
			}
		}

		/* ---- REBALANCE PROCESS ---- */
		bool found = false;

		// -- Transform schedule representation
		matrix<int> schedule;
		sol.transform_schedule_representation (schedule);

		int machine2;

		// -- Randomly select a task (task2) from another machine
		int task1;
		int task2= sol.select_task_from_another_machine(machine1,schedule);
		double new_flowtime1 = 0;
		double flowtime2 = 0;
		double new_flowtime2 = 0;

		if (task2 != -1)
		{
			// -- Randomly find a task in machine1 wich improves flowtime in machine2 if swaped by task2
			// -- Moreover, whith probability lsearch_extra_parameter(),
			// -- new flowtime must be lower than old one
			machine2 = sol.schedule[task2];
			flowtime2 = sol.get_flowtime(machine2);
			vector<int> tasks = schedule[machine1];
			/*machine1 may have no task assigned*/
			if (tasks.size() > 0)
			{
				random_shuffle (tasks.begin(), tasks.end(), rand_n);

				unsigned int i = 0;
				bool strong_b = (rand_(solver.setup().lsearch_extra_parameter()));
				while ( (i < tasks.size()) && (!found) )
				{
					task1 = tasks[i];
					sol.perform_swap(task1, task2);
					new_flowtime2 = sol.get_flowtime(machine2);

					if (new_flowtime2 <= flowtime2)
					{
						if (strong_b)
						{
							new_flowtime1 = sol.get_flowtime(machine1);
							if (new_flowtime1 + new_flowtime2 <= min_flowtime + flowtime2) found = true;
							else sol.perform_swap(task1, task2);
						}
						else found = true;
					}
					else sol.perform_swap(task1, task2);
					++i;
				}
			}
		}
		if (!found)
		{
			/*In case all tasks are assigned to machine1 (because of ready_times from others)
			 *nothing is done*/
			if (task2 != -1) sol.perform_move (task2,machine1);
		}
	}

	void local_short_hop (const Solver& solver, Solution& sol)
	{
		/*Order permutation of tasks and machines*/
		vector<int> task_perm1; /*Tasks corresponding to most loaded machines*/
		vector<int> task_perm2; /*Tasks corresponding to emptiest machines*/
		vector<int> machine_perm1 (0); /*most loaded machines permutation*/
		vector<int> machine_perm2 (0); /*emptiest machines permutation*/

		sol.fill_machines_permutation (machine_perm1, machine_perm2);

		matrix<int> schedule2;
		sol.transform_schedule_representation (schedule2);

		for (unsigned int j=0; j<machine_perm1.size(); j++)
			for(unsigned int i=0; i<schedule2[machine_perm1[j]].size(); i++)
				task_perm1.push_back(schedule2[machine_perm1[j]][i]);

		for (unsigned int j=0; j<machine_perm2.size(); j++)
			for(unsigned int i=0; i<schedule2[machine_perm2[j]].size(); i++)
				task_perm2.push_back(schedule2[machine_perm2[j]][i]);

		random_shuffle(task_perm1.begin(), task_perm1.end(), rand_n);
		random_shuffle(task_perm2.begin(), task_perm2.end(), rand_n);
		random_shuffle(machine_perm1.begin(), machine_perm1.end(), rand_n);
		random_shuffle(machine_perm2.begin(), machine_perm2.end(), rand_n);

		/*Iteration of different pairs*/
		int not_improving_hops = 0;
		int it = 0;
		float total = solver.setup().lsearch_extra_parameter();
		double max_ni_it = solver.setup().nb_ls_not_improving_iterations();

		unsigned int ntask1 = 0;
		while (it < total && not_improving_hops < max_ni_it && ntask1 < task_perm1.size())
		{
			unsigned int nmachine1 = 0;
			while (it < total && not_improving_hops < max_ni_it && nmachine1 < machine_perm1.size())
			{
				unsigned int ntask2 = 0;
				while (it < total && not_improving_hops < max_ni_it && ntask2 < task_perm2.size())
				{
					unsigned int nmachine2 = 0;
					while (it < total && not_improving_hops < max_ni_it && nmachine2 < machine_perm2.size())
					{
						int task1 = task_perm1[ntask1];
						int task2 = task_perm2[ntask2];
						int old_m1 = sol.schedule[task1];
						int old_m2 = sol.schedule[task2];
						int new_m1 = machine_perm1[nmachine1];
						int new_m2 = machine_perm2[nmachine2];

						double old_max_compl = max (max(sol.completion[old_m1], sol.completion[old_m2]),
													max(sol.completion[new_m1], sol.completion[new_m2]));

						sol.perform_move (task1, new_m2);
						sol.perform_move (task2, new_m1);

						double new_max_compl = max (max(sol.completion[old_m1], sol.completion[old_m2]),
													max(sol.completion[new_m1], sol.completion[new_m2]));

						if (new_max_compl <= old_max_compl)
						/*This hop is considered as an improvement*/
							not_improving_hops = 0;
						else
						{
							sol.perform_move (task1, old_m1);
							sol.perform_move (task2, old_m2);
							not_improving_hops++;
						}

						it++;
						nmachine2++;
					}
					ntask2++;
				}
				nmachine1++;
			}
			ntask1++;
		}
	}

        void variable_nbh_search (const Solver& solver, Solution& sol)
	/*'lsearch_extra_parameter' different task moves are performed*/
	{
		int nb_moves = int (solver.setup().lsearch_extra_parameter());
		if (nb_moves > solver.problem().number_tasks) nb_moves = solver.problem().number_tasks;
		if (nb_moves < 1) return;

		vector<bool> moved_tasks(solver.problem().number_tasks, false);
		int t, m;

		for (int i=0; i<nb_moves; i++)
		{
			t = rand_(0, solver.problem().number_tasks-1);
			while(moved_tasks[t] == true) t = rand_(0, solver.problem().number_tasks-1);

			m = rand_(0, solver.problem().number_machines-1);
			while(sol.schedule[t] == m) m = rand_(0, solver.problem().number_machines-1);

			sol.perform_move(t, m);
			moved_tasks[t] = true;
		}
	}

	void local_tabu_hop (const Solver& solver, Solution& sol)
	/*Hibridation with Tabu Search skeleton*/
	{
		int not_improving_hops = 0;
		double max_ni_it = solver.setup().nb_ls_not_improving_iterations();

		/**************************************  TS Problem  **********************************************/
		TabuSearch::Problem TS_probl = TabuSearch::Problem();
		TS_probl.set_problem (solver.problem().etc,	solver.problem().ready_times, solver.problem().now);

		/**************************************  TS Setup  ************************************************/
		TabuSearch::Setup TS_setup = TabuSearch::Setup();
		TS_setup.set_max_tabu_status ((int) solver.problem().number_machines*1.5);

		if (max_ni_it >solver.setup().lsearch_extra_parameter())
		{
			int aux = (int) (solver.setup().lsearch_extra_parameter()/2);
			if (aux < 1) aux = 1;
			TS_setup.set_max_repetitions (aux);
		}
		else TS_setup.set_max_repetitions (max(1.0,max_ni_it-1));

		double d_aux = log10(solver.problem().number_tasks)/log10(2);

		TS_setup.set_nb_intensifications (d_aux);
		TS_setup.set_nb_diversifications (d_aux);
		TS_setup.set_elite_size (10);

		d_aux = (TS_setup.max_tabu_status()/2) - (log10(TS_setup.max_tabu_status())/log10(2));
		TS_setup.set_aspiration_value (d_aux);
		TS_setup.set_max_load_interval (1.00);
		TS_setup.set_percent_min_load (0.75);
		TS_setup.set_tabu_size (max( (TS_setup.nb_intensifications()+TS_setup.nb_diversifications()+1)*solver.setup().lsearch_extra_parameter(), (float) 1));

		/**********************************  TS Initial Solution  ********************************************/
		TabuSearch::Solution TS_sol = TabuSearch::Solution(TS_probl);
		TS_sol.schedule = sol.schedule;
		TS_sol.completions = sol.completion;

		vector<double> etc_tasks(solver.problem().number_tasks, 0);
		sol.transform_schedule_representation (TS_sol.schedule_);

		for (int i = 0; i<solver.problem().number_machines; i++)
		{
			for (unsigned int j=0; j<TS_sol.schedule_[i].size(); j++)
				etc_tasks[TS_sol.schedule_[i][j]] = solver.problem().etc[TS_sol.schedule_[i][j]][i];

			quick_sort (etc_tasks, TS_sol.schedule_[i], 0, TS_sol.schedule_[i].size()-1);
			for (unsigned int j=0; j<TS_sol.schedule_[i].size(); j++)
				TS_sol.positions_[TS_sol.schedule_[i][j]] = j;
		}
		TS_sol.update_fitness();
		TabuSearch::Solution TS_sol_aux = TabuSearch::Solution(TS_sol);

		/**************************************  TS Solver  **********************************************/
		TabuSearch::Solver_Seq TS_solver(TS_probl, TS_setup);
		TS_solver.set_current_solution (TS_sol);

		int i=0;
		while (i<solver.setup().lsearch_extra_parameter() && not_improving_hops < max_ni_it)
		{
			TS_solver.perform_one_phase();
			TS_sol_aux = TS_solver.best_solution();

			if (TS_sol_aux.is_better_than(TS_sol))
			{
				not_improving_hops = 0;
				TS_sol = TS_sol_aux;
			}
			else
			{
				not_improving_hops++;
			}
			i++;
		}

		/**************************************  TS Found Solution  *******************************************/
		sol.schedule = TS_sol.schedule;
		sol.completion = TS_sol.completions;
	}

	void Local_Search_Engine::apply_to (Solution& sol)
	{
		assert((_solver.setup().local_search_choice() >= 0) && (_solver.setup().local_search_choice() < 16));
		static LocalSearchFunctionsType SelectFunctions[] = { &local_move, &steepest_local_move, &local_swap,
			&steepest_local_swap, &local_rebalance,
			&deep_local_rebalance, &local_mct_move, &local_mct_swap,
			&local_mft_move, &local_mft_swap, &local_f_rebalance,
			&local_empt_ressource_rebalance,
			&local_empt_ressource_fl_rebalance, &local_short_hop,
			&variable_nbh_search, &local_tabu_hop};

		/********/
		#ifdef LS_TEST
			float _ls_time = 0;
			float _gtime = used_time();
			*(_solver.setup().trace()) << 0 << " " << sol.local_makespan << " "
				<< sol.local_avg_utilization << " "
			<< sol.flowtime << endl;
		#endif
		/********/

		int it = 0;
		int ni_it = 0;
		Solution new_sol(sol);

		/*Hill Climbing iterations*/
		while ((it < _solver.setup().nb_local_search_iterations())
			&& ni_it < _solver.setup().nb_ls_not_improving_iterations())
		{
			/*Neighbor solution is generated*/
			SelectFunctions[_solver.setup().local_search_choice()] (_solver, new_sol);


			/*LS_improvement only considers makespan to evaluate improvement, flowtime is only calculated
				in case where both solutions have same makespan. This avoids usless flowtime calculations thus
				being more efficient*/
			/*if new solution is accepted flowtime must be calculated*/
			if (LS_improvement(new_sol,sol))
			{
				new_sol.compute_flowtime();
				sol = new_sol;
				ni_it = 0;
			}
			else
			{
				new_sol = sol;
				ni_it++;
			}
			it++;

			/********/
			#ifdef LS_TEST
				_ls_time += used_time(_gtime);
				*(_solver.setup().trace()) << it << " " << sol.local_makespan << " "
					 << sol.local_avg_utilization << " "
					 << sol.flowtime << endl;
			#endif
			/********/
		}

		sol.update_feasible();
		sol.update_fitness();

		/********/
		#ifdef LS_TEST
			_ls_time += used_time(_gtime);
			*(_solver.setup().trace()) << it << " " << sol.local_makespan << " "
				<< sol.local_avg_utilization << " "
			<< sol.flowtime << endl << endl;
			*(_solver.setup().trace()) << "Makespan: " << sol.local_makespan << endl;
			*(_solver.setup().trace()) << "AvgUtil: " << sol.local_avg_utilization << endl;
			*(_solver.setup().trace()) << "Flowtime: " << sol.flowtime << endl;
			*(_solver.setup().trace()) << "TimeBest: " << _ls_time << endl;
			*(_solver.setup().trace()) << "TotalTimeSpent: " << _ls_time << endl;
		#endif
		/********/
	}

	

	

	// *********************************************************************
	// Solution_Changes
	// *********************************************************************
	Solution_Changes::Solution_Changes (const Problem& pbm)
		:	_pbm(pbm), _schedule_changes(vector<vector_changes>(0))
	{
	}

	Solution_Changes::~Solution_Changes ()
	{
	}

	opacket2& operator<< (opacket2& op, const Solution_Changes& sol_cha)
	{
		op << sol_cha._new_local_makespan << sol_cha._new_flowtime << sol_cha._new_local_avg_utilization
		   << sol_cha._new_fitness << sol_cha._new_feasible << (int) sol_cha._schedule_changes.size();

		for (unsigned int i=0; i<sol_cha._schedule_changes.size(); i++)
			op << sol_cha._schedule_changes[i].pos << sol_cha._schedule_changes[i].new_val;

		op << sol_cha._last_machine;

		return op;
	}

	ipacket2& operator>> (ipacket2& ip, Solution_Changes& sol_cha)
	{
		int n_changes;
		ip >> sol_cha._new_local_makespan >> sol_cha._new_flowtime >> sol_cha._new_local_avg_utilization
		   >> sol_cha._new_fitness >> sol_cha._new_feasible >> n_changes;

		sol_cha._schedule_changes.resize(n_changes);
		for (unsigned int i=0; i<sol_cha._schedule_changes.size(); i++)
			ip >> sol_cha._schedule_changes[i].pos >> sol_cha._schedule_changes[i].new_val;

		ip >> sol_cha._last_machine;

		return ip;
	}

	Solution_Changes& Solution_Changes::operator= (const Solution_Changes& sol_cha)
	{
		if (this != &sol_cha)
		{
			_new_local_makespan = sol_cha._new_local_makespan;
			_new_flowtime = sol_cha._new_flowtime;
			_new_local_avg_utilization = sol_cha._new_local_avg_utilization;
			_new_fitness = sol_cha._new_fitness;
			_new_feasible = sol_cha._new_feasible;
			_schedule_changes = sol_cha._schedule_changes;
			_last_machine = sol_cha._last_machine;
		}
		return *this;
	}

	void Solution_Changes::differences(const Solution& old_sol, const Solution& new_sol)
	{
		vector_changes change;

		_new_local_makespan = new_sol.local_makespan;
		_new_flowtime = new_sol.flowtime;
		_new_local_avg_utilization = new_sol.local_avg_utilization;
		_new_fitness = new_sol.fitness();
		_new_feasible = new_sol.feasible();

		_schedule_changes.resize(0);
		for (int i=0; i<_pbm.number_tasks; i++)
		{
			if (old_sol.schedule[i] != new_sol.schedule[i])
			{
				change.pos = i;
				change.new_val = new_sol.schedule[i];
				_schedule_changes.push_back(change);
			}
		}

		_last_machine = 0;
		for (int i=1; i<_pbm.number_machines; i++)
		{
			if (new_sol.completion[i] > new_sol.completion[_last_machine])
				_last_machine = i;
		}
	}

	void Solution_Changes::accumulate(const Solution& old_sol, const Solution& new_sol)
	{
		vector_changes change;

		_new_local_makespan = new_sol.local_makespan;
		_new_flowtime = new_sol.flowtime;
		_new_local_avg_utilization = new_sol.local_avg_utilization;
		_new_fitness = new_sol.fitness();
		_new_feasible = new_sol.feasible();

		/*Redundant elements are removed from _schedule_changes*/
		vector<vector_changes>::iterator it = _schedule_changes.begin();
		int index = 0;
		while (it != _schedule_changes.end())
		{
			if (it->new_val != new_sol.schedule[it->pos])
			{
				_schedule_changes.erase(it);
				it = _schedule_changes.begin() + index;
			}
			else
			{
				it++;
				index ++;
			}
		}

		/*New changes are added into _schedule_changes*/
		for (int i=0; i<_pbm.number_tasks; i++)
		{
			if (old_sol.schedule[i] != new_sol.schedule[i])
			{
				change.pos = i;
				change.new_val = new_sol.schedule[i];
				_schedule_changes.push_back(change);
			}
		}

		_last_machine = 0;
		for (int i=1; i<_pbm.number_machines; i++)
		{
			if (new_sol.completion[i] > new_sol.completion[_last_machine])
				_last_machine = i;
		}
	}

	void Solution_Changes::apply_to(Solution &sol)
	{
		sol.local_makespan = _new_local_makespan;
		sol.local_avg_utilization = _new_local_avg_utilization;
		sol.flowtime = _new_flowtime;
		sol._fitness = _new_fitness;
		sol._feasible = _new_feasible;

		for (unsigned int i=0; i<_schedule_changes.size(); i++)
		{
			assert(_schedule_changes[i].pos < _pbm.number_tasks);
			assert(_schedule_changes[i].new_val < _pbm.number_machines);
			sol.perform_move (_schedule_changes[i].pos, _schedule_changes[i].new_val);
		}
		/*****************************************************************************************/
		/* Important note: Due to tha fact that completion time values are updated independently */
		/* from received makespan value (precondition imposes that it is correct)			     */
		/* we must ensure that max completion time corresponds exactly to makespan value	     */
		/* (because Solution_Changes sender may have a different precision from Receiver)	     */
		/* Other operators need max_completion and makespan to be the same with error of 0		 */
		/* using the precision of the current processor (this is done in order to avoid fitness  */
		/* update).																				 */
		/*****************************************************************************************/
		sol.completion[_last_machine] = _new_local_makespan;
	}

	/***************** RECOMBINE given possibilities *********************/
	Solution recombine_one_point (const Solver& solver, vector<Solution>& sols)
	{
		if (solver.problem().number_tasks <= 1) return sols[0];
		/*Ensures that the number of possible cuts is greater than the number of cuts to be done*/
		int nb_cuts = int (min(sols.size()-1, solver.problem().number_tasks-1));
		Solution sol(solver.problem());
		vector<int> cuts(nb_cuts);
		int pos;

		vector<bool> chosed(solver.problem().number_tasks, false);
		/*Generating the different cuts*/
		for (unsigned int i=0; i<cuts.size(); i++)
		{
			cuts[i] = rand_(1, solver.problem().number_tasks -1);
			while (chosed[cuts[i]]== true)
				cuts[i] = rand_(1, solver.problem().number_tasks -1);
			chosed[cuts[i]] = true;
		}
		sort(cuts.begin(), cuts.end());

		/*Generating the new solution*/
		pos = 0;
		for (unsigned int i=0; i<cuts.size(); i++)
		{
			for (int j=pos; j<cuts[i]; j++)
			{
				sol.schedule[j] = sols[i].schedule[j];
			}
			pos = cuts[i];
		}

		for (unsigned int j=pos; j<sol.schedule.size(); j++)
		{
			sol.schedule[j] =sols[sols.size()-1].schedule[j];
		}
		sol.update_feasible();
		sol.update_fitness();

		return sol;
	}

	Solution recombine_uniform (const Solver& solver, vector<Solution>& sols)
	{
		Solution sol(solver.problem());
		/*vector values are randomly taken from one solution but from the corresponding position*/
		for (unsigned int i=0; i<sol.schedule.size(); i++)
			sol.schedule[i] = sols[rand_(0,sols.size()-1)].schedule[i];

		sol.update_feasible();
		sol.update_fitness();
		return sol;
	}

	Solution recombine_fitness_based (const Solver& solver, vector<Solution>& sols)
	{
		Solution sol(solver.problem());
		vector<double> fitness_ranking (sols.size());
		double fitness_ranking_sum = 1;
		double sum=0;

		for (unsigned int i=0; i<sols.size(); i++)
		{
			fitness_ranking[i] = fitness_ranking_sum;
			fitness_ranking_sum += sols[i].fitness()+1;
			sum += sols[i].fitness()+1;
		}

		if (solver.problem().direction() == Minimize)
		{
			fitness_ranking_sum = 0;
			for (unsigned int i=0; i<sols.size(); i++)
			{
				fitness_ranking[i] = fitness_ranking_sum;
				fitness_ranking_sum += 1-((sols[i].fitness()+1)/sum);
			}

			for (unsigned int i = 0; i<sol.schedule.size(); i++)
			{
				double rnd = rand_();
				rnd = rnd*fitness_ranking_sum;
				int index = binary_locate (fitness_ranking, rnd);
				sol.schedule[i] = sols[index].schedule[i];
			}
		}
		else
		{
			for (unsigned int i = 0; i<sol.schedule.size(); i++)
			{
				double rnd    = (double)rand_(1,fitness_ranking_sum);
				int index = binary_locate (fitness_ranking, rnd);
				sol.schedule[i] = sols[index].schedule[i];
			}
		}
		sol.update_feasible();
		sol.update_fitness();
		return sol;
	}

	/***************** MUTATE given possibilities ************************/
    void mutate_move (const Solver& solver, Solution& sol)
    {
		if (solver.problem().number_machines == 1) return;

		/* Randomly select a task from a machine and moves it to another */
		int task = rand_(0, solver.problem().number_tasks - 1);
		int machine_src = sol.schedule[task];
		int machine_dst = rand_(0, solver.problem().number_machines - 1);

		while (machine_dst == machine_src)
			machine_dst = rand_(0, solver.problem().number_machines - 1);

		// Perform Move (task, machine_dst);
		sol.schedule[task] = machine_dst;
	}

	void mutate_swap (const Solver& solver, Solution& sol)
	{
		/* Transform schedule representation */
		matrix<int> schedule;
		sol.transform_schedule_representation (schedule);

		/* Randomly selects a task */
		int task1    = rand_(0, solver.problem().number_tasks - 1);
		int machine = sol.schedule[task1];

		/* Randomly select a task from another machine, and swaps them */
		int task2= sol.select_task_from_another_machine(machine,schedule);
		if (task2 != -1)
		{
			// Perform Swap (task1, task2);
			int tmp = sol.schedule[task1];
			sol.schedule[task1] = sol.schedule[task2];
			sol.schedule[task2] = tmp;
		}
	}

    void mutate_both (const Solver& solver, Solution& sol)
	{
		if (rand_(solver.setup().mutate_extra_parameter()))
		{
			mutate_move (solver, sol);
		}
		else
		{
			mutate_swap (solver, sol);
		}
	}

    void mutate_rebalancing (const Solver& solver, Solution& sol)
    {
		/* ---- REBALANCE PROCESS ---- */
			bool found = false;
		// -- Important: Makespan value must be exactly the max completion value.
		// -- using some parallel modes does not garantees this due to different precision
		// -- systems between machines, for this reason makespan is again calculated
		sol.compute_makespan();
		// -- Transform schedule representation
		matrix<int> schedule;
		sol.transform_schedule_representation (schedule);

		// -- Randomly select a machine that is loaded the most
		vector<int> most_loaded_machines;
		sol.compute_most_loaded_machines (schedule, most_loaded_machines);
		int machine1 =  most_loaded_machines.front();
		int machine2;
		// -- Randomly select a task (task2) from another machine
		int task1;
		int task2= sol.select_task_from_another_machine(machine1,schedule);

		if (task2 != -1)
		{
			// -- Randomly find a task in machine1 with more etc than task2
			// -- Moreover, whith probability mutate_extra_parameter(),
			// -- makespan for machine2 must be <= than the makespan
			machine2          = sol.schedule[task2];
			vector<int> tasks = schedule[machine1];
			random_shuffle (tasks.begin(), tasks.end(), rand_n);
			unsigned int i = 0;
			bool strong_b = (rand_(solver.setup().mutate_extra_parameter()));
			while ( (i < tasks.size()) && (!found) )
			{
				task1 = tasks[i];
				if ( solver.problem().etc[task1][machine1]    >=
					solver.problem().etc[task2][machine1] )
				{
					if (strong_b)
					{
						double completion2 = sol.completion[machine2] -
							solver.problem().etc[task2][machine2]     +
							solver.problem().etc[task1][machine2];
						if (completion2 <= sol.local_makespan)
						{
							found = true;
							int tmp = sol.schedule[task1];
							sol.schedule[task1] = sol.schedule[task2];
							sol.schedule[task2] = tmp;
						}
					}
					else
					{
						found = true;
						int tmp = sol.schedule[task1];
						sol.schedule[task1] = sol.schedule[task2];
						sol.schedule[task2] = tmp;
					}
				}
				++i;
			}
		}
		if (found) return;

		// -- Transform schedule representation
		sol.transform_schedule_representation (schedule);

		// -- Randomly select a machine that is loaded the most
		sol.compute_most_loaded_machines (schedule, most_loaded_machines);
		machine1 = most_loaded_machines.front();

		// -- Compute tasks from machine1
		vector<int> tasks = schedule[machine1];
		random_shuffle (tasks.begin(), tasks.end(), rand_n);
		// -- Try to move a task (once) to reduce the completion time
		unsigned int i = 0; int j = 0;
		while ( (i < tasks.size()) && (!found) )
		{
			task1 = tasks[i];  j = 0;
			while ( (j < sol._problem.number_machines) && (!found) )
			{
				if (j != machine1)
				{
					double completion2  =  sol.completion[j] +
						sol._problem.etc[task1][j];
					if (completion2 <= sol.local_makespan)
					{
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
		if (!found || rand_(0.15))
		{
			if (rand_(0.50))
			{
				mutate_swap (solver,sol);
			}
			else
			{
				mutate_move (solver, sol);
			}
		}
	}

	// *********************************************************************
	// External Methods
	// *********************************************************************
	typedef Solution (*RecombineFunctionsType) (const Solver& solver, vector<Solution>& sols);
	
	bool terminateQ (const Solver& solver)
	{ 
		return (solver.generation()>=solver.setup().nb_generations() 
			|| solver.current_time_spent() >= solver.setup().max_time_to_spend());
	}
	
	Solution recombine (const Solver& solver, vector<Solution>& sols)
	{		
		assert((solver.setup().cross_choice() >= 0) && (solver.setup().cross_choice() <= 2));
		static RecombineFunctionsType RecombineFunctions[] =
		{  &recombine_one_point, &recombine_uniform, &recombine_fitness_based };
		if (sols.size() < 2) return sols[0];
		else return RecombineFunctions[solver.setup().cross_choice()] (solver, sols);       
	}
	
	// *********************************************************************
	// Problem
	// *********************************************************************
	Problem::Problem ()
	{
		number_tasks    = 0;
		number_machines = 0;
		workload     =  vector<double> (0);
		mips         =  vector<double> (0);
		now          =  0.0;
		ready_times  =  vector<double> (0);
		etc          =  matrix<double> (0,0);
	}
	
	Problem::~Problem()
	{
	}
	
	ostream& operator<< (ostream& os, const Problem& pbm) 
	{
		os << _repeat_ ("-",28) << " PROBLEM DEFINITION "
		<< _repeat_ ("-",28) << endl;
		os << " . Number of tasks: "   << pbm.number_tasks << endl;
		os << " . Workload for each task: ";
		for (int i = 0; i < pbm.number_tasks; ++i)
		{
			os << pbm.workload[i] << " ";
		} 
		os << endl;
		
		os << " . Number of machines: " << pbm.number_machines << endl;
		os << " . Mips for each machine: ";
		for (int i = 0; i < pbm.number_machines; ++i) 
		{
			os << pbm.mips[i] << " ";
		} 
		os << endl;
		
		os << " . Current simulation time: " << pbm.now << endl;
		os << " . Ready-time for each machine: ";
		for (int i = 0; i < pbm.number_machines; ++i) 
		{
			os << pbm.ready_times[i] << " ";
		}
		os << endl;
		os << _repeat_ ("-", 76) << endl;
		return os;		
	}
	
	istream& operator>> (istream& is, Problem& pbm) 
	{
		return is;
	}
	
	opacket2& operator<< (opacket2& op, const Problem& pbm) 
	{
		op << pbm.number_tasks << pbm.number_machines << pbm.workload << pbm.mips
			<< pbm.now << pbm.ready_times;
		for (int i=0; i<pbm.number_tasks; i++)
		{
			for (int j=0; j<pbm.number_machines; j++)
			{
				op << pbm.etc[i][j];
			}
		}
		return op;
	}
	
	ipacket2& operator>> (ipacket2& ip, Problem& pbm) 
	{
		ip >> pbm.number_tasks >> pbm.number_machines >> pbm.workload >> pbm.mips
			>> pbm.now >> pbm.ready_times;
		
		pbm.etc = matrix<double> (pbm.number_tasks,pbm.number_machines);
		for (int i=0; i<pbm.number_tasks; i++)
		{
			for (int j=0; j<pbm.number_machines; j++)
			{
				ip >> pbm.etc[i][j];
			}
		}
		pbm.fix_ready_times_to_now();
		return ip;
	}
	
	Direction Problem::direction () const
	{
		return Minimize;
	}
	
	void Problem::set_problem (const vector<double>& task_info,
		const vector<double>& machine_info,
		const vector<double>& ready_time_info,
		const double          current_time) 
	{
		assert ( (machine_info.size() == ready_time_info.size()) &&
			(task_info.size()>0) && (machine_info.size()>0) &&
			(current_time >= 0.0) );
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
		const double          current_time) 
	{
		assert ((etc_info.rows() > 0) && (etc_info.cols() > 0) &&
			(etc_info.cols() == (int) ready_time_info.size()) && current_time >= 0.0);
		etc             = etc_info;
		number_tasks    = etc_info.rows();
		number_machines = etc_info.cols();
		ready_times     = ready_time_info;
		now             = current_time;
		
		/*workload and mips info has to be filled using etc_info respecting relative workloads and
		*mips between tasks and machines respectively. This info is not valid, but it fixes relative 
		*order between tasks (using their workload) and machines (using their mips)*/
		//fill_workload_mips();
		
		// This skeleton can be used independently from the simulator, and
		// thereby sometimes it may be necessary to fix the input data ...
		fix_ready_times_to_now();
	}
	
	void Problem::compute_etc ()  
	{
		etc = matrix<double>(number_tasks, number_machines);
		for (int i = 0; i < number_tasks; ++i) 
		{
			for (int j = 0; j < number_machines; ++j) 
			{
				etc[i][j] = workload[i]/mips[j];
			}
		}
	}
	
	void Problem::fix_ready_times_to_now () 
	{
		for (int i = 0; i < number_machines; ++i) 
		{
			if (ready_times[i] < now) ready_times[i] = now;
		}
	}
	
	void Problem::fill_workload_mips()
	{
		workload.resize(number_tasks);
		for (int i=0; i<number_tasks; i++)
			workload[i] = etc[i][0]*1000;
		
		mips.resize(number_machines);
		for (int i=0; i<number_machines; i++)
			mips[i] = 100000000/etc[0][i]; 
	}
	
	// *********************************************************************
	// Solution
	// *********************************************************************
	typedef void (*MutateFunctionsType) (const Solver&, Solution&);
	
	Solution::Solution (const Problem& pbm) : _problem(pbm)
	{     	
		schedule = vector<int> (pbm.number_tasks, 0);
		completion  = pbm.ready_times;    
		local_makespan  = plus_infinity;
		local_avg_utilization = 0.0;
		flowtime        = plus_infinity;
		energy			= plus_infinity;
		_feasible       = true;
		_fitness        = (pbm.direction()==Minimize) ? plus_infinity
			: minus_infinity;
	}
	
	Solution::Solution (const Solution& sol) : _problem(sol._problem)
	{
		schedule        = sol.schedule;
		completion      = sol.completion;
		local_makespan  = sol.local_makespan;
		local_avg_utilization = sol.local_avg_utilization;
		flowtime        = sol.flowtime;
		energy          = sol.energy;
		_feasible       = sol._feasible;
		_fitness        = sol._fitness;
	}
	
	Solution::~Solution ()
	{
	}
	
	Solution& Solution::operator= (const Solution& sol) 
	{
		if (this != &sol) 
		{
			schedule        = sol.schedule;
			completion      = sol.completion;
			local_makespan  = sol.local_makespan;
			local_avg_utilization = sol.local_avg_utilization;
			flowtime        = sol.flowtime;
			energy			= sol.energy;
			_feasible       = sol._feasible;
			_fitness        = sol._fitness;
		}
		return *this;
	}
	
	bool operator== (const Solution& sol1, const Solution& sol2)
	{
		if((sol1._problem.number_tasks    != sol2._problem.number_tasks)   ||
			(sol1._problem.number_machines != sol2._problem.number_machines)||
			(sol1.fitness()               != sol2.fitness()) ) return false;
		for (int i = 0; i < sol1._problem.number_tasks; ++i) 
		{
			if (sol1.schedule[i] != sol2.schedule[i])   return false;
		} 
		return true;
	}
	
	bool operator!= (const Solution& sol1, const Solution& sol2)
	{
		return !(sol1==sol2);
	}
	
	ostream& operator<< (ostream& os, const Solution& sol) 
	{
		os << _repeat_("-", 33) << " SOLUTION " <<_repeat_("-", 33)<< endl;
		os << " . Number of tasks: "    << sol._problem.number_tasks;
		os << " . Number of machines: " << sol._problem.number_machines;

		os << endl << " . Schedule: " << endl;
		for (int i = 0; i < sol._problem.number_tasks; ++i) 
		{      
			os << _repeat_(" ", 13);
			os << "[Task: " << i << ",Host: "<<sol.schedule[i]<< "]"<<endl;
		}
		os << " . Makespan : " << sol.local_makespan << endl;
		os << " . Avg Utilization : " << sol.local_avg_utilization << endl;
		os << " . Flowtime : " << sol.flowtime << endl;
		os << " . Energy : " << sol.energy << endl;
		os << _repeat_("-", 76) << endl;
		
		return os;
	}
	
	istream& operator>> (istream& is, Solution& sol) 
	{
		return is;
	}
	
	opacket2& operator<< (opacket2& op, const Solution& sol) 
	{
		op << sol._fitness << sol._feasible;
		op << sol.schedule << sol.completion;
		op << sol.local_makespan << sol.local_avg_utilization << sol.flowtime;	
		
		return op;
	}
	
	ipacket2& operator>> (ipacket2& ip, Solution& sol) 
	{
		ip >> sol._fitness >> sol._feasible;
		ip >> sol.schedule >> sol.completion;
		ip >> sol.local_makespan >> sol.local_avg_utilization >> sol.flowtime;
	
		return ip;
	}
	
	double Solution::fitness () const
	{
		return _fitness;
	}
	
	void Solution::update_fitness () 
	{
		/* Compute machine completion times */
		completion = _problem.ready_times;
		for (int i = 0; i < _problem.number_tasks; ++i) 
		{
			completion[schedule[i]]+= _problem.etc[i][schedule[i]];
		}
		
		/* Transform schedule representation */
		matrix<int> schedule2;
		transform_schedule_representation (schedule2);
		
		/* Compute makespans, sum_completions and utilizations */        
			double sum_local_completion = 0;
		int    local_machines       = 0;
		
		local_makespan  =  minus_infinity;
		
		for (int i = 0; i < _problem.number_machines; ++i) 
		{
			if (schedule2[i].size() > 0) 
			{
				sum_local_completion += completion[i];
				local_machines++;
				if (completion[i] > local_makespan) 
				{
					local_makespan = completion[i];
				} 
			}
		} 
		local_avg_utilization = sum_local_completion / (local_makespan*local_machines);
		
		/* Compute minimum flowtime of the schedule (sorting by etc) */
		flowtime = 0;
		vector<double> etc_tasks;
		for (int i = 0; i < _problem.number_machines; ++i) 
		{	 
			etc_tasks.resize (schedule2[i].size());
			for (unsigned int j = 0; j < schedule2[i].size(); ++j) 
			{
				etc_tasks[j] = _problem.etc[schedule2[i][j]][i];
			}
			sort (etc_tasks.begin(), etc_tasks.end());
			
			double completion = _problem.ready_times[i];
			for (unsigned int j = 0; j < etc_tasks.size(); ++j) 
			{
				completion += etc_tasks[j];
				flowtime   += completion;		
			}           
		}
		
		/* Compute energy */
		if( ENERGY_CASE ) {
			Energy* tmp_energy = new Energy(_problem.number_machines, _problem.number_tasks);
			tmp_energy->assignEnergy(_problem.mips,local_makespan,completion,_problem.ready_times);
			if ( ENERGY_CASE == 1 )
				energy = tmp_energy->EnergyI;
			else if ( ENERGY_CASE == 2 )
				energy = tmp_energy->EnergyII;
			else
				energy = tmp_energy->EnergyI;
		}

		#ifdef MULTIOBJECTIVE       
			_fitness = 0.1*local_makespan + 0.9*(flowtime/_problem.number_tasks);            
		#else
			_fitness = local_makespan;        
		#endif        
	}
	
	bool Solution::is_better_than (const Solution& sol) const
	{
		int sign = ((_problem.direction() == Minimize)?(1):(-1));
		#ifdef MULTIOBJECTIVE  
			/* Multiobjective:   k*min makespan + (k-1)*flowtime */
		//return ((sign * _fitness) < (sign * sol._fitness));
		if      ((sign* _fitness) < (sign* sol._fitness))              return true;
		else if ((sign* _fitness) > (sign* sol._fitness))              return false;
		else if (ENERGY_CASE && ((sign* energy) < (sign* sol.energy))) return true;
		else                                                           return false;
		#else    	
			/* Hierarchical:  min flowtime subject to min makespan    */
			/*               _fitness is that case refers to makespan */
		if      ((sign* _fitness) < (sign* sol._fitness)) return true;
		else if ((sign* _fitness) > (sign* sol._fitness)) return false;
		else if ((sign* flowtime) < (sign* sol.flowtime)) return true;
		else if ((sign* flowtime) > (sign* sol.flowtime)) return false;
		else if (ENERGY_CASE && ((sign* energy) < (sign* sol.energy))) return true;
		else                                                           return false;
		#endif    	
	}
	
	bool Solution::feasible () const 
	{
		return _feasible;
	}
	
	void Solution::update_feasible () 
	{   
	}
	
	void Solution::mutate (const Solver& solver)
	{     
		assert((solver.setup().mutate_choice() >= 0) && (solver.setup().mutate_choice() <= 3));
		static MutateFunctionsType MutateFunctions[] =
			{ &mutate_move, &mutate_swap, &mutate_both, &mutate_rebalancing };
		MutateFunctions[solver.setup().mutate_choice()] (solver, *this);
		
		update_feasible();
		update_fitness();
	}
	
	/**************** INITIAL SOLUTIONS given possibilities **************/
    void Solution::heuristic (heuristic_type choice) 
	{
		/* Use Initial Solutions */
		InitialSolutions is;
        is.set_problem(_problem.workload,_problem.mips,_problem.ready_times);
        is.set_problem(_problem.etc,_problem.ready_times);
        schedule = is.heuristic(choice);
        
        update_feasible();
        update_fitness();
    }
          
    void Solution::perturbate_randomly () 
	{    
		for (int i = 0; i < _problem.number_tasks; ++i) 
		{
			if (rand_(0.15)) 
			{
				schedule[i] = rand_(0, _problem.number_machines - 1);
            }
        }
		update_feasible();
        update_fitness();
    }

    /************************ Auxiliar functions *************************/
    void Solution::compute_most_loaded_machines (matrix<int>& sched, vector<int>& machines) 
    /*machines will contain those machines with completion_time = makespan*/
	{
        for (int i = 0; i < _problem.number_machines; ++i) 
		{
            if ((completion[i] == local_makespan) && (sched[i].size() > 0))	    
		      machines.push_back (i);
        }	
		random_shuffle (machines.begin(), machines.end(), rand_n);	      
    }

    void Solution::transform_schedule_representation (matrix<int>& sched)
	{  
		sched = matrix<int> (_problem.number_machines, 0);
		for (int i = 0; i < _problem.number_tasks; ++i) 
		{
			sched[schedule[i]].push_back (i);
        }        
    }

    void Solution::transform_schedule_representation (vector<int>& perm, vector<int>& limits)
	{
		matrix<int> sched;
		transform_schedule_representation (sched);
		perm   = vector<int> (_problem.number_tasks, 0);
		int task_index = 0;
        limits = vector<int> (_problem.number_machines, 0);
		
		for (int i = 0; i < _problem.number_machines; ++i) 
		{
			// Be aware of: Maybe it is better not to random_shuffle them
			//random_shuffle (sched[i].begin(), sched[i].end());
			for (unsigned int j = 0; j < sched[i].size(); ++j) 
			{
				perm[task_index] = sched[i][j];
				++task_index;
			}
			limits[i] = task_index;
		}                 
	}
	
	void Solution::apply_schedule_representation(const matrix<int>& sched)
	{
		for (int i = 0; i < _problem.number_machines; ++i) 
		{
			for (unsigned int j = 0; j < sched[i].size(); ++j) 
			{
				schedule[sched[i][j]] = i;
			}
		}
	}

    void Solution::apply_schedule_representation(const vector<int>& perm, const vector<int>& limits)
	{
		int last_pos = 0;
		for (int i = 0; i < _problem.number_machines; ++i) 
		{
			for (int pos = last_pos; pos < limits[i]; ++pos) 
			{
				schedule[perm[pos]] = i;
			}
			last_pos = limits[i];
		}
	}
	
	int Solution::select_task_from_another_machine (int machine, const matrix<int>& sched)
    {       
        vector<int> machines;
		for (int i = 0; i < _problem.number_machines; ++i) 
		{
			if ( (i != machine) && (sched[i].size() > 0))	    
				machines.push_back(i);	      	    
		}
		
		int task = -1;
		if (machines.size() > 0) 
		{            
			random_shuffle (machines.begin(), machines.end(), rand_n);
			int mach     = machines.front();
			int t        = rand_ (0, sched[mach].size() - 1);
			task         = sched[mach][t];                                    
		}
		return task;
	}

	int  Solution::select_best_task_from_another_machine (int machine, const matrix<int>& sched)
	/*Gives the task not assigned in the given machine with the lowest etc for the given machine
	 *-1 is returned if all tasks are assigned in this machine*/
	{
		int best = -1;
		double best_etc = plus_infinity;
		double new_etc;

		for	(int i=0; i<_problem.number_machines; i++)
		{
			if (i != machine)
			{
				for (unsigned int j=0; j<sched[i].size(); j++)
				{
					new_etc = _problem.etc[sched[i][j]][machine] < best_etc;
					if (new_etc < best_etc)
					{
						best = sched[i][j];
						best_etc = new_etc;
					}
				}
			}
		}
		return best;
	}
	
	int  Solution::select_best_task_for_this_machine  (int machine, const vector<int>& tasks)
	/*Select the task from 'tasks' with the lowest etc for the given machine*/
	{
		int best = tasks[0];
		double best_etc = _problem.etc[tasks[0]][machine]; 
		double new_etc;	

		for (unsigned int i=1; i<tasks.size(); i++)
		{
			new_etc = _problem.etc[tasks[i]][machine]; 
			if (new_etc < best_etc)
			{
				best = tasks[i];
				best_etc = new_etc;
			}
		}
		return best;
	}

	void Solution::fill_machines_permutation (vector<int>& loaded, vector<int>& emptiest)
	/*Machines are sorted by their completion times*/
	{
		vector<int> sorted_machines (_problem.number_machines);
		for (int i=0;i<_problem.number_machines; i++)
			sorted_machines[i] = i;

		quick_sort (completion, sorted_machines, 0, _problem.number_machines -1);

		int max_load = (int) (0.1 * (_problem.number_machines));
		if (max_load < 1) max_load = 1;

		int min_load = (int) (0.75 * (_problem.number_machines));
		if (min_load < 1) min_load = 1;

		for (int i=0; i<max_load; i++)
			loaded.push_back(sorted_machines[_problem.number_machines-1-i]);
		for (int i=0; i<min_load; i++)
			emptiest.push_back(sorted_machines[i]);
	}

	inline bool Solution::host_participates (int host) 
    /* Pre: completions must be computed before */
	{
		return ((completion[host] - _problem.ready_times[host]) > 0);
    }


    double Solution::get_flowtime(int machine)
    /*Gives the minimum flowtime at the given machine*/
    {
		vector<double> etc_tasks(0);
		double comp = _problem.ready_times[machine];
		double ftime = 0;
		
		for (unsigned int i=0; i<schedule.size(); i++)
		{
			if (schedule[i] == machine)
				etc_tasks.push_back(_problem.etc[i][machine]);
		}
		
		sort(etc_tasks.begin(), etc_tasks.end());
		
		for (unsigned int i=0; i<etc_tasks.size(); i++)
		{
			comp += etc_tasks[i];
			ftime += comp;
		}
		return ftime;
	}

	void Solution::compute_makespan ()
	/*Pre: completion times must be updated*/
	{
		local_makespan = minus_infinity;
		
		for (int i=0; i < _problem.number_machines; i++)
		{
			if ((host_participates(i) > 0) && (completion[i] > local_makespan))
				local_makespan = completion[i];
		}
	}

    void Solution::compute_flowtime ()
	{
		/* Transform schedule representation */
		matrix<int> schedule2;
		transform_schedule_representation (schedule2);
		
		/* Compute minimum flowtime of the schedule (sorting by etc) */
		flowtime = 0;
		vector<double> etc_tasks;
		for (int i = 0; i < _problem.number_machines; ++i)
		{	 
			etc_tasks.resize (schedule2[i].size());
			for (unsigned int j = 0; j < schedule2[i].size(); ++j)
			{
				etc_tasks[j] = _problem.etc[schedule2[i][j]][i];
			}     
			sort (etc_tasks.begin(), etc_tasks.end());
			
			double completion = _problem.ready_times[i];
			for (unsigned int j = 0; j < etc_tasks.size(); ++j) 
			{
				completion += etc_tasks[j];
				flowtime   += completion;		
			}           
		}    
	}
                                                    
	void Solution::perform_move (int t, int m)
	/*task t is moved to machine m and completion times are updated*/
	{
		completion[schedule[t]] -= _problem.etc[t][schedule[t]];
		completion[m] += _problem.etc[t][m];
		schedule[t] = m;
	}

	void Solution::perform_swap (int t1, int t2)
	/*task t1 and task t2 are swaped and compltion times are updated*/
	{
		int m1 = schedule[t1];
		int m2 = schedule[t2];
		completion[m1] = completion[m1] - _problem.etc[t1][m1] + _problem.etc[t2][m1];
		completion[m2] = completion[m2] - _problem.etc[t2][m2] + _problem.etc[t1][m2];
		schedule[t1] = m2;
		schedule[t2] = m1;
	}
	
	bool improvement (const Solution& s1, const Solution& s2)
	/*Returns true if s1 is better than s2 considering hierarchical criteria
	used for local search mechanism*/
	{
		int sign = ((s1._problem.direction() == Minimize)?(1):(-1));
		
		/* Hierarchical:  min flowtime subject to min makespan    */
		if      ((sign * s1.local_makespan) < (sign * s2.local_makespan)) return true;
		else if ((sign * s1.local_makespan) > (sign * s2.local_makespan)) return false;
		else if ((sign * s1.flowtime) < (sign * s2.flowtime)) return true;
		else return false;    	   	
	}

	bool LS_improvement (Solution& new_s, const Solution& old_s)
	/*Same as improvement. Compares new_s and old_s more efficiently avoiding, if
	 *possible, the evaluation of flowtime. It is used by LSearch Mechanism*/
	/*Pre: new_s has completion_times updated, old_s has flowtime updated*/
	{
		int sign = ((new_s._problem.direction() == Minimize)?(1):(-1));

		new_s.compute_makespan(); //Calculate local_makespan from compl.times
		
		/* Hierarchical:  min flowtime subject to min makespan    */
		if      ((sign * new_s.local_makespan) < (sign * old_s.local_makespan)) return true;
		else if ((sign * new_s.local_makespan) > (sign * old_s.local_makespan)) return false;
		else
		{
			new_s.compute_flowtime();
			if ((sign * new_s.flowtime) < (sign * old_s.flowtime)) return true;
			else return false;    	
		}
	}
	
	// *********************************************************************
	// Population
	// *********************************************************************
	typedef void (*StartFunctionsType) (Population&);
	typedef void (*SelectFunctionsType) (Population&);
	typedef vector<Solution> (*RecSelectFunctionsType) (const Population&, int n, float extra_p);
	
	Population::Population (const Solver& solver)
		:	_solver(solver),		            
		_solutions(solver.setup().population_size()),
		_best_solution(NULL),
		_worst_solution(NULL),
		_best_Mksp_solution(NULL),
		_best_fitness(((solver.problem().direction()==Minimize)? plus_infinity :
		minus_infinity )),
		_worst_fitness(-_best_fitness)
	{
		for (unsigned int i=0; i<_solutions.size(); i++)
			_solutions[i] = new Solution(solver.problem());		
	}
	
	Population::~Population ()
	{
		for (unsigned int i=0; i<_solutions.size(); i++)
		{
			delete _solutions[i];
		}
		_solutions.resize(0);
	}
	
	void Population::initialize ()
	{
		assert(_solver.setup().start_choice()>=0 && _solver.setup().start_choice()<=7);
		static StartFunctionsType StartFunctions[] =
			{   &start_ljfrsjfr, &start_mct, &start_random, &start_all,
			&start_minmin, &start_rc, &start_kpb, &start_all2 };        
		StartFunctions[_solver.setup().start_choice()] (*this);
		return update();
	}
	
	void Population::add (const Solution& sol)
	{
		if (_solutions.size()>0)
		{
			double cost = sol.fitness();	
			if (_solver.setup().add_only_if_better())
			{
				/*'sol' is only added if it's better than worst solution*/	  
				if (sol.is_better_than(*_worst_solution)) 
				{
					_solutions.push_back(new Solution(sol));
					/* _solutions.resize(_solutions.size()+1);
					_solutions[_solutions.size()-1] = new Solution(sol);*/
					if (sol.is_better_than(*_best_solution))
					{
						//_best_solution = _solutions[_solutions.size()-1];
						_best_solution = _solutions.back();
						_best_fitness = cost;
					}
					if (improvement(sol,*_best_Mksp_solution))
					{
						_best_Mksp_solution = _solutions.back();
					}
				}
			}
			else 
			{
				/*'sol' is always added*/
				_solutions.push_back(new Solution(sol));	
				
				if (sol.is_better_than(*_best_solution))
				{
					_best_solution = _solutions.back();	    
					_best_fitness = cost;
				}
				if (_worst_solution->is_better_than(sol))
				{
					_worst_solution = _solutions.back();
					_worst_fitness = cost; 
				}
				if (improvement(sol,*_best_Mksp_solution))
				{
					_best_Mksp_solution = _solutions.back();
				}
			}			
		} 
		else 
		{
			_solutions.resize(1);
			_solutions[0] = new Solution(sol);
			_best_solution = _solutions[0];
			_best_fitness = _solutions[0]->fitness();
			_worst_solution = _best_solution;
			_worst_fitness = _best_fitness;	 
			_best_Mksp_solution = _best_solution;
		}
	}
	
    void Population::add (const vector<Solution>& sols)
    {
		for(unsigned int i=0; i<sols.size(); i++) add(sols[i]);
    }	
		
    void Population::restart ()
    {
		/*This operator will not be used*/
    }
	
	
    int Population::size () const
    {
		return _solutions.size();
    }
	
	
    Solution Population::best () const
    {
		return *_best_solution;
    }

	Solution Population::best_Mksp_solution () const
	{
		return *_best_Mksp_solution;
	}
		
    vector<Solution> Population::select_to_merge (bool at_random)
    {
		int sel = _solver.setup().recombine_selection();

		assert((sel >= 0) && (sel <= 5));
		static RecSelectFunctionsType RecSelectFunctions[] =
        {   &random_sel, &best_sel, &linear_ranking_sel,
            &exp_ranking_sel, &binary_tournament_sel,
            &n_tournament_sel
        };

		if (sel == Memetic::SelectBest 
		 || sel == Memetic::SelectLinearRanking 
		 || sel == Memetic::SelectExpRanking)
		{
			sort();
		}

		vector<Solution> selected(_solver.setup().nb_solutions_to_recombine(), Solution(_solver.problem()));
		selected = RecSelectFunctions[sel] 
					(*this, _solver.setup().nb_solutions_to_recombine(), _solver.setup().rec_selection_extra());
	
		if(_solver.setup().nb_solutions_to_recombine()>0)
		{
			if(at_random == false) 
			{	
				selected[0] = *_best_solution;
			}    
		 }
		
	     return selected;
    }
		
    Solution Population::select_to_mutate ()
    {
		int sel = _solver.setup().mutate_selection();
	
		assert((sel >= 0) && (sel <= 5));
		static RecSelectFunctionsType MutSelectFunctions[] =
        {   &random_sel, &best_sel, &linear_ranking_sel,
            &exp_ranking_sel, &binary_tournament_sel,
            &n_tournament_sel
        };

		if (sel == Memetic::SelectBest 
		 || sel == Memetic::SelectLinearRanking 
		 || sel == Memetic::SelectExpRanking)
			sort();
		vector<Solution> selected(1, Solution(_solver.problem()));
		selected = MutSelectFunctions[sel] 
						(*this, 1, _solver.setup().mut_selection_extra());						
		return selected[0];
    }
	
	
    void Population::select ()
    {
      assert((_solver.setup().select_choice() >= 0) && (_solver.setup().select_choice() <= 5));
      static SelectFunctionsType SelectFunctions[] =
        {   &select_random, &select_best, &select_linear_ranking,
            &select_exp_ranking, &select_binary_tournament,
            &select_n_tournament
        };
        assert((_solver.setup().select_choice()>=0) && (_solver.setup().select_choice()<=5));
		sort();
        SelectFunctions[_solver.setup().select_choice()] (*this);
		update();
    }
	
	
    bool Population::meets_conv_criteria () const
    {
		return false;
    }
	
	void Population::update ()
	{      
		int size1 = size();
		if (size1 == 0) return;
		
		_best_solution = _solutions[0];
		_worst_solution = _best_solution;
		_best_fitness  = _worst_fitness = _solutions[0]->fitness();
		_best_Mksp_solution = _best_solution;	  
		
		for (int i = 1; i < size1; ++i) 
		{
			double f = _solutions[i]->fitness();
			if (_solutions[i]->is_better_than(*_best_solution) ) 
			{
				_best_fitness  = f;
				_best_solution = _solutions[i];
			}
			if (_worst_solution->is_better_than(*_solutions[i]))
			{
				_worst_fitness = f;
				_worst_solution = _solutions[i];
			}
			if (improvement(*_solutions[i],*_best_Mksp_solution))
			{
				_best_Mksp_solution = _solutions[i];
			}
		}
	}
	
	void Population::sort ()
    {   
		std::sort(_solutions.begin(), _solutions.end(), compare_solutions);		
    }
		
    Solution& Population::operator[] (int i_th)
    {
		return *_solutions[i_th];
    }

    Population& Population::operator= (const Population& pop) 
    {
		if (this != &pop) 
		{
			int size1  = size();
			int size2 = pop.size();

            if (size2 == 0) 
			{
				return *this;
			} 
			else if (size1 >= size2) 
			{
				for (int i = size2; i < size1; ++i) delete _solutions[i];
				_solutions.resize(size2);
			} 
			else
			{
				_solutions.resize(size2);
				for (int i = size1; i < size2; ++i) 
				{
					_solutions[i] = new Solution(_solver.problem());
				}
			}
			for (int i = 0; i < size2; ++i) 
			{
				*_solutions[i] = *pop._solutions[i];
				if (pop._solutions[i] == pop._best_solution)
					_best_solution = _solutions[i];
				if (pop._solutions[i] == pop._worst_solution)
					_worst_solution = _solutions[i];
				if (pop._solutions[i] == pop._best_Mksp_solution)
					_best_Mksp_solution = _solutions[i];	
			}            
			_best_fitness  = pop._best_fitness;
			_worst_fitness = pop._worst_fitness;
		}
		return *this;
	}
	          
    ostream& operator<< (ostream& os, const Population& pop)
    {
		os << '(';
		for(int i=0; i<pop.size(); ++i)
		{
			os << pop._solutions[i]->fitness();
			if(i<pop.size()) os << ", ";
		}
		os << ')';
		os << " best: " << *pop._best_solution;
		os << " worst: " << *pop._worst_solution;
		
		return os;
	}	

    
}
