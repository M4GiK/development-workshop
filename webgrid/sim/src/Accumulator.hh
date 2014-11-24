#ifndef __ACCUMULATOR_H__
#define __ACCUMULATOR_H__
class Accumulator;

#include "RandomPackage.hh"
#include <float.h>
#include <math.h>
using namespace std;

class Accumulator {
    public:
        Accumulator(double clock = 0.0);
        virtual ~Accumulator ();

        /* First added time, is considered to be the start time */
        virtual void add (double item, double clock = 0.0, int times=1);
        double min();
        double max();
        double last();
        double count();
        double sum();

        double mean();
        double sd();
        double area();
        double confidence_interval (double conf);

        double mean_freq();
        double sd_freq();
        double area_freq();

    protected:
        int valCount;
        double valMean;
        double valSqrMean;
        double valMin;
        double valMax;
        double valLast;
        double valArea;
        double valArea_freq;
        double valSqrArea_freq;
        double freqSum_freq;
        double valSum;
        double clock;
        bool   first_time;
};
#endif
