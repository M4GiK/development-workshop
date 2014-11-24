#include "ConstantDistribution.hh"

ConstantDistribution::ConstantDistribution(double constant_, long int seed)
                     :Distribution(seed) {
    this->constant = constant_;
}

ConstantDistribution::~ConstantDistribution() {
}

double ConstantDistribution::generate(double x) {
    return constant;
}
