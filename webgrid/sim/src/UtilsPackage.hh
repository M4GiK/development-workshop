#ifndef __UTILS_PACKAGE_H__
#define __UTILS_PACKAGE_H__

#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <stdarg.h>
#include <cstdlib>
using namespace std;

  vector<string> splitString(string s,string d);
  string repeat(string str, int times);
  double parseDouble(char const *s);
  long int parseInteger(char const *s);
  string doubleToString(double d);
  string integerToString(long int i);
  void error (string msg);
  vector<double> wrap (int number, ...);
  double infinity();

#endif
