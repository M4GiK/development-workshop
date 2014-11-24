#ifndef __UNIFORM_DISTRIBUTION_H__
#define __UNIFORM_DISTRIBUTION_H__
class UniformDistribution;

#include "Distribution.hh"
#include "RandomPackage.hh"

class UniformDistribution: public Distribution {
    public:
        UniformDistribution (double lo, double hi, long int seed=0);
        virtual ~UniformDistribution();

        virtual double generate (double x=0.0);

    protected:
        double hi;
        double lo;
        double num;         /* hi - lo */
};
#endif
