#include "AllocPolicyFactory.hh"
#include "ESTFPolicy.hh"
#include "FCFSPolicy.hh"
#include "SPTFPolicy.hh"

AllocPolicyFactory::AllocPolicyFactory() {
}


AllocPolicyFactory::~AllocPolicyFactory() {
}


/* To use method polymorphism, it is necessary to work with pointers to */
/* the base clase, and not with base clase objects.                     */
AllocPolicy* AllocPolicyFactory::CreateAllocPolicy (string st){
    AllocPolicy* policy;
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

    if (!sName.compare("fcfs")) {
        if (sParam.size() != 0)
            error("AllocPolicyFactory.cc: No parameters needed");
        policy = new FCFSPolicy();
    } else if (!sName.compare("estf")) {
        if (sParam.size() != 0)
            error("AllocPolicyFactory.cc: No parameters needed");
        policy = new ESTFPolicy();
    } else if (!sName.compare("sptf")) {
        if (sParam.size() != 0)
            error("AllocPolicyFactory.cc: No parameters needed");
        policy = new SPTFPolicy();
    } else {
       error("AllocPolicyFactory.cc: Unknown scheduling policy ("+s+")");
    }

    return policy;
}
