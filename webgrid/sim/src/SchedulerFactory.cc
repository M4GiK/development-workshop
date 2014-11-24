#include "SchedulerFactory.hh"
#include "SimpleMetaPolicy.hh"
#include "AdHocScheduler.hh"
#include "GAScheduler.hh"
#include "TSScheduler.hh"
#include "TimeIntervalActivation.hh"
#include "ResourceAndTimeActivation.hh"
#include "SelectAllHosts.hh"
#include "SelectIdleHosts.hh"
#include "SelectAllTasks.hh"
#include "HgsScheduler.hh"
#include "StackelbergScheduler.hh"
#include "CMAScheduler.hh"
#include "MAScheduler.hh"
#include "NGAScheduler.hh"

SchedulerFactory::SchedulerFactory() {
}


SchedulerFactory::~SchedulerFactory() {
}

/* To use method polymorphism, it is necessary to work with pointers to */
/* the base clase, and not with base clase objects.                     */
SchedulingPolicy* SchedulerFactory::CreateSchedulingPolicy (string st,
		GAParams params, GAParams (&hgs_params)[4], TabuParams tabuParams,
                CMAParams cmaParams, MAParams maParams){
    SchedulingPolicy* policy;
    string sName, s = st;
    vector<string> sParam;
    int lb, rb;

    policy = NULL;
    lb = s.find("(");
    rb = s.rfind(")");
    if (lb > 0 && rb > lb) {
        sName = s.substr(0,lb);
        sParam = splitString(s.substr(lb+1,rb-lb-1),",");
    } else {
        sName = s;
        sParam = vector<string> ();
    }



    if (!sName.compare("simple_metapolicy")) {
        if (sParam.size() != 0)
            error("SchedulerFactory.cc: No parameters needed");
        policy = new SimpleMetaPolicy ();



    } else if (!sName.compare("GA_scheduler")) {
        if (sParam.size() != 2)
            error("SchedulerFactory.cc: 2 parameters needed");        
        bool h_s = false;        
        if      (!sParam[1].compare("h"))   h_s = false;
        else if (!sParam[1].compare("s"))   h_s = true;
        else      error("SchedulerFactory.cc: must indicate h(ierachical) " 
                        "or s(imultaneous)");
        policy = new GAScheduler (parseDouble(sParam[0].c_str()),h_s,params);



    } else if (!sName.compare("TS_scheduler")) {
        if (sParam.size() != 2)
            error("SchedulerFactory.cc: 2 parameters needed");        
        bool h_s = false;        
        if      (!sParam[1].compare("h"))   h_s = false;
        else if (!sParam[1].compare("s"))   h_s = true;
        else      error("SchedulerFactory.cc: must indicate h(ierachical) " 
                        "or s(imultaneous)");
        policy = new TSScheduler (parseDouble(sParam[0].c_str()),h_s,tabuParams);



    } else if (!sName.compare("AdHoc_scheduler")) {
        if (sParam.size() != 1)
            error("SchedulerFactory.cc: 1 parameter needed");
        policy = new AdHocScheduler(sParam[0]);



    } else if(!sName.compare("HGS_scheduler")){//lukkol
    	if (sParam.size() != 2)
    		error("SchedulerFactory.cc: 2 parameters needed");
		bool h_s = false;
		if      (!sParam[1].compare("h"))   h_s = false;
		else if (!sParam[1].compare("s"))   h_s = true;
		else      error("SchedulerFactory.cc: must indicate h(ierachical) "
					"or s(imultaneous)");
		policy = new HgsScheduler (parseDouble(sParam[0].c_str()),h_s, params, hgs_params);



	} else if(!sName.compare("SG_scheduler")){
    	if (sParam.size() != 2)
    		error("SchedulerFactory.cc: 2 parameters needed");
		bool h_s = false;
		if      (!sParam[1].compare("h"))   h_s = false;
		else if (!sParam[1].compare("s"))   h_s = true;
		else      error("SchedulerFactory.cc: must indicate h(ierachical) "
					"or s(imultaneous)");
		policy = new StackelbergScheduler (parseDouble(sParam[0].c_str()),h_s,
				params);


	} else if(!sName.compare("CMA_scheduler")){
    	if (sParam.size() != 2)
    		error("SchedulerFactory.cc: 2 parameters needed");
		policy = new CMAScheduler (parseDouble(sParam[0].c_str()),
                        (parseInteger(sParam[0].c_str())),
				cmaParams);


	} else if(!sName.compare("MA_scheduler")){
    	if (sParam.size() != 2)
    		error("SchedulerFactory.cc: 2 parameters needed");
		policy = new MAScheduler (parseDouble(sParam[0].c_str()),
                        (parseInteger(sParam[0].c_str())),
				maParams);


	} else if (!sName.compare("NGA_scheduler")) {
        if (sParam.size() != 2)
            error("SchedulerFactory.cc: 2 parameters needed");
        bool h_s = false;
        if      (!sParam[1].compare("h"))   h_s = false;
        else if (!sParam[1].compare("s"))   h_s = true;
        else      error("SchedulerFactory.cc: must indicate h(ierachical) "
                        "or s(imultaneous)");
        policy = new NGAScheduler (parseDouble(sParam[0].c_str()),h_s,params);



    }else{
       error("SchedulerFactory.cc: Unknown scheduling policy (" + s + ")");
    }

    return policy;
}

