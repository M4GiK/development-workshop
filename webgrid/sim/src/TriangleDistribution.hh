#ifndef __TRIANGLE_DISTRIBUTION_H__
#define __TRIANGLE_DISTRIBUTION_H__
class TriangleDistribution;

#include "Distribution.hh"
#include "RandomPackage.hh"

class TriangleDistribution: public Distribution {
    public:
        TriangleDistribution (double a, double b, double c, long int seed=0);
        virtual ~TriangleDistribution();

        virtual double generate (double x=0.0);

    protected:
        double a;
        double b;
        double c;
};
#endif
