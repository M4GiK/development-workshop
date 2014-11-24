#include "Distribution.hh"

Distribution::Distribution(long int seed) {
    this->seed = seed;
    if (seed == 0) {
        this->seed = genSeed();
    }
}

Distribution::~Distribution(){
}

void Distribution::setSeed(long int seed){
    this->seed = seed;
}

long int Distribution::getSeed() {
    return seed;
}
