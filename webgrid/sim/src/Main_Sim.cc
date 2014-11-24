#include "Main_Sim.hh"


#define NO_STRATEGY             "NONE"
 
#define DEFAULT_TOTAL_TASKS     512
#define DEFAULT_INITIAL_TASKS   512
#define DEFAULT_IATIME          "c(9999999999)"
#define DEFAULT_WORKLOAD        "n(250000000,43750000)"
#define DEFAULT_INITIAL_HOSTS   32
#define DEFAULT_MIPS            "n(1000,175)"
#define DEFAULT_ADD_HOST        "c(9999999999)"
#define DEFAULT_DEL_HOST        "c(9999999999))"
#define DEFAULT_MIN_HOSTS       32
#define DEFAULT_MAX_HOSTS       32
#define DEFAULT_RESCHEDULE      false
#define DEFAULT_STRATEGY        "NGA_scheduler(1,h)" //"MA_scheduler(5,1)" //"SG_scheduler(5,h)"//"simple_metapolicy"
#define DEFAULT_ACTIVATION      "resource_and_time_interval(c(9999999999))"
#define DEFAULT_HOST_SELECTION  "all"
#define DEFAULT_TASK_SELECTION  "all"
#define DEFAULT_HOST_POLICY     "fcfs"
#define DEFAULT_NRUNS           10
#define DEFAULT_SEED            123467890
#define DEFAULT_TRACE           false

#include <stdio.h>
#include <set>
#include <string.h>

int g_run_number = 0;
bool g_print_debug_info = false;

int main (int argc, char *argv[]) {
    int totalTasks, initialTasks; string iaTime; string workLoad;
    int initialHosts; string mips; string addHost; string delHost;
    int minHosts, maxHosts;
    bool reschedule; string strategy; string activation;
    string hostSelection; string taskSelection; string allocPolicy;
    int numRuns; long int seed; bool trace; string traceFile;
    GAParams ga_params;

    if (argc == 1) { cout << "...Using default parameters..." << endl; }

    GAParams hgs_params[4];
    TabuParams tabuParams;
    CMAParams cmaParams;
    MAParams maParams;

    parseArg(argc, argv, totalTasks, initialTasks, iaTime, workLoad,
             initialHosts, mips, addHost, delHost, minHosts, maxHosts,
             reschedule, strategy, activation, hostSelection,taskSelection,
             allocPolicy, numRuns, seed, trace, traceFile, ga_params, hgs_params
             , tabuParams, cmaParams, maParams);


    for ( int i = 0; i < 4; i++ ){

    	if ( ! hgs_params[i].is_set_evolution_steps ){
    		hgs_params[i].evolution_steps = ga_params.evolution_steps;
    	}
    	if ( ! hgs_params[i].is_set_pop_size ){
			hgs_params[i].pop_size = ga_params.pop_size;
		}
    	if ( ! hgs_params[i].is_set_intermediate_size ){
			hgs_params[i].intermediate_size = ga_params.intermediate_size;
		}
    	if ( ! hgs_params[i].is_set_prob_cross ){
			hgs_params[i].prob_cross = ga_params.prob_cross;
		}
    	if ( ! hgs_params[i].is_set_prob_mutate ){
			hgs_params[i].prob_mutate = ga_params.prob_mutate;
		}
    	if ( ! hgs_params[i].is_set_start_choice ){
			hgs_params[i].start_choice = ga_params.start_choice;
		}
    	if ( ! hgs_params[i].is_set_select_choice ){
			hgs_params[i].select_choice = ga_params.select_choice;
		}
    	if ( ! hgs_params[i].is_set_select_extra ){
			hgs_params[i].select_extra = ga_params.select_extra;
		}
    	if ( ! hgs_params[i].is_set_cross_choice ){
			hgs_params[i].cross_choice = ga_params.cross_choice;
		}
    	if ( ! hgs_params[i].is_set_cross_extra ){
			hgs_params[i].cross_extra = ga_params.cross_extra;
		}


    	if ( ! hgs_params[i].is_set_mutate_choice ){
			hgs_params[i].mutate_choice = ga_params.mutate_choice;
		}
    	if ( ! hgs_params[i].is_set_mutate_extra ){
			hgs_params[i].mutate_extra = ga_params.mutate_extra;
		}
    	if ( ! hgs_params[i].is_set_mutation_size ){
			hgs_params[i].mutation_size = ga_params.mutation_size;
		}
		if ( ! hgs_params[i].is_set_replace_if_better ){
			hgs_params[i].replace_if_better = ga_params.replace_if_better;
		}
    	if ( ! hgs_params[i].is_set_replace_generational ){
			hgs_params[i].replace_generational = ga_params.replace_generational;
		}
    	if ( ! hgs_params[i].is_set_env_length ){
			hgs_params[i].env_length = ga_params.env_length;
		}
    	if ( ! hgs_params[i].is_set_env_choice ){
			hgs_params[i].env_choice = ga_params.env_choice;
		}

    }


    //ofstream file;
    //if (trace) { file.open(traceFile, ios::app);
    //             if (!file) error("Main_Sim.cc: Can't open trace file");
    //} else {  file.copyfmt(cout); file.clear(cout.rdstate());
    //          file.basic_ios<char>::rdbuf(cout.rdbuf()); }

    /*------------------------ Output Parameters ------------------------*/
    reportParameters (totalTasks, initialTasks, iaTime, workLoad,
                      initialHosts, mips, addHost, delHost, minHosts,
                      maxHosts, reschedule, strategy, activation,
                      hostSelection, taskSelection, allocPolicy, numRuns,
                      seed, trace, traceFile, ga_params, hgs_params,
                      tabuParams, cmaParams, maParams, cout);

    /*------------------------ Perform Simulation -----------------------*/
    GlobalStatistics gs;
	GlobalStatisticsSummator gsm;
    IndependentRunsStatistics irs;

    for (int i = 1; i <= numRuns; i++) {

    	g_run_number = i;

        // No hace falta definir constructor por copia de GlobalStatistics
        // ni operator= de Accumulator, ya que no trabajan con el heap
        gs = run (totalTasks, initialTasks, iaTime, workLoad, initialHosts,
                  mips, addHost, delHost, minHosts, maxHosts, reschedule,
                  strategy, activation, hostSelection, taskSelection,
                  allocPolicy, seed, trace, traceFile, ga_params, hgs_params,
                  tabuParams, cmaParams, maParams);

        gsm.pushBack(gs);
        if (numRuns > 1) processMultipleRuns (gs, irs);
    }

    /*-------------------------- Output Results -------------------------*/
    //if (numRuns == 1)
    gsm.outputStatistics (cout);
    if (numRuns > 1){
    	irs.outputStatistics (cout);
    }
    return 0;
}

GlobalStatistics run  ( int totalTasks, int initialTasks, string iaTime,
                        string workLoad, int initialHosts, string mips,
                        string addHost, string delHost, int minHosts,
                        int maxHosts, bool reschedule, string strategy,
                        string activation, string hostSelection,
                        string taskSelection, string allocPolicy,
                        long int& seed, bool trace, string traceFile,
                        GAParams ga_params, GAParams (&hgs_params)[4],
                        TabuParams tabuParams, CMAParams cmaParams,
                        MAParams maParams) {

    sranf(seed++);

    GridSimulation sim;
    SchedulerFactory sf;  DistributionFactory df;

    Scheduler*           sched  = new Scheduler();
    SchedulingPolicy*    policy = sf.CreateSchedulingPolicy
    		(strategy, ga_params, hgs_params, tabuParams, cmaParams, maParams);
    SchedulerActivation* activ  = sf.CreateSchedulerActivation (activation);
    HostSelectionPolicy* hostS=sf.CreateHostSelectionPolicy(hostSelection);
    TaskSelectionPolicy* taskS=sf.CreateTaskSelectionPolicy(taskSelection);

    sched->setSchedulingPolicy    (policy);
    sched->setSchedulerActivation (activ);
    sched->setHostSelectionPolicy (hostS);
    sched->setTaskSelectionPolicy (taskS);
    sched->setReschedule          (reschedule);

    Distribution* j1 = df.CreateDistribution (iaTime);
    Distribution* j2 = df.CreateDistribution (workLoad);
    Distribution* h1 = df.CreateDistribution (mips);
    Distribution* h3 = df.CreateDistribution (addHost);
    Distribution* h2 = df.CreateDistribution (delHost);

    sim.setTasks (j1, j2, totalTasks, initialTasks);
    sim.addGrid (h1, h2, h3, minHosts, initialHosts, maxHosts,allocPolicy);
    sim.setScheduler (sched);
    sim.setTrace (trace);
    sim.setTraceFile (traceFile);
    sim.run ();

    delete sched;
    delete policy;
    delete activ;
    delete hostS;
    delete taskS;
    delete j1;
    delete j2;
    delete h1;
    delete h2;
    delete h3;

    return sim.getGlobalStatistics();
}

