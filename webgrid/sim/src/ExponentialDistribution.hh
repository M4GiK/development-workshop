#ifndef __EXPONENTIAL_DISTRIBUTION_H__
#define __EXPONENTIAL_DISTRIBUTION_H__
class ExponentialDistribution;

#include "Distribution.hh"
#include "RandomPackage.hh"

class ExponentialDistribution: public Distribution {
    public:
        ExponentialDistribution (double mean, long int seed=0);
        virtual ~ExponentialDistribution();

        virtual double generate (double x=0.0);

    protected:
        double mean;
};
#endif
