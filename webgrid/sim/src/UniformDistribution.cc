#include "UniformDistribution.hh"

UniformDistribution::UniformDistribution(double lo,double hi,long int seed)
                    :Distribution(seed) {
    this->lo = lo;
    this->hi = hi;
    this->num = hi-lo;
}

UniformDistribution::~UniformDistribution() {
}

double UniformDistribution::generate (double x) {
    return lo+num*ranf(&seed);
}
