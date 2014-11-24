#include "ExponentialDistribution.hh"

ExponentialDistribution::ExponentialDistribution(double mean,long int seed)
                        :Distribution(seed) {
    this->mean = mean;
}


ExponentialDistribution::~ExponentialDistribution() {
}

double ExponentialDistribution::generate(double x) {
    return -mean * log(ranf(&seed));
}
