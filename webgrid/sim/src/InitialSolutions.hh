#ifndef __INITIALSOLUTIONS_H__
#define __INITIALSOLUTIONS_H__

#include <iostream>
#include <vector>
#include <set>
#include <math.h>
#include <assert.h>
#include <algorithm>
using namespace std;
#include "mallba.hh"
#include "no_leda.hh"
#include "GAParams.hh"

namespace AdHocHeuristics {

  //********************************************

  class InitialSolutions {
      public:

          void set_problem (const vector<double>& workload,
                            const vector<double>& mips,
                            const vector<double>& ready);

          void set_problem (const matrix<double>& etc,
                            const vector<double>& ready);

          /* To invoke whatever heuristic from this centralized method */
          vector<int> heuristic     (heuristic_type choice);

          /*  Batch mode mapping heuristics */
          vector<int> min_min       ();
          vector<int> max_min       ();
          vector<int> sufferage     ();
          vector<int> relative_cost (double x= 0.5);
          vector<int> ljfr_sjfr     ();

          vector<int> game_min_min_old_broken  (vector<int>& original, int cmp_start, int cmp_end);
          vector<int> game_min_min_fixed  (vector<int>& original, int cmp_start, int cmp_end);
          vector<int> game_min_min_v2 (vector<int>& original, int cmp_start, int cmp_end);
          vector<int> game_max_min  (vector<int>& original, int cmp_start, int cmp_end);
          vector<int> game_sufferage (vector<int>& original, int cmp_start, int cmp_end);

          /* Immediate / batch mode mapping heuristics */
          vector<int> mct           ();
          vector<int> met           ();
          vector<int> sa            (double r_low= 0.6, double r_high=0.9);
          vector<int> kpb           (double percent = 0.80);
          vector<int> olb           ();

          /* Random schedule */
          vector<int> random        ();

          /* Auxiliar methods */
          double makespan    (vector<int>& schedule);
          double utilization (vector<int>& schedule);
          double flowtime    (vector<int>& schedule);
          double match_prox  (vector<int>& schedule);


          vector<int> enviroment(const vector<int> &best_sol, int env_length, enviroment_type choise);
          /*Enviroment selecting*/
          vector<int> enviroment_basic (const vector<int>& schedule, int env_length);
          vector<int> enviroment_min (const vector<int>& best_sol, int env_length);

       private:
          void compute_etc (const vector<double>& workload,
                            const vector<double>& mips);

          vector<double> completion (vector<int>& schedule);

          int              number_tasks;
          int              number_machines;
          matrix<double>   etc;
          vector<double>   ready;
          vector<double>   workload;
          vector<double>   mips;
  };

}
#endif
