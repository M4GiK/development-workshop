#include "mallba.hh"

double one () {
    return 1.0;
}

double zero () {
    return 0.0;
}
 
const double plus_infinity = one() / zero();
const double minus_infinity = - one() / zero();
const double undefined = zero() / zero();