void usage() {
    cout <<
"Usage: sim -s  <STRATEGY>  [options]"                           <<endl <<
//"or:    sim -s '<STRATEGY>' [options]"   << endl <<
//"   Use later method if your command line complains about brackets () used." << endl <<
"" << endl <<
"<STRATEGY> must be one of the following:"<<endl<<
"  simple_metapolicy            Just assign tasks to machines (t1=m1, t2=m2, etc)"<<endl<<
"  AdHoc_scheduler(<HEURISTIC>) Depends on <HEURISTIC>, see below."<<endl<<
"  GA_scheduler(5,h)            Genetic algorithm. Configure with ga_* options."<<endl<<
"  HGS_scheduler(5,h)           Hierarchic Genetic Strategy. Configure with"<<endl<<
"                               ga_* and hgsX_* options."<<endl<<
"  SG_scheduler(5,h)            Stackelberg Game scheduler. Configure with"<<endl<<
"                               ga_* and sg_* options."<<endl<<
"  TS_scheduler(5,h)            TabuSearch. Configure with ts_* options."<<endl<<
""<<endl<<
"Options for simulator. Apply for all strategies."             <<endl<<
"  -n, --ttasks      <integer>    Total number of tasks (jobs)"<<endl<<
"  -b, --itasks      <integer>    Initial number of tasks"     <<endl<<
"  -i, --iatime      <distrib>    Task mean interarrival time" <<endl<<
"  -w, --workload    <distrib>    Task mean work load (M.I.)"  <<endl<<
"  -o, --ihosts      <integer>    Initial number of hosts"     <<endl<<
"  -m, --mips        <distrib>    Host mean (M.I.P.S.)"        <<endl<<
"  -a, --addhost     <distrib>    Add-host event distribution" <<endl<<
"  -d, --delhost     <distrib>    Del-host event distribution" <<endl<<
"  -f, --minhosts    <integer>    Minimum number of hosts"     <<endl<<
"  -g, --maxhosts    <integer>    Maximum number of hosts"     <<endl<<
"  -r, --reschedule               Reschedule unstart tasks"    <<endl<<
//"  -s, --strategy    <meta_p>     Scheduler meta policy"     <<endl<<
"  -x, --hostselect  <host_p>     Host selection policy"       <<endl<<
"  -y, --taskselect  <task_p>     Task selection policy"       <<endl<<
"  -z, --activate    <wake_p>     Scheduler wake-up policy"    <<endl<<
"  -l, --allocpolicy <local_p>    Host local scheduling policy"<<endl<<
"  -1, --nruns       <integer>    Number of runs"              <<endl<<
"  -2, --seed        <integer>    Random seed"                 <<endl<<
"  -t, --trace       <filename>   Enable trace on output file" <<endl<<
"  -h, --help                     Show this help"              <<endl<<
"      --print_debug_info <bool>  Print debug info (useful for debugging)" << endl <<
endl <<
"Options related to *ALL* genetic algorithm types (standard GA,HGS,Stackelberg):" << endl <<
"  parameter:" << endl <<
"  --ga_evolution_steps      <integer> (all) Number of evolution steps" <<endl<<
"  --ga_pop_size             <integer> (all) Population size" <<endl<<
"  --ga_intermediate_size    <integer> (all) Intermediate population size" <<endl<<
"  --ga_prob_cross           <double>  (all) Probability of crossover" <<endl<<
"  --ga_prob_mutate          <double>  (all) Probability of mutation" <<endl<<
"  --ga_start_choice         <START>   (all) Initial population operator" <<endl<<
"  --ga_select_choice        <SELECT>  (all) Selection operator" <<endl<<
"  --ga_select_extra         <double>  (all) Extra parameter for selection op." <<endl<<
"  --ga_cross_choice         <CROSS>   (all) Crossover operator" <<endl<<
"  --ga_cross_extra          <double>  (all) Extra parameter for crossover op." <<endl<<
"  --ga_mutate_choice        <MUTATE>  (all) Mutation operator" <<endl<<
"  --ga_mutate_extra         <double>  (all) Extra parameter for mutation op." <<endl<<

"  --ga_replace_if_better    <bool>    (all) ???" <<endl<<
"  --ga_replace_generational <bool>    (all) ???" <<endl<<

endl <<

"Options exclusive for StackelbergGame:"

"  --sg_mutation_size        <integer> ???" <<endl<<
"  --sg_env_length           <integer> ???" <<endl<<
"  --sg_env_choice           <ENV>     ???" <<endl<<

"  --sg_enable_game          <bool>    (SG) Enable game at the start of algorithm" <<endl<<
"  --sg_leader_factor        <double>  (SG) Factor of tasks assigned to leader" <<endl<<
"  --sg_number_followers     <integer> (SG) Number of followers" <<endl<<
"  --sg_min_fun_choice       <MINFUN>  (SG) Minimization function for game" <<endl<<
"  --sg_independent_GA       <bool>    (SG) Run GA independently for each player" <<endl<<
"  --sg_security             <bool>    (SG) Enable security in fitness" <<endl<<
"  --sg_security_extra       <double>  (SG) Extra multiplier for security" <<endl<<
"  --sg_calc_penalty         <bool>    (SG) Calculate makespan/flowtime penalties" <<endl<<
"  --sg_enable_qle           <bool>    (SG) Enable 'exec' in fitness" <<endl<<
"  --sg_enable_qlu           <bool>    (SG) Enable 'util' in fitness" <<endl<<
"  --sg_enable_qlab          <bool>    (SG) Enable 'abort' in fitness" <<endl<<
"  --sg_abort_extra          <double>  (SG) Extra multiplier for abort" <<endl<<


endl <<
"Options exclusive for HGS:" << endl <<
"  --hgs_mutation_size        <integer> HGS specific param (?)" <<endl<<
"  --hgs_env_length           <double>  ???" <<endl<<
"  --hgs_env_choice           <ENV>     ???" <<endl<<
"  --hgs_number_branches      <integer> Number branches (1 means no HGS)" <<endl<<
"  --hgs_number_metaepochs    <integer> Number of metaepochs for HGS alg." << endl <<
endl <<
"Additional options for HGS. Override ga_* for given level X of HGS branch." << endl <<
"X may be [2..4]. By default ga_* params set values for *ALL* levels" << endl <<
"of HGS. For example, if one sets ga_prob_mutate to 0.8, then *ALL* levels of" << endl <<
"will use 0.8 as mutation_probability, unless overridden with hgs2_prob_mutate, " << endl <<
"hgs3_prob_mutate etc. In short: set common parameters with ga_*, and then" << endl <<
"override for each HGS lvl with with hgsX_*. Options here:" << endl <<
"  --hgsX_evolution_steps      <integer> Override ga_evolution_steps for lvl X" << endl <<
"  --hgsX_pop_size             <integer> Override ga_pop_size for lvl X" << endl <<
"  --hgsX_intermediate_size    <integer> Override ga_intermediate_size for lvl X" << endl <<
"  --hgsX_prob_cross           <double>  Override ga_prob_cross for lvl X" << endl <<
"  --hgsX_prob_mutate          <double>  Override ga_prob_mutate for lvl X" << endl <<
"  --hgsX_start_choice         <START>   Override ga_start_choice for lvl X" << endl <<
"  --hgsX_select_choice        <SELECT>  Override ga_select_choice for lvl X" << endl <<
"  --hgsX_select_extra         <double>  Override ga_select_extra for lvl X" << endl <<
"  --hgsX_cross_choice         <CROSS>   Override ga_cross_choice for lvl X" << endl <<
"  --hgsX_cross_extra          <double>  Override ga_cross_extra for lvl X" << endl <<
"  --hgsX_mutate_choice        <MUTATE>  Override ga_mutate_choice for lvl X" << endl <<
"  --hgsX_mutate_extra         <double>  Override ga_mutate_extra for lvl X" << endl <<
"  --hgsX_mutation_size        <integer> Override ga_mutation_size for lvl X" << endl <<
"  --hgsX_replace_if_better    <bool>    Override ga_replace_if_better for lvl X" << endl <<
"  --hgsX_replace_generational <bool>    Override ga_replace_generational for X" << endl <<
"  --hgsX_env_length           <double>  Override ga_env_length for lvl X" << endl <<
"  --hgsX_env_choice           <integer> Override ga_env_choice for lvl X" << endl <<
endl <<
"Parameter section for TabuSearch scheduler."
"  --tb_nb_iterations       <integer> Number of iterations" << endl <<
"  --tb_start_choice        <HEURYS>  ???" << endl <<
"  --tb_use_delta_function  <bool>    ???" << endl <<
"  --tb_tabu_size           <integer> ???" << endl <<
"  --tb_max_tabu_status     <integer> ???" << endl <<
"  --tb_min_tabu_status     <integer> ???" << endl <<
"  --tb_max_repetitions     <integer> ???" << endl <<
"  --tb_nb_intensifications <integer> ???" << endl <<
"  --tb_nb_diversifications <integer> ???" << endl <<
"  --tb_elite_size          <integer> ???" << endl <<
"  --tb_aspiration_value    <integer> ???" << endl <<
"  --tb_max_nb_swaps        <float>   ???" << endl <<
"  --tb_max_nb_transfers    <float>   ???" << endl <<
"  --tb_max_load_interval   <float>   ???" << endl <<
"  --tb_percent_min_load    <float>   ???" << endl <<
endl <<
"Choices:" << endl <<
"  <HEURISTIC>  min_min       - ???" << endl  <<
"               max_min       - ???" << endl  <<
"               sufferage     - ???" << endl  <<
"               relative_cost - ???" << endl  <<
"               ljfr_sjfr     - ???" << endl  <<
"               mct           - ???" << endl  <<
"               met           - ???" << endl  <<
"               sa            - ???" << endl  <<
"               kpb           - ???" << endl  <<
"               olb           - ???" << endl <<
"               random        - ???"<< endl <<
"" << endl <<
"  <START>  0 - StartLJFRSJFR" << endl <<
"           1 - StartMCT" << endl <<
"           2 - StartRandom               (default)" << endl <<
"           3 - StartAll" << endl <<
"           4 - StartMinMin" << endl <<
"           5 - StartRelativeCost"  << endl <<
"           6 - StartKPB" << endl <<
"           7 - StartAll2" << endl <<
"" << endl <<
"  <SELECT> 0 - SelectRandom" << endl <<
"           1 - SelectBest" << endl <<
"           2 - SelectLinearRanking       (default)" << endl <<
"           3 - SelectExpRanking" << endl <<
"           4 - SelectBinaryTournament" << endl <<
"           5 - SelectNTournament"  << endl <<
"" << endl <<
"  <CROSS>  0 - CrossOnePoint" << endl <<
"           1 - CrossTwoPoints" << endl <<
"           2 - CrossUniform" << endl <<
"           3 - CrossFitnessBased" << endl <<
"           4 - CrossPMX" << endl <<
"           5 - CrossOX"  << endl <<
"           6 - CrossCX                   (default)" << endl <<
"" << endl <<
"  <MUTATE> 0 - MutateMove" << endl <<
"           1 - MutateSwap" << endl <<
"           2 - MutateBoth" << endl <<
"           3 - MutateRebalancing         (default)" << endl <<
"" << endl <<
"  <MINFUN> 0 - min_min (default)" << endl <<
"           1 - min_max (broken?)" << endl <<
"           2 - sufferage (broken?)" << endl<<
"" << endl <<
"  <HEURYS> 0 - MinMin_h" << endl <<
"           1 - MaxMin_h" << endl <<
"           2 - Sufferage_h" << endl <<
"           3 - RelativeCost_h" << endl <<
"           4 - LJFRSJFR_h" << endl <<
"           5 - MCT_h                     (default)" << endl <<
"           6 - MET_h" << endl <<
"           7 - SA_h" << endl <<
"           8 - KPB_h" << endl <<
"           9 - OLB_h" << endl <<
"          10 - Random_h" << endl <<
"" << endl <<
"  <ENV>    0 - env_basic                (default)" << endl <<
"           1 - env_minimalizing" << endl;

exit(0);
}

