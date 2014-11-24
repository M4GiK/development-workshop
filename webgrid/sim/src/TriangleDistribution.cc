#include "TriangleDistribution.hh"

TriangleDistribution::TriangleDistribution(double a, double b, double c,
									   long int seed) :Distribution(seed) {
    this->a = a;
    this->b = b;    
    this->c = c;
}

TriangleDistribution::~TriangleDistribution() {
}

double TriangleDistribution::generate (double x) {
	double cdf_mid, d, y;
	double cdf = ranf(&seed);

	d = 2.0 / ( c - a );
	cdf_mid = 0.5 * d * ( b - a );
	if (cdf <= cdf_mid) {
    	y = a + sqrt ( cdf * ( b - a ) * ( c - a ) );
  	} else {
    	y = c - sqrt (( c - b) * (( c - b) - (cdf - cdf_mid) * (c - a)));
  	}
  	return y;
}
