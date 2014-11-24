#ifndef __ZIPF_DISTRIBUTION_H__
#define __ZIPF_DISTRIBUTION_H__
class ZipfDistribution;

#include "Distribution.hh"
#include "RandomPackage.hh"
#include <math.h>

class ZipfDistribution: public Distribution {
    public:
        ZipfDistribution (double x1, double x2, double p, long int seed=0);
        virtual ~ZipfDistribution();

        virtual double generate (double y=0.0);

    protected:
        double x1;          /*  n1 */
        double x2;          /*  n2 */
        double p;           /*  t  */
};
#endif