static struct option long_opts[] = {
    {"ttasks", 1, 0, 'n'},
    {"itasks", 1, 0, 'b'},
    {"iatime", 1, 0, 'i'},
    {"workload", 1, 0, 'w'},
    {"ihosts", 1, 0, 'o'},
    {"mips", 1, 0, 'm'},
    {"addhost", 1, 0, 'a'},
    {"delhost", 1, 0, 'd'},
    {"minhosts", 1, 0, 'f'},
    {"maxhosts", 1, 0, 'g'},
    {"reschedule", 0, 0, 'r'},
    {"strategy", 1, 0,'s'},
    {"hostselect", 1, 0, 'x'},
    {"taskselect", 1, 0, 'y'},
    {"activate", 1, 0, 'z'},
    {"allocpolicy", 1, 0, 'l'},
    {"nruns", 1, 0, '1'},
    {"seed", 1, 0, '2'},
    {"help", 0, 0, 'h'},
    {"trace",1, 0, 't'},

    {"ga_evolution_steps",1,0,1000},
    {"ga_pop_size",1,0,1001},
    {"ga_intermediate_size",1,0,1002},

    {"ga_prob_cross",1,0,1003},
    {"ga_prob_mutate",1,0,1004},

    {"ga_start_choice",1,0,1005},
    {"ga_select_choice",1,0,1006},
    {"ga_select_extra",1,0,1007},

    {"ga_cross_choice",1,0,1011},
    {"ga_cross_extra",1,0,1012},

    {"ga_mutate_choice",1,0,1013},
    {"ga_mutate_extra",1,0,1014},


    {"ga_replace_if_better",1,0,1016},
    {"ga_replace_generational",1,0,1017},
    {"ga_struggle_replace",1,0,1018},
    {"ga_struggle_extra_parameter",1,0,1019},


    {"sg_enable_game",1,0,1020},
	{"sg_leader_factor",1,0,1021},
	{"sg_number_followers",1,0,1022},
	{"sg_min_fun_choice",1,0,1023},
	{"sg_independent_GA",1,0,1024},
	{"sg_security",1,0,1025},
	{"sg_calc_penalty", 1, 0, 1026},
	{"sg_security_extra",1,0,1027},
	{"sg_enable_qle",1,0,1028},
	{"sg_enable_qlu",1,0,1029},
	{"sg_enable_qlab",1,0,1030},
	{"sg_abort_extra",1,0,1031},

    {"hgs_number_branches",1,0,1032},
    {"hgs_number_metaepochs",1,0,1033},

    {"tb_nb_iterations",1,0,1034},
    {"tb_start_choice",1,0,1035},
    {"tb_use_delta_function",1,0,1036},

    {"tb_tabu_size",1,0,1037},
    {"tb_max_tabu_status",1,0,1038},
    {"tb_min_tabu_status",1,0,1039},

    {"tb_max_repetitions",1,0,1040},
    {"tb_nb_intensifications",1,0,1041},
    {"tb_nb_diversifications",1,0,1042},
    {"tb_elite_size",1,0,1043},
    {"tb_aspiration_value",1,0,1044},

    {"tb_max_nb_swaps",1,0,1045},
    {"tb_max_nb_transfers",1,0,1046},
    {"tb_max_load_interval",1,0,1047},
    {"tb_percent_min_load",1,0,1048},



    {"hgs_mutation_size",1,0,1049},
    {"sg_mutation_size",1,0,1050},

    {"hgs_env_length",1,0,1051},
    {"hgs_env_choice",1,0,1052},

    {"sg_env_length",1,0,1053},
    {"sg_env_choice",1,0,1054},


    {"print_debug_info",0,0,9999},


    /// HGS
    /*
    --hgsX_evolution_steps
	--hgsX_pop_size
	--hgsX_intermediate_size
	--hgsX_prob_cross
	--hgsX_prob_mutate
	--hgsX_start_choice
	--hgsX_select_choice
	--hgsX_select_extra
	--hgsX_cross_choice
	--hgsX_cross_extra
	--hgsX_mutate_choice
	--hgsX_mutate_extra
	--hgsX_mutation_size
	--hgsX_replace_if_better
	--hgsX_replace_generational
	--hgsX_env_length
	--hgsX_env_choice

	*/


    {"hgs2_evolution_steps",1,0,2000},
    {"hgs2_pop_size",1,0,2001},
    {"hgs2_intermediate_size",1,0,2002},
    {"hgs2_prob_cross",1,0,2003},
    {"hgs2_prob_mutate",1,0,2004},
    {"hgs2_start_choice",1,0,2005},
    {"hgs2_select_choice",1,0,2006},
    {"hgs2_select_extra",1,0,2007},
    {"hgs2_cross_choice",1,0,2011},
    {"hgs2_cross_extra",1,0,2012},
    {"hgs2_mutate_choice",1,0,2013},
    {"hgs2_mutate_extra",1,0,2014},
    {"hgs2_mutation_size",1,0,2015},
    {"hgs2_replace_if_better",1,0,2016},
    {"hgs2_replace_generational",1,0,2017},
    {"hgs2_env_length",1,0,2018},
    {"hgs2_env_choice",1,0,2019},

    {"hgs3_evolution_steps",1,0,3000},
    {"hgs3_pop_size",1,0,3001},
    {"hgs3_intermediate_size",1,0,3002},
    {"hgs3_prob_cross",1,0,3003},
    {"hgs3_prob_mutate",1,0,3004},
    {"hgs3_start_choice",1,0,3005},
    {"hgs3_select_choice",1,0,3006},
    {"hgs3_select_extra",1,0,3007},
    {"hgs3_cross_choice",1,0,3011},
    {"hgs3_cross_extra",1,0,3012},
    {"hgs3_mutate_choice",1,0,3013},
    {"hgs3_mutate_extra",1,0,3014},
    {"hgs3_mutation_size",1,0,3015},
    {"hgs3_replace_if_better",1,0,3016},
    {"hgs3_replace_generational",1,0,3017},
    {"hgs3_env_length",1,0,3018},
    {"hgs3_env_choice",1,0,3019},

    {"hgs4_evolution_steps",1,0,4000},
    {"hgs4_pop_size",1,0,4001},
    {"hgs4_intermediate_size",1,0,4002},
    {"hgs4_prob_cross",1,0,4003},
    {"hgs4_prob_mutate",1,0,4004},
    {"hgs4_start_choice",1,0,4005},
    {"hgs4_select_choice",1,0,4006},
    {"hgs4_select_extra",1,0,4007},
    {"hgs4_cross_choice",1,0,4011},
    {"hgs4_cross_extra",1,0,4012},
    {"hgs4_mutate_choice",1,0,4013},
    {"hgs4_mutate_extra",1,0,4014},
    {"hgs4_mutation_size",1,0,4015},
    {"hgs4_replace_if_better",1,0,4016},
    {"hgs4_replace_generational",1,0,4017},
    {"hgs4_env_length",1,0,4018},
    {"hgs4_env_choice",1,0,4019},

    {"hgs5_evolution_steps",1,0,5000},
    {"hgs5_pop_size",1,0,5001},
    {"hgs5_intermediate_size",1,0,5002},
    {"hgs5_prob_cross",1,0,5003},
    {"hgs5_prob_mutate",1,0,5004},
    {"hgs5_start_choice",1,0,5005},
    {"hgs5_select_choice",1,0,5006},
    {"hgs5_select_extra",1,0,5007},
    {"hgs5_cross_choice",1,0,5011},
    {"hgs5_cross_extra",1,0,5012},
    {"hgs5_mutate_choice",1,0,5013},
    {"hgs5_mutate_extra",1,0,5014},
    {"hgs5_mutation_size",1,0,5015},
    {"hgs5_replace_if_better",1,0,5016},
    {"hgs5_replace_generational",1,0,5017},
    {"hgs5_env_length",1,0,5018},
    {"hgs5_env_choice",1,0,5019},


