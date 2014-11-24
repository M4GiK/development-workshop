#ifndef __DISTRIBUTION_FACTORY_H__
#define __DISTRIBUTION_FACTORY_H__
class DistributionFactory;

#include "Distribution.hh"
#include "UtilsPackage.hh"
#include <vector>
#include <string>
using namespace std;

class DistributionFactory {
    public:
        DistributionFactory ();
        virtual ~DistributionFactory();
        virtual Distribution* CreateDistribution(string st,
                                                 long int seed = 0);
};
#endif
