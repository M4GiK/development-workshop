#ifndef __CONSTANT_DISTRIBUTION_H__
#define __CONSTANT_DISTRIBUTION_H__
class ConstantDistribution;

#include "Distribution.hh"

class ConstantDistribution: public Distribution {
    public:
        ConstantDistribution(double constant, long int seed=0);
        virtual ~ConstantDistribution();

        virtual double generate(double x=0.0);

    protected:
        double constant;
};
#endif