    {"cma_nb_generations",1,0,6000},
    {"cma_population_height",1,0,6001},
    {"cma_population_width",1,0,6002},
    {"cma_nb_solutions_to_recombine",1,0,6003},
    {"cma_nb_recombinations",1,0,6004},
    {"cma_nb_mutations",1,0,6005},
    {"cma_start_choice",1,0,6006},
    {"cma_neighborhood_pattern",1,0,6007},
    {"cma_recombination_order",1,0,6011},
    {"cma_mutation_order",1,0,6012},
    {"cma_recombine_choice",1,0,6013},
    {"cma_recombine_selection",1,0,6014},
    {"cma_rec_selection_extra",1,0,6015},
    {"cma_mutate_choice",1,0,6016},
    {"cma_mutate_extra_parameter",1,0,6017},
    {"cma_nb_ls_not_improving_iterations",1,0,6018},
    {"cma_add_only_if_better",1,0,6019},


    {"ma_nb_generations",1,0,7000},
    {"ma_population_size",1,0,7001},
    {"ma_nb_solutions_to_recombine",1,0,7002},
    {"ma_nb_recombinations",1,0,7003},
    {"ma_nb_mutations",1,0,7004},
    {"ma_start_choice",1,0,7005},
    {"ma_select_choice",1,0,7006},
    {"ma_select_extra_parameter",1,0,7007},
    {"ma_cross_choice",1,0,7011},
    {"ma_recombine_selection",1,0,7012},
    {"ma_rec_selection_extra",1,0,7013},
    {"ma_mutate_choice",1,0,7014},
    {"ma_mutate_extra_parameter",1,0,7015},
    {"ma_mutate_selection",1,0,7016},
    {"ma_mut_selection_extra",1,0,7017},
    {"ma_nb_ls_not_improving_iterations",1,0,7018},
    {"ma_add_only_if_better",1,0,7019},

    {"security",1,0,8000},
    {"security_param",1,0,8001},
    {"energy",1,0,8002},
    {0, 0, 0, 0}
};

