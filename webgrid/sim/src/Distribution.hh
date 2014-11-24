#ifndef __DISTRIBUTION_H__
#define __DISTRIBUTION_H__
class Distribution;

#include "RandomPackage.hh"

class Distribution {
    public:
        Distribution(long int seed = 0);
        virtual ~Distribution();

        void setSeed(long int seed);
        long int getSeed();
        virtual double generate(double x = 0.0) = 0;

    protected:
        long int seed;
};
#endif
