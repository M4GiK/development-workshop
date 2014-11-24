#include "ZipfDistribution.hh"

ZipfDistribution::ZipfDistribution ( double x1, double x2,  double p,
                                     long int seed): Distribution(seed) {
    this->x1 = x1;
    this->x2 = x2;
    this->p = p;
}

ZipfDistribution::~ZipfDistribution() {
}

double ZipfDistribution::generate (double y) {
    double x;
    int i;
    double r, HsubV, sum;
    int V = 100;

    HsubV = 0.0;
    for(i=1; i<=V; i++) {
        HsubV += 1.0/pow( (double)i, p);
    }

    r = ranf(&seed)*HsubV;
    sum = 1.0;
    i = 1;
    while(sum < r) {
        i++;
        sum += 1.0/pow( (double)i, p);
    }

    x = ( (double) i - 1. ) / ( (double) V - 1. );
    x = (x2 - x1) * x + x1;

    return x;
}