int parseArg (int argc, char* argv[], int& totalTasks, int& initialTasks,
              string& iaTime, string& workLoad, int& initialHosts,
              string& mips, string& addHost, string& delHost,int& minHosts,
              int& maxHosts, bool& reschedule, string& strategy,
              string& activation, string& hostSelection,
              string& taskSelection, string& allocPolicy, int& nRuns,
              long int& seed, bool& trace, string& traceFile,
              GAParams& ga_par, GAParams (&hgs_par)[4], TabuParams& tabuParams,
              CMAParams cmaParams, MAParams maParams) {

    int optidx;
    int c = 2;

    totalTasks    = DEFAULT_TOTAL_TASKS;
    initialTasks  = DEFAULT_INITIAL_TASKS;
    iaTime        = DEFAULT_IATIME;
    workLoad      = DEFAULT_WORKLOAD;
    initialHosts  = DEFAULT_INITIAL_HOSTS;
    mips          = DEFAULT_MIPS;
    addHost       = DEFAULT_ADD_HOST;
    delHost       = DEFAULT_DEL_HOST;
    minHosts      = DEFAULT_MIN_HOSTS;
    maxHosts      = DEFAULT_MAX_HOSTS;
    reschedule    = DEFAULT_RESCHEDULE;
    strategy      = DEFAULT_STRATEGY;
    activation    = DEFAULT_ACTIVATION;
    hostSelection = DEFAULT_HOST_SELECTION;
    taskSelection = DEFAULT_TASK_SELECTION;
    allocPolicy   = DEFAULT_HOST_POLICY;
    nRuns         = DEFAULT_NRUNS;
    seed          = DEFAULT_SEED;
    trace         = DEFAULT_TRACE;

    /*
     * Make sure we don't use forbidden optoins!
     */
    enum ele{ GA, SG, HGS, TS, CMA, MA };
    set<int> ss;

    while (c != -1) {
        c= getopt_long(argc, argv,"n:b:i:w:o:m:a:d:f:g:rs:x:y:z:l:1:2:ht:",
                       long_opts, &optidx);
        switch (c) {
            case 'n':  totalTasks = parseInteger(optarg);           break;
            case 'b':  initialTasks = parseInteger(optarg);         break;
            case 'i':  iaTime = optarg;                             break;
            case 'w':  workLoad = optarg;                           break;
            case 'o':  initialHosts = parseInteger(optarg);         break;
            case 'm':  mips = optarg;                               break;
            case 'a':  addHost = optarg;                            break;
            case 'd':  delHost = optarg;                            break;
            case 'f':  minHosts = parseInteger(optarg);             break;
            case 'g':  maxHosts = parseInteger(optarg);             break;
            case 'r':  reschedule = true;                           break;
            case 's':  strategy = optarg;                           break;
            case 'x':  hostSelection = optarg;                      break;
            case 'y':  taskSelection = optarg;                      break;
            case 'z':  activation = optarg;                         break;
            case 'l':  allocPolicy = optarg;                        break;
            case '1':  nRuns = parseInteger(optarg);                break;
            case '2':  seed = parseInteger(optarg);                 break;
            case 'h':  usage();                                     break;
            case 't':  trace = true; traceFile = optarg;            break;
            case 1000:  ss.insert(GA);  ga_par.evolution_steps = parseInteger(optarg); break;
            case 1001:  ss.insert(GA);  ga_par.pop_size = parseInteger(optarg);    break;
            case 1002:  ss.insert(GA);  ga_par.intermediate_size = parseInteger(optarg);    break;
            case 1003:  ss.insert(GA);  ga_par.prob_cross = parseDouble(optarg);    break;
            case 1004:  ss.insert(GA);  ga_par.prob_mutate = parseDouble(optarg);    break;
            case 1005:  ss.insert(GA);  ga_par.start_choice = parseInteger(optarg);    break;
            case 1006:  ss.insert(GA);  ga_par.select_choice = parseInteger(optarg);    break;
            case 1007:  ss.insert(GA);  ga_par.select_extra = parseDouble(optarg);    break;
            case 1011:  ss.insert(GA);  ga_par.cross_choice = parseInteger(optarg);    break;
            case 1012:  ss.insert(GA);  ga_par.cross_extra = parseDouble(optarg);    break;
            case 1013:  ss.insert(GA);  ga_par.mutate_choice = parseInteger(optarg);    break;
            case 1014:  ss.insert(GA);  ga_par.mutate_extra = parseDouble(optarg);    break;

            case 1016:  ss.insert(GA);  ga_par.replace_if_better = parseInteger(optarg);    break;
            case 1017:  ss.insert(GA);  ga_par.replace_generational = parseInteger(optarg);    break;
            case 1018:  ss.insert(GA);  ga_par.struggle_replace = parseInteger(optarg);    break;
            case 1019:  ss.insert(GA);  ga_par.struggle_extra_parameter = parseInteger(optarg);    break;

            ////

            case 1020:  ss.insert(SG); ga_par.enable_game = parseInteger(optarg);    break;
            case 1021:  ss.insert(SG); ga_par.leader_factor = parseDouble(optarg);    break;
            case 1022:  ss.insert(SG); ga_par.number_followers = parseInteger(optarg);    break;
            case 1023:  ss.insert(SG); ga_par.min_fun_choice = parseInteger(optarg);    break;
            case 1024:  ss.insert(SG); ga_par.independent_GA = parseInteger(optarg);    break;
            case 1025:  ss.insert(SG); ga_par.security = parseInteger(optarg);     break;
            case 1026:  ss.insert(SG); ga_par.calc_penalty = parseInteger(optarg);     break;
            case 1027:  ss.insert(SG); ga_par.security_extra = parseDouble(optarg);     break;

            case 1028:  ss.insert(SG); ga_par.enable_qle = parseInteger(optarg); break;
            case 1029:  ss.insert(SG); ga_par.enable_qlu = parseInteger(optarg); break;
            case 1030:  ss.insert(SG); ga_par.enable_qlab = parseInteger(optarg); break;

            case 1031:  ss.insert(SG); ga_par.abort_extra = parseDouble(optarg); break;

            case 1032:  ss.insert(HGS); ga_par.number_branches = parseInteger(optarg); break;
            case 1033:  ss.insert(HGS); ga_par.number_metaepochs = parseInteger(optarg); break;




            case 1034:  ss.insert(TS); tabuParams.nb_iterations = parseInteger(optarg); break;
			case 1035:  ss.insert(TS); tabuParams.start_choice = parseInteger(optarg); break;
			case 1036:  ss.insert(TS); tabuParams.use_delta_function = parseInteger(optarg); break;

			case 1037:  ss.insert(TS); tabuParams.tabu_size = parseInteger(optarg); break;
			case 1038:  ss.insert(TS); tabuParams.max_tabu_status = parseInteger(optarg); break;
			case 1039:  ss.insert(TS); tabuParams.min_tabu_status = parseInteger(optarg); break;

			case 1040:  ss.insert(TS); tabuParams.max_repetitions = parseInteger(optarg); break;
			case 1041:  ss.insert(TS); tabuParams.nb_intensifications = parseInteger(optarg); break;
			case 1042:  ss.insert(TS); tabuParams.nb_diversifications = parseInteger(optarg); break;
			case 1043:  ss.insert(TS); tabuParams.elite_size = parseInteger(optarg); break;
			case 1044:  ss.insert(TS); tabuParams.aspiration_value = parseInteger(optarg); break;

			case 1045:  ss.insert(TS); tabuParams.max_nb_swaps = parseDouble(optarg); break;
			case 1046:  ss.insert(TS); tabuParams.max_nb_transfers = parseDouble(optarg); break;
			case 1047:  ss.insert(TS); tabuParams.max_load_interval = parseDouble(optarg); break;
			case 1048:  ss.insert(TS); tabuParams.percent_min_load = parseDouble(optarg); break;


			case 1049:  ss.insert(HGS); ga_par.mutation_size = parseInteger(optarg);    break;
			case 1050:  ss.insert(SG); ga_par.mutation_size = parseInteger(optarg);    break;

			case 1051:  ss.insert(HGS); ga_par.env_length = parseDouble(optarg);    break;
			case 1052:  ss.insert(HGS); ga_par.env_choice = parseInteger(optarg);    break;

			case 1053:  ss.insert(SG); ga_par.env_length = parseDouble(optarg);    break;
			case 1054:  ss.insert(SG); ga_par.env_choice = parseInteger(optarg);    break;


            case 9999:  g_print_debug_info = true;                  break;


            /*
             * HGS branches (branch marked with hgs2_xxx here)
             */

            case 2000:  ss.insert(HGS); hgs_par[0].is_set_evolution_steps = true;
						ss.insert(HGS); hgs_par[0].evolution_steps = parseInteger(optarg);
                        break;
			case 2001:  ss.insert(HGS); hgs_par[0].is_set_pop_size = true;
			            ss.insert(HGS); hgs_par[0].pop_size = parseInteger(optarg);
			            break;
			case 2002:  ss.insert(HGS); hgs_par[0].is_set_intermediate_size = true;
			            ss.insert(HGS); hgs_par[0].intermediate_size = parseInteger(optarg);
			            break;
			case 2003:  ss.insert(HGS); hgs_par[0].is_set_prob_cross = true;
			            ss.insert(HGS); hgs_par[0].prob_cross = parseDouble(optarg);
			            break;
			case 2004:  ss.insert(HGS); hgs_par[0].is_set_prob_mutate = true;
			            ss.insert(HGS); hgs_par[0].prob_mutate = parseDouble(optarg);
			            break;
			case 2005:  ss.insert(HGS); hgs_par[0].is_set_start_choice = true;
			            ss.insert(HGS); hgs_par[0].start_choice = parseInteger(optarg);
			            break;
			case 2006:  ss.insert(HGS); hgs_par[0].is_set_select_choice = true;
			            ss.insert(HGS); hgs_par[0].select_choice = parseInteger(optarg);
			            break;
			case 2007:  ss.insert(HGS); hgs_par[0].is_set_select_extra = true;
			            ss.insert(HGS); hgs_par[0].select_extra = parseDouble(optarg);
			            break;
			case 2011:  ss.insert(HGS); hgs_par[0].is_set_cross_choice = true;
			            ss.insert(HGS); hgs_par[0].cross_choice = parseInteger(optarg);
			            break;
			case 2012:  ss.insert(HGS); hgs_par[0].is_set_cross_extra = true;
			            ss.insert(HGS); hgs_par[0].cross_extra = parseDouble(optarg);
			            break;
			case 2013:  ss.insert(HGS); hgs_par[0].is_set_mutate_choice = true;
			            ss.insert(HGS); hgs_par[0].mutate_choice = parseInteger(optarg);
			            break;
			case 2014:  ss.insert(HGS); hgs_par[0].is_set_mutate_extra = true;
			            ss.insert(HGS); hgs_par[0].mutate_extra = parseDouble(optarg);
			            break;
			case 2015:  ss.insert(HGS); hgs_par[0].is_set_mutation_size = true;
			            ss.insert(HGS); hgs_par[0].mutation_size = parseInteger(optarg);
			            break;
			case 2016:  ss.insert(HGS); hgs_par[0].is_set_replace_if_better = true;
			            ss.insert(HGS); hgs_par[0].replace_if_better = parseInteger(optarg);
			            break;
			case 2017:  ss.insert(HGS); hgs_par[0].is_set_replace_generational = true;
			            ss.insert(HGS); hgs_par[0].replace_generational = parseInteger(optarg);
			            break;
			case 2018:  ss.insert(HGS); hgs_par[0].is_set_env_length = true;
			            ss.insert(HGS); hgs_par[0].env_length = parseDouble(optarg);
			            break;
			case 2019:  ss.insert(HGS); hgs_par[0].is_set_env_choice = true;
			            ss.insert(HGS); hgs_par[0].env_choice = parseInteger(optarg);
			            break;


			/*
			 * HGS branches (branch marked with hgs3_xxx here)
			 */

			case 3000:  ss.insert(HGS); hgs_par[1].is_set_evolution_steps = true;
						ss.insert(HGS); hgs_par[1].evolution_steps = parseInteger(optarg);
						break;
			case 3001:  ss.insert(HGS); hgs_par[1].is_set_pop_size = true;
						ss.insert(HGS); hgs_par[1].pop_size = parseInteger(optarg);
						break;
			case 3002:  ss.insert(HGS); hgs_par[1].is_set_intermediate_size = true;
						ss.insert(HGS); hgs_par[1].intermediate_size = parseInteger(optarg);
						break;
			case 3003:  ss.insert(HGS); hgs_par[1].is_set_prob_cross = true;
						ss.insert(HGS); hgs_par[1].prob_cross = parseDouble(optarg);
						break;
			case 3004:  ss.insert(HGS); hgs_par[1].is_set_prob_mutate = true;
						ss.insert(HGS); hgs_par[1].prob_mutate = parseDouble(optarg);
						break;
			case 3005:  ss.insert(HGS); hgs_par[1].is_set_start_choice = true;
						ss.insert(HGS); hgs_par[1].start_choice = parseInteger(optarg);
						break;
			case 3006:  ss.insert(HGS); hgs_par[1].is_set_select_choice = true;
						ss.insert(HGS); hgs_par[1].select_choice = parseInteger(optarg);
						break;
			case 3007:  ss.insert(HGS); hgs_par[1].is_set_select_extra = true;
						ss.insert(HGS); hgs_par[1].select_extra = parseDouble(optarg);
						break;
			case 3011:  ss.insert(HGS); hgs_par[1].is_set_cross_choice = true;
						ss.insert(HGS); hgs_par[1].cross_choice = parseInteger(optarg);
						break;
			case 3012:  ss.insert(HGS); hgs_par[1].is_set_cross_extra = true;
						ss.insert(HGS); hgs_par[1].cross_extra = parseDouble(optarg);
						break;
			case 3013:  ss.insert(HGS); hgs_par[1].is_set_mutate_choice = true;
						ss.insert(HGS); hgs_par[1].mutate_choice = parseInteger(optarg);
						break;
			case 3014:  ss.insert(HGS); hgs_par[1].is_set_mutate_extra = true;
						ss.insert(HGS); hgs_par[1].mutate_extra = parseDouble(optarg);
						break;
			case 3015:  ss.insert(HGS); hgs_par[1].is_set_mutation_size = true;
						ss.insert(HGS); hgs_par[1].mutation_size = parseInteger(optarg);
						break;
			case 3016:  ss.insert(HGS); hgs_par[1].is_set_replace_if_better = true;
						ss.insert(HGS); hgs_par[1].replace_if_better = parseInteger(optarg);
						break;
			case 3017:  ss.insert(HGS); hgs_par[1].is_set_replace_generational = true;
						ss.insert(HGS); hgs_par[1].replace_generational = parseInteger(optarg);
						break;
			case 3018:  ss.insert(HGS); hgs_par[1].is_set_env_length = true;
						ss.insert(HGS); hgs_par[1].env_length = parseDouble(optarg);
						break;
			case 3019:  ss.insert(HGS); hgs_par[1].is_set_env_choice = true;
						ss.insert(HGS); hgs_par[1].env_choice = parseInteger(optarg);
						break;

			/*
			 * HGS branches (branch marked with hgs4_xxx here)
			 */

			case 4000:  ss.insert(HGS); hgs_par[2].is_set_evolution_steps = true;
						ss.insert(HGS); hgs_par[2].evolution_steps = parseInteger(optarg);
						break;
			case 4001:  ss.insert(HGS); hgs_par[2].is_set_pop_size = true;
						ss.insert(HGS); hgs_par[2].pop_size = parseInteger(optarg);
						break;
			case 4002:  ss.insert(HGS); hgs_par[2].is_set_intermediate_size = true;
						ss.insert(HGS); hgs_par[2].intermediate_size = parseInteger(optarg);
						break;
			case 4003:  ss.insert(HGS); hgs_par[2].is_set_prob_cross = true;
						ss.insert(HGS); hgs_par[2].prob_cross = parseDouble(optarg);
						break;
			case 4004:  ss.insert(HGS); hgs_par[2].is_set_prob_mutate = true;
						ss.insert(HGS); hgs_par[2].prob_mutate = parseDouble(optarg);
						break;
			case 4005:  ss.insert(HGS); hgs_par[2].is_set_start_choice = true;
						ss.insert(HGS); hgs_par[2].start_choice = parseInteger(optarg);
						break;
			case 4006:  ss.insert(HGS); hgs_par[2].is_set_select_choice = true;
						ss.insert(HGS); hgs_par[2].select_choice = parseInteger(optarg);
						break;
			case 4007:  ss.insert(HGS); hgs_par[2].is_set_select_extra = true;
						ss.insert(HGS); hgs_par[2].select_extra = parseDouble(optarg);
						break;
			case 4011:  ss.insert(HGS); hgs_par[2].is_set_cross_choice = true;
						ss.insert(HGS); hgs_par[2].cross_choice = parseInteger(optarg);
						break;
			case 4012:  ss.insert(HGS); hgs_par[2].is_set_cross_extra = true;
						ss.insert(HGS); hgs_par[2].cross_extra = parseDouble(optarg);
						break;
			case 4013:  ss.insert(HGS); hgs_par[2].is_set_mutate_choice = true;
						ss.insert(HGS); hgs_par[2].mutate_choice = parseInteger(optarg);
						break;
			case 4014:  ss.insert(HGS); hgs_par[2].is_set_mutate_extra = true;
						ss.insert(HGS); hgs_par[2].mutate_extra = parseDouble(optarg);
						break;
			case 4015:  ss.insert(HGS); hgs_par[2].is_set_mutation_size = true;
						ss.insert(HGS); hgs_par[2].mutation_size = parseInteger(optarg);
						break;
			case 4016:  ss.insert(HGS); hgs_par[2].is_set_replace_if_better = true;
						ss.insert(HGS); hgs_par[2].replace_if_better = parseInteger(optarg);
						break;
			case 4017:  ss.insert(HGS); hgs_par[2].is_set_replace_generational = true;
						ss.insert(HGS); hgs_par[2].replace_generational = parseInteger(optarg);
						break;
			case 4018:  ss.insert(HGS); hgs_par[2].is_set_env_length = true;
						ss.insert(HGS); hgs_par[2].env_length = parseDouble(optarg);
						break;
			case 4019:  ss.insert(HGS); hgs_par[2].is_set_env_choice = true;
						ss.insert(HGS); hgs_par[2].env_choice = parseInteger(optarg);
						break;




			/*
			 * HGS branches (branch marked with hgs2_xxx here)
			 */

			case 5000:  ss.insert(HGS); hgs_par[3].is_set_evolution_steps = true;
						ss.insert(HGS); hgs_par[3].evolution_steps = parseInteger(optarg);
						break;
			case 5001:  ss.insert(HGS); hgs_par[3].is_set_pop_size = true;
						ss.insert(HGS); hgs_par[3].pop_size = parseInteger(optarg);
						break;
			case 5002:  ss.insert(HGS); hgs_par[3].is_set_intermediate_size = true;
						ss.insert(HGS); hgs_par[3].intermediate_size = parseInteger(optarg);
						break;
			case 5003:  ss.insert(HGS); hgs_par[3].is_set_prob_cross = true;
						ss.insert(HGS); hgs_par[3].prob_cross = parseDouble(optarg);
						break;
			case 5004:  ss.insert(HGS); hgs_par[3].is_set_prob_mutate = true;
						ss.insert(HGS); hgs_par[3].prob_mutate = parseDouble(optarg);
						break;
			case 5005:  ss.insert(HGS); hgs_par[3].is_set_start_choice = true;
						ss.insert(HGS); hgs_par[3].start_choice = parseInteger(optarg);
						break;
			case 5006:  ss.insert(HGS); hgs_par[3].is_set_select_choice = true;
						ss.insert(HGS); hgs_par[3].select_choice = parseInteger(optarg);
						break;
			case 5007:  ss.insert(HGS); hgs_par[3].is_set_select_extra = true;
						ss.insert(HGS); hgs_par[3].select_extra = parseDouble(optarg);
						break;
			case 5011:  ss.insert(HGS); hgs_par[3].is_set_cross_choice = true;
						ss.insert(HGS); hgs_par[3].cross_choice = parseInteger(optarg);
						break;
			case 5012:  ss.insert(HGS); hgs_par[3].is_set_cross_extra = true;
						ss.insert(HGS); hgs_par[3].cross_extra = parseDouble(optarg);
						break;
			case 5013:  ss.insert(HGS); hgs_par[3].is_set_mutate_choice = true;
						ss.insert(HGS); hgs_par[3].mutate_choice = parseInteger(optarg);
						break;
			case 5014:  ss.insert(HGS); hgs_par[3].is_set_mutate_extra = true;
						ss.insert(HGS); hgs_par[3].mutate_extra = parseDouble(optarg);
						break;
			case 5015:  ss.insert(HGS); hgs_par[3].is_set_mutation_size = true;
						ss.insert(HGS); hgs_par[3].mutation_size = parseInteger(optarg);
						break;
			case 5016:  ss.insert(HGS); hgs_par[3].is_set_replace_if_better = true;
						ss.insert(HGS); hgs_par[3].replace_if_better = parseInteger(optarg);
						break;
			case 5017:  ss.insert(HGS); hgs_par[3].is_set_replace_generational = true;
						ss.insert(HGS); hgs_par[3].replace_generational = parseInteger(optarg);
						break;
			case 5018:  ss.insert(HGS); hgs_par[3].is_set_env_length = true;
						ss.insert(HGS); hgs_par[3].env_length = parseDouble(optarg);
						break;
			case 5019:  ss.insert(HGS); hgs_par[3].is_set_env_choice = true;
						ss.insert(HGS); hgs_par[3].env_choice = parseInteger(optarg);
						break;


                        case 6000:  ss.insert(CMA); cmaParams._nb_generations = parseInteger(optarg);
						break;
			case 6001:  ss.insert(CMA); cmaParams._population_height = parseInteger(optarg);
						break;
			case 6002:  ss.insert(CMA); cmaParams._population_width = parseInteger(optarg);
						break;
			case 6003:  ss.insert(CMA); cmaParams._nb_solutions_to_recombine = parseInteger(optarg);
						break;
			case 6004:  ss.insert(CMA); cmaParams._nb_recombinations = parseInteger(optarg);
						break;
			case 6005:  ss.insert(CMA); cmaParams._nb_mutations = parseInteger(optarg);
						break;
			case 6006:  ss.insert(CMA); cmaParams._start_choice = parseInteger(optarg);
						break;
			case 6007:  ss.insert(CMA); cmaParams._neighborhood_pattern = parseInteger(optarg);
						break;
			case 6011:  ss.insert(CMA); cmaParams._recombination_order = parseInteger(optarg);
						break;
			case 6012:  ss.insert(CMA); cmaParams._mutation_order = parseInteger(optarg);
						break;
			case 6013:  ss.insert(CMA); cmaParams._recombine_choice = parseInteger(optarg);
						break;
			case 6014:  ss.insert(CMA); cmaParams._recombine_selection = parseInteger(optarg);
						break;
			case 6015:  ss.insert(CMA); cmaParams._rec_selection_extra = parseDouble(optarg);
						break;
			case 6016:  ss.insert(CMA); cmaParams._mutate_choice = parseInteger(optarg);
						break;
			case 6017:  ss.insert(CMA); cmaParams._mutate_extra_parameter = parseDouble(optarg);
						break;
			case 6018:  ss.insert(CMA); cmaParams._nb_ls_not_improving_iterations = parseDouble(optarg);
						break;
			case 6019:  ss.insert(CMA); cmaParams._add_only_if_better = parseInteger(optarg);
						break;


                        case 7000:  ss.insert(MA); maParams._nb_generations = parseInteger(optarg);
						break;
			case 7001:  ss.insert(MA); maParams._population_size = parseInteger(optarg);
						break;
			case 7002:  ss.insert(MA); maParams._nb_solutions_to_recombine = parseInteger(optarg);
						break;
			case 7003:  ss.insert(MA); maParams._nb_recombinations = parseInteger(optarg);
						break;
			case 7004:  ss.insert(MA); maParams._nb_mutations = parseInteger(optarg);
						break;
			case 7005:  ss.insert(MA); maParams._start_choice = parseInteger(optarg);
						break;
			case 7006:  ss.insert(MA); maParams._select_choice = parseInteger(optarg);
						break;
			case 7007:  ss.insert(MA); maParams._select_extra_parameter = parseInteger(optarg);
						break;
			case 7011:  ss.insert(MA); maParams._cross_choice = parseInteger(optarg);
						break;
			case 7012:  ss.insert(MA); maParams._recombine_selection = parseDouble(optarg);
						break;
			case 7013:  ss.insert(MA); maParams._rec_selection_extra = parseDouble(optarg);
						break;
			case 7014:  ss.insert(MA); maParams._mutate_choice = parseInteger(optarg);
						break;
			case 7015:  ss.insert(MA); maParams._mutate_extra_parameter = parseDouble(optarg);
						break;
			case 7016:  ss.insert(MA); maParams._mutate_selection = parseInteger(optarg);
						break;
			case 7017:  ss.insert(MA); maParams._mut_selection_extra = parseDouble(optarg);
						break;
			case 7018:  ss.insert(MA); maParams._nb_ls_not_improving_iterations = parseDouble(optarg);
						break;
			case 7019:  ss.insert(MA); maParams._add_only_if_better = parseInteger(optarg);
						break;

			case 8000:  ga_par.security = parseInteger(optarg);
						tabuParams.security = parseInteger(optarg);
						hgs_par[0].security = parseInteger(optarg);
						hgs_par[1].security = parseInteger(optarg);
						hgs_par[2].security = parseInteger(optarg);
						hgs_par[3].security = parseInteger(optarg);
						cmaParams.security = parseInteger(optarg);
						maParams.security = parseInteger(optarg);
						break;

			case 8001:  ga_par.security_param = parseInteger(optarg);
						tabuParams.security_param = parseInteger(optarg);
						hgs_par[0].security_param = parseInteger(optarg);
						hgs_par[1].security_param = parseInteger(optarg);
						hgs_par[2].security_param = parseInteger(optarg);
						hgs_par[3].security_param = parseInteger(optarg);
						cmaParams.security_param = parseInteger(optarg);
						maParams.security_param = parseInteger(optarg);
						break;

			case 8002:  ga_par.energy = parseInteger(optarg);
						tabuParams.energy = parseInteger(optarg);
						hgs_par[0].energy = parseInteger(optarg);
						hgs_par[1].energy = parseInteger(optarg);
						hgs_par[2].energy = parseInteger(optarg);
						hgs_par[3].energy = parseInteger(optarg);
						cmaParams.energy = parseInteger(optarg);
						maParams.energy = parseInteger(optarg);
						break;



            default:   usage();                                     break;
            case -1 :                                               break;
        };
    }

    argc -= optind;
    argv += optind;



    // enum ele{ GA, SG, HGS, TS };

    int lb = strategy.find("(");
    string sName;

    if (lb > 0 ) {
        sName = strategy.substr(0,lb);
    }else{
    	sName = strategy;
    }

    if (!sName.compare("simple_metapolicy")) {
    	if ( ss.size() != 0 ){
    		cout << "Unsuported params passed to 'simple_metapolicy'" << endl;
    		exit(1);
    	}
    } else if (!sName.compare("AdHoc_scheduler")) {
    	if ( ss.size() != 0 ){
			cout << "Unsuported params passed to 'AdHoc_scheduler'" << endl;
			exit(1);
		}
    } else if (!sName.compare("GA_scheduler")) {
    	if ( ss.find(SG) != ss.end() ||
			 ss.find(HGS) != ss.end() ||
			 ss.find(TS) != ss.end() ||
                         ss.find(CMA) != ss.end() ||
                        ss.find(MA) != ss.end() ){
    		cout << "Unsuported params passed to 'GA_scheduler'" << endl;
    		exit(1);
    	}
    } else if (!sName.compare("TS_scheduler")) {
    	if ( ss.find(SG) != ss.end() ||
		     ss.find(HGS) != ss.end() ||
		     ss.find(GA) != ss.end() ||
                     ss.find(CMA) != ss.end() ||
                     ss.find(MA) != ss.end() ){
			cout << "Unsuported params passed to 'TS_scheduler'" << endl;
			exit(1);
		}
    } else if(!sName.compare("HGS_scheduler")){
    	if ( ss.find(SG) != ss.end() ||
			 ss.find(TS) != ss.end() ||
                         ss.find(CMA) != ss.end() ||
                        ss.find(MA) != ss.end() ){
			cout << "Unsuported params passed to 'HGS_scheduler'" << endl;
			exit(1);
		}
    } else if(!sName.compare("SG_scheduler")){
    	if ( ss.find(HGS) != ss.end() ||
			 ss.find(TS) != ss.end() ||
                         ss.find(CMA) != ss.end() ||
                     ss.find(MA) != ss.end()){
			cout << "Unsuported params passed to 'SG_scheduler'" << endl;
			exit(1);
		}
    } else if (!sName.compare("CMA_scheduler")) {
    	if ( ss.find(SG) != ss.end() ||
		     ss.find(HGS) != ss.end() ||
		     ss.find(GA) != ss.end() ||
                     ss.find(TS) != ss.end() ||
                     ss.find(MA) != ss.end() ){
			cout << "Unsuported params passed to 'CMA_scheduler'" << endl;
			exit(1);
		}
    } else if (!sName.compare("MA_scheduler")) {
    	if ( ss.find(SG) != ss.end() ||
		     ss.find(HGS) != ss.end() ||
		     ss.find(GA) != ss.end() ||
                     ss.find(TS) != ss.end() ||
                     ss.find(CMA) != ss.end() ){
			cout << "Unsuported params passed to 'MA_scheduler'" << endl;
			exit(1);
		}
    } else if (!sName.compare("NGA_scheduler")) {
    	if ( ss.find(SG) != ss.end() ||
			 ss.find(HGS) != ss.end() ||
			 ss.find(TS) != ss.end() ||
                         ss.find(CMA) != ss.end() ||
                        ss.find(MA) != ss.end() ){
    		cout << "Unsuported params passed to 'NGA_scheduler'" << endl;
    		exit(1);
    	}
    }


    return 0;
}