/* To use method polymorphism, it is necessary to work with pointers to */
/* the base clase, and not with base clase objects.                     */
SchedulerActivation* SchedulerFactory::CreateSchedulerActivation(string st)
{
    DistributionFactory df;
    SchedulerActivation* policy;
    string sName, s = st;
    vector<string> sParam;
    int lb, rb;

    policy = NULL;
    lb = s.find("(");
    rb = s.rfind(")");
    if (lb > 0 && rb > lb) {
        sName = s.substr(0,lb);
        sParam = splitString(s.substr(lb+1,rb-lb-1),";");
    } else {
        sName = s;
        sParam = vector<string> ();
    }

    if (!sName.compare("time_interval")) {
        if (sParam.size() != 1)
            error("SchedulerFactory.cc: 1 parameter needed");
        policy = new TimeIntervalActivation (
                        df.CreateDistribution (sParam[0].c_str())
                 );
    } else if (!sName.compare("resource_and_time_interval")) {
        if (sParam.size() != 1)
            error("SchedulerFactory.cc: 1 parameter needed");
        policy = new ResourceAndTimeActivation (
                        df.CreateDistribution (sParam[0].c_str())
                 );
    } else {
       error("SchedulerFactory.cc: Unknown scheduler activation ("+s+ ")");
    }

    return policy;
}

/* To use method polymorphism, it is necessary to work with pointers to */
/* the base clase, and not with base clase objects.                     */
HostSelectionPolicy* SchedulerFactory::CreateHostSelectionPolicy
                                       (string st){
    HostSelectionPolicy* policy;
    string sName, s = st;
    vector<string> sParam;
    int lb, rb;

    policy = NULL;
    lb = s.find("(");
    rb = s.rfind(")");
    if (lb > 0 && rb > lb) {
        sName = s.substr(0,lb);
        sParam = splitString(s.substr(lb+1,rb-lb-1),";");
    } else {
        sName = s;
        sParam = vector<string> ();
    }

    if (!sName.compare("all")) {
        if (sParam.size() != 0)
            error("SchedulerFactory.cc: No parameters needed");
        policy = new SelectAllHosts();
    } else if (!sName.compare ("idle")) {
        if (sParam.size() != 0)
            error("SchedulerFactory.cc: No parameters needed");
        policy = new SelectIdleHosts();
    } else {
       error("SchedulerFactory.cc: Unknown host selection policy "
             "(" + s + ")");
    }

    return policy;
}

/* To use method polymorphism, it is necessary to work with pointers to */
/* the base clase, and not with base clase objects.                     */
TaskSelectionPolicy* SchedulerFactory::CreateTaskSelectionPolicy
                                       (string st){
    TaskSelectionPolicy* policy;
    string sName, s = st;
    vector<string> sParam;
    int lb, rb;

    policy = NULL;
    lb = s.find("(");
    rb = s.rfind(")");
    if (lb > 0 && rb > lb) {
        sName = s.substr(0,lb);
        sParam = splitString(s.substr(lb+1,rb-lb-1),";");
    } else {
        sName = s;
        sParam = vector<string> ();
    }

    if (!sName.compare("all")) {
        if (sParam.size() != 0)
            error("SchedulerFactory.cc: No parameters needed");
        policy = new SelectAllTasks();
    } else {
       error("SchedulerFactory.cc: Unknown host selection policy "
             "(" + s + ")");
    }

    return policy;
}
