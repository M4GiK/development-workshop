#ifndef __RANDOM_PACKAGE_H__
#define __RANDOM_PACKAGE_H__

#include <math.h>
#include <stdlib.h>
#include "UtilsPackage.hh"

  void sranf(long int const seed);
  long int genSeed();
  double ranf(long int *seed);
  double ranf();
  int randint (int a, int b);
  double expon(double const mean);
  double uniform(double const a,double const b);
  double gam(double const a);
  double erlang(int const a);
  double triangle(double const a);
  double normal(double const m,double const s);
  double beta(double const a,double const b);
  double qnorm(double p , bool upper);
  double student (double p, double ndf, bool lower_tail);

#endif
