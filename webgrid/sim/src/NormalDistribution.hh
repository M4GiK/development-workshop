#ifndef __NORMAL_DISTRIBUTION_H__
#define __NORMAL_DISTRIBUTION_H__
class NormalDistribution;

#include "Distribution.hh"
#include "RandomPackage.hh"

class NormalDistribution: public Distribution {
    public:
        NormalDistribution (double mean, double sd, long int seed=0);
        virtual ~NormalDistribution();

        virtual double generate (double x=0.0);

    protected:
        double mean;
        double sd;
};
#endif
