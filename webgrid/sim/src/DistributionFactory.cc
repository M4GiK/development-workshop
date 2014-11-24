#include "DistributionFactory.hh"
#include "ConstantDistribution.hh"
#include "UniformDistribution.hh"
#include "ExponentialDistribution.hh"
#include "NormalDistribution.hh"
#include "ZipfDistribution.hh"
#include "TriangleDistribution.hh"
#include "TraceDistribution.hh"

DistributionFactory::DistributionFactory() {
}

DistributionFactory::~DistributionFactory() {
}


/* To use method polymorphism, it is necessary to work with pointers to */
/* the base clase, and not with base clase objects. For example :       */
/*   -----code 1-----------  --------------------code 2---------------- */
/*    Distribution d;         Distribution* d;                          */
/*    CDistribution cd(2);    CDistribution* cd = new CDistribution(2); */
/*    d = cd;                 d = cd;                                   */
/*    d.generate ()           d->generate();                            */
/*   -----code 1-----------  --------------------code 2---------------- */
/* The first code won't generate a sample from ConstantDistribution,but */
/* from Distribution (if it wasn't abstract). The second code generates */
/* a sample from ConstantDistribution...                                */

Distribution* DistributionFactory::CreateDistribution (string st,
                                                       long int seed){
    Distribution* dist;
    string dName, s = st;
    vector<string> dParam;
    int lb, rb;

    dist = NULL;
    lb = s.find("(");
    rb = s.find(")");
    if (lb > 0 && rb > lb) {
        dName = s.substr(0,lb);
        dParam = splitString(s.substr(lb+1,rb-lb-1),",");
    } else {
        dName = "c";
        dParam = splitString(s,",");
    }

    if (!dName.compare("c")) {
        if (dParam.size() != 1)
            error("DistributionFactory.cc: 1 parameter needed");
        dist = new ConstantDistribution( parseDouble(dParam[0].c_str()),
                                         seed );
    } else if (!dName.compare("u")) {
        if (dParam.size() != 2)
            error("DistributionFactory.cc: 2 parameters needed");
        dist = new UniformDistribution( parseDouble(dParam[0].c_str()),
                                        parseDouble(dParam[1].c_str()),
                                        seed );
    } else if (!dName.compare("e")) {
        if (dParam.size() != 1)
            error("DistributionFactory.cc: 1 parameter needed");
        dist = new ExponentialDistribution( parseDouble(dParam[0].c_str()),
                                            seed );
    } else if (!dName.compare("n")) {
        if (dParam.size() != 2)
            error("DistributionFactory.cc: 2 parameters nedded");
        dist = new NormalDistribution( parseDouble(dParam[0].c_str()),
                                       parseDouble(dParam[1].c_str()),
                                       seed );
    } else if (!dName.compare("zipf")) {
        if (dParam.size() != 3)
            error("DistributionFactory.cc: 3 parameters needed");
        dist = new ZipfDistribution( parseDouble(dParam[0].c_str()),
                                     parseDouble(dParam[1].c_str()),
                                     parseDouble(dParam[2].c_str()), seed);
    } else if (!dName.compare("triangle")) {
        if (dParam.size() != 3)
            error("DistributionFactory.cc: 3 parameters needed");
        dist = new TriangleDistribution( parseInteger(dParam[0].c_str()),
                                         parseInteger(dParam[1].c_str()),
                                         parseInteger(dParam[2].c_str()),
                                         seed );
    } else if (!dName.compare("trace")) {
        if (dParam.size() != 1)
            error("DistributionFactory.cc: 1 parameter needed");
        dist = new TraceDistribution ( dParam[0].c_str(), seed );
    } else {
       error("DistributionFactory.cc: unknown distribution (" + s + ")");
    }
    return dist;
}