void reportParameters  (int totalTasks, int initialTasks, string iaTime,
                        string workLoad, int initialHosts, string mips,
                        string addHost, string delHost, int minHosts,
                        int maxHosts, bool reschedule, string strategy,
                        string activation, string hostSelection,
                        string taskSelection, string allocPolicy,int nRuns,
                        long int seed, bool trace, string traceFile,
                        GAParams params, GAParams (&hgs_params)[4],
                        TabuParams tabuParams, CMAParams cmaParams,
                        MAParams maParams, ostream& out) {

    string reschedule_str = reschedule ? "yes" :"no";
    string trace_str      = trace      ? "yes" : "no";
    string traceFile_str  = trace      ? traceFile: "not used";

    char* buffer = new char[5000];
    sprintf( buffer,
        "---------INITIAL PARAMETERS: ------------------------------------"
        "--------------\n"
        "Total #tasks:   [%15d] Interarrival: [%.29s]\n"
        "Initial #tasks: [%15d] Workload:     [%.29s]\n"
        "Initial #hosts: [%15d] Mips:         [%.29s]\n"
        "Max. #hosts:    [%15d] Add Hosts:    [%.29s]\n"
        "Min. #hosts:    [%15d] Del Hosts:    [%.29s]\n"
        "# Runs:         [%15d] Seed:         [%29ld]\n"
        "Trace:          [%15s] Tracefile:    [%.29s]\n"
        "Reschedule:     [%15s] Strategy:     [%.29s]\n"
        "Host Selection: [%15s] Activation:   [%.29s]\n"
    	"Task Selection: [%15s] Host Policy:  [%.29s]\n"
        "-----------------------------------------------------------------"
        "--------------\n",
        totalTasks, iaTime.c_str(), initialTasks, workLoad.c_str(),
        initialHosts, mips.c_str(), maxHosts, addHost.c_str(), minHosts,
        delHost.c_str(), nRuns, seed, trace_str.c_str(),
        traceFile_str.c_str(), reschedule_str.c_str(), strategy.c_str(),
        hostSelection.c_str(), activation.c_str(), taskSelection.c_str(),
        allocPolicy.c_str()
    );
    out << buffer;
    out << endl;

    // enum ele{ GA, SG, HGS, TS };

	int lb = strategy.find("(");
	string sName;
	if (lb > 0 ) {
		sName = strategy.substr(0,lb);
	}else{
		sName = strategy;
	}

	if (!sName.compare("TS_scheduler")) {

		out << "------------TABU PARAMETERS: --------------------------------------------------" << endl;
		sprintf( buffer,
				"Number iterations:           [%15d] \n"
				"Start Choice:                [%15d] \n"
				"Use Select Delata Function:  [%15d] \n"
				"Tabu Size:                   [%15d] \n"
				"Max Tabu Status:      [%15d] Min Tabu Status:      [%15d]\n"
				"Max Repetitions:      [%15d] \n"
				"Nb. Intensifications: [%15d] Nb. Diversifications: [%15d]\n"
				"Elite Size:           [%15d] Aspiration Value:     [%15d]\n"
				"Max nb. Swaps:        [%15f] Max nb. Transfers:    [%15f]\n"
				"Max Load Interval:    [%15f] Percent Min Load:     [%15f]\n"
				"Security:             [%15s] Security parameter:   [%15f]\n"
				"Energy:               [%15s] Energy case:          [%15d]\n"
				"-----------------------------------------------------------------"
				"--------------\n",
				tabuParams.nb_iterations,
				tabuParams.start_choice,
				tabuParams.use_delta_function,
				tabuParams.tabu_size,
				tabuParams.max_tabu_status, tabuParams.min_tabu_status,
				tabuParams.max_repetitions,
				tabuParams.nb_intensifications, tabuParams.nb_diversifications,
				tabuParams.elite_size, tabuParams.aspiration_value,
				tabuParams.max_nb_swaps, tabuParams.max_nb_transfers,
				tabuParams.max_load_interval, tabuParams.percent_min_load,
				tabuParams.security?"enabled":"disabled", tabuParams.security_param,
				tabuParams.energy?"enabled":"disabled", tabuParams.energy

		);
		out << buffer;
		out << endl;

	}

	if (!sName.compare("GA_scheduler") || !sName.compare("HGS_scheduler")
			|| !sName.compare("SG_scheduler")) {

		if (params.number_branches == 1){
			out << "---------GENETIC PARAMETERS: --------------------------------------------------" << endl;
		}else{
			out << "---------GENETIC PARAMETERS: -------- (branch lvl 1 ) -------------------------" << endl;
		}

		sprintf( buffer,
				"Evolution steps: [%15d] \n"
				"Population size: [%15d] Inter. Pop. Size: [%15d]\n"
				"Cross Prob.:     [%15f] Mutation Prob.:   [%15f]\n"
				"Start Choice:    [%15d] \n"
				"Select Choice:   [%15d] Select Extra:     [%15f]\n"
				"Cross Choice:    [%15d] Cross Extra:      [%15f]\n"
				"Mutate Choice:   [%15d] Mutate Extra:     [%15f]\n"
				"Replace if beter [%15d] Replace Generat.: [%15d]\n"
				"Security:        [%15s] Security param:   [%15f]\n"
				"Energy:          [%15s] Energy case:      [%15d]\n"
				"-----------------------------------------------------------------"
				"--------------\n",
				params.evolution_steps,
				params.pop_size, params.intermediate_size,
				params.prob_cross, params.prob_mutate,
				params.start_choice,
				params.select_choice, params.select_extra,
				params.cross_choice, params.cross_extra,
				params.mutate_choice, params.mutate_extra,
				params.replace_if_better, params.replace_generational,
				params.security?"enabled":"disabled", params.security_param,
				params.energy?"enabled":"disabled", params.energy

		);
		out << buffer;
		out << endl;

	}

	if (!sName.compare("SG_scheduler") ){

		out << "-----STACKELBERG PARAMETERS: --------------------------------------------------" << endl;

		sprintf( buffer,
			"Mutation Size:   [%15d] \n"
			"Env. Choice:     [%15d] Env. Length:      [%15f]\n"
			"Enable Game:     [%15d] Min. Fun. Choice  [%15d]\n"
			"Leader Factor    [%15f] Number Followers  [%15d]\n"
			"Independent GA:  [%15d] Security:         [%15d]\n"
			"Calc. Penalty:   [%15d] Security extra:   [%15f]\n"
			"Enable Ql_e:     [%15d] Enable Ql_u:      [%15d]\n"
			"Enable Ql_ab     [%15d] Abort extra:      [%15f]\n"
			"Energy:          [%15s] Energy case:      [%15d]\n"
			"-----------------------------------------------------------------"
			"--------------\n",
			params.mutation_size,
			params.env_choice, params.env_length,
			params.enable_game, params.min_fun_choice,
			params.leader_factor, params.number_followers,
			params.independent_GA, params.security,
			params.calc_penalty, params.security_extra,
			params.enable_qle, params.enable_qlu,
			params.enable_qlab, params.abort_extra,
			params.energy?"enabled":"disabled", params.energy

		);

		out << buffer;
		out << endl;

	}

	if ( !sName.compare("HGS_scheduler") ){

		out << "-------------HGS PARAMETERS: --------------------------------------------------" << endl;
		sprintf( buffer,
			"Mutation Size:   [%15d] \n"
			"Env. Choice:     [%15d] Env. Length:      [%15f]\n"
			"Number Branches: [%15d] Number Metaepochs [%15d]\n"
			"-----------------------------------------------------------------"
			"--------------\n",
			params.mutation_size,
			params.env_choice, params.env_length,
			params.number_branches, params.number_metaepochs

		);
		out << buffer;
		out << endl;

		for ( int i = 0; i < params.number_branches -1; i++ ){

			/*
			 * hgs_params[0] are 2nd branch (1st is params)
			 */

			out << "------------ HGS PARAMETERS: -------- (branch lvl " << i + 2 << " ) -------------------------" << endl;

			sprintf( buffer,
				"Evolution steps: [%15d] \n"
				"Population size: [%15d] Inter. Pop. Size: [%15d]\n"
				"Cross Prob.:     [%15f] Mutation Prob.:   [%15f]\n"
				"Start Choice:    [%15d] \n"
				"Select Choice:   [%15d] Select Extra:     [%15f]\n"
				"Cross Choice:    [%15d] Cross Extra:      [%15f]\n"
				"Mutate Choice:   [%15d] Mutate Extra:     [%15f]\n"
				"Mutation Size:   [%15d] \n"
				"Replace if beter [%15d] Replace Generat.: [%15d]\n"
				"Env. Choice:     [%15d] Env. Length:      [%15f]\n"
				"Security:        [%15s] Security param:   [%15f]\n"
				"Energy:          [%15s] Energy case:      [%15d]\n"
				"-----------------------------------------------------------------"
				"--------------\n",
				hgs_params[i].evolution_steps,
				hgs_params[i].pop_size, hgs_params[i].intermediate_size,
				hgs_params[i].prob_cross, hgs_params[i].prob_mutate,
				hgs_params[i].start_choice,
				hgs_params[i].select_choice, hgs_params[i].select_extra,
				hgs_params[i].cross_choice, hgs_params[i].cross_extra,
				hgs_params[i].mutate_choice, hgs_params[i].mutate_extra,
				hgs_params[i].mutation_size,
				hgs_params[i].replace_if_better, hgs_params[i].replace_generational,
				hgs_params[i].env_choice, hgs_params[i].env_length,
				hgs_params[i].security?"enabled":"disabled", hgs_params[i].security_param,
				hgs_params[i].energy?"enabled":"disabled", hgs_params[i].energy

				);
				out << buffer;
				out << endl;

		}

	}


        if (!sName.compare("CMA_scheduler")) {

		out << "------------CMA PARAMETERS: --------------------------------------------------" << endl;
		sprintf( buffer,
				"Number gen:      [%15d] \n"
				"Population hei:  [%15d] \n"
				"Population wid:  [%15d] \n"
				"Nb sol to recomb:[%15d] \n"
				"Nmb recomb:      [%15d] Nmb mutations:    [%15d]\n"
				"Start choice:    [%15d] \n"
				"Neighb pattern:  [%15d] Recomb order:     [%15d]\n"
				"Mutation order:  [%15d] Recombine choice: [%15d]\n"
				"Recomb select:   [%15d] Rec select extra: [%15f]\n"
				"Mutate choice:   [%15d] Mut extra param:  [%15f]\n"
				"Max not imp iter:[%15f] Add only if bett: [%15d]\n"
				"Security:        [%15s] Security param:   [%15f]\n"
				"Energy:          [%15s] Energy case:      [%15d]\n"
				"-----------------------------------------------------------------"
				"--------------\n",
				cmaParams._nb_generations,
				cmaParams._population_height,
				cmaParams._population_width,
				cmaParams._nb_solutions_to_recombine,
				cmaParams._nb_recombinations, cmaParams._nb_mutations,
				cmaParams._start_choice,
				cmaParams._neighborhood_pattern, cmaParams._recombination_order,
				cmaParams._mutation_order, cmaParams._recombine_choice,
				cmaParams._recombine_selection, cmaParams._rec_selection_extra,
				cmaParams._mutate_choice, cmaParams._mutate_extra_parameter,
				cmaParams._nb_ls_not_improving_iterations, cmaParams._add_only_if_better,
				cmaParams.security?"enabled":"disabled", cmaParams.security_param,
				cmaParams.energy?"enabled":"disabled", cmaParams.energy

		);
		out << buffer;
		out << endl;

	}

        if (!sName.compare("MA_scheduler")) {

		out << "------------MA PARAMETERS: --------------------------------------------------" << endl;
		sprintf( buffer,
				"Number gen:      [%15d] \n"
				"Population size: [%15d] \n"
				"Nb sol to recomb:[%15d] \n"
				"Nmb recomb:      [%15d] \n"
				"Nmb mutations:   [%15d] Start choice:     [%15d]\n"
				"Select choice:   [%15d] \n"
				"Sel extr param:  [%15f] Cross choice:     [%15d]\n"
				"Recomb selection:[%15d] Recomb sel extra: [%15f]\n"
				"Mutate choice:   [%15d] Mut extra param:  [%15f]\n"
				"Mutate selection:[%15d] Mut select extra: [%15f]\n"
				"Nb ls not imp it:[%15f] Add only if bett: [%15d]\n"
				"Security:        [%15s] Security param:   [%15f]\n"
				"Energy:          [%15s] Energy case:      [%15d]\n"
				"-----------------------------------------------------------------"
				"--------------\n",
				maParams._nb_generations,
				maParams._population_size,
				maParams._nb_solutions_to_recombine,
				maParams._nb_recombinations,
				maParams._nb_mutations, maParams._start_choice,
				maParams._select_choice,
				maParams._select_extra_parameter, maParams._cross_choice,
				maParams._recombine_selection, maParams._rec_selection_extra,
				maParams._mutate_choice, maParams._mutate_extra_parameter,
				maParams._mutate_selection, maParams._mut_selection_extra,
				maParams._nb_ls_not_improving_iterations, maParams._add_only_if_better,
				maParams.security?"enabled":"disabled", maParams.security_param,
				maParams.energy?"enabled":"disabled", maParams.energy

		);
		out << buffer;
		out << endl;

	}

        if (!sName.compare("NGA_scheduler") || !sName.compare("HGS_scheduler")
			|| !sName.compare("SG_scheduler")) {

		if (params.number_branches == 1){
			out << "---------GENETIC PARAMETERS: --------------------------------------------------" << endl;
		}else{
			out << "---------GENETIC PARAMETERS: -------- (branch lvl 1 ) -------------------------" << endl;
		}

		sprintf( buffer,
				"Evolution steps: [%15d] \n"
				"Population size: [%15d] Inter. Pop. Size: [%15d]\n"
				"Cross Prob.:     [%15f] Mutation Prob.:   [%15f]\n"
				"Start Choice:    [%15d] \n"
				"Select Choice:   [%15d] Select Extra:     [%15f]\n"
				"Cross Choice:    [%15d] Cross Extra:      [%15f]\n"
				"Mutate Choice:   [%15d] Mutate Extra:     [%15f]\n"
				"Replace if beter:[%15d] Replace Generat.: [%15d]\n"
                "Struggle replace:[%15d] Struggle ext para:[%15d]\n"
				"Security:        [%15s] Security param:   [%15f]\n"
				"Energy:          [%15s] Energy case:      [%15d]\n"
				"-----------------------------------------------------------------"
				"--------------\n",
				params.evolution_steps,
				params.pop_size, params.intermediate_size,
				params.prob_cross, params.prob_mutate,
				params.start_choice,
				params.select_choice, params.select_extra,
				params.cross_choice, params.cross_extra,
				params.mutate_choice, params.mutate_extra,
				params.replace_if_better, params.replace_generational,
                params.struggle_replace, params.struggle_extra_parameter,
                params.security?"enabled":"disabled", params.security_param,
                params.energy?"enabled":"disabled", params.energy

		);
		out << buffer;
		out << endl;

	}


    delete[] buffer;
}

void processMultipleRuns (GlobalStatistics& gs,
                          IndependentRunsStatistics& irs) {
    irs.addMakespan (gs.getMakespan());
    irs.addFlowTime (gs.getFlowTime());
    irs.addUtilisation (gs.getPercentAverageHostUtilisation());

}
