#include "NormalDistribution.hh"

#define abs_(x) ((x) < 0)?(-x):(x)

NormalDistribution::NormalDistribution(double mean,double sd,long int seed)
                    :Distribution(seed) {
    this->mean = mean;
    this->sd   = sd;    
}

NormalDistribution::~NormalDistribution() {
}

double NormalDistribution::generate (double x) {
    double u1,u2;
    double y;

	do {
    	u1 = ranf(&seed);
    	u2 = ranf(&seed);
    	y = sqrt(-2.0*log(u1))*cos(2.0*3.14159265*u2);
    } while ( ((y*sd) < 0) && (abs_(y*sd) >= mean));
    return mean + y*sd;
}
