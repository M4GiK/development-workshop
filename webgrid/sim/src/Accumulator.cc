#include "Accumulator.hh"

Accumulator::Accumulator(double clock) {
    valCount        = 0;
    this->clock     = clock;
    valMean         = 0.0;
    valSqrMean      = 0.0;
    valMin          = DBL_MAX;
    valMax          = DBL_MIN;
    valLast         = 0.0;
    valArea         = 0.0;
    valSum          = 0.0;
    valArea_freq    = 0.0;
    valSqrArea_freq = 0.0;
    freqSum_freq    = 0.0;
    first_time      = true;
}

Accumulator::~Accumulator() {
}

void Accumulator::add (double item, double clock, int times){
    /* If two items with the same time are added, this function keeps   */
    /* the last one, computing correctly mean_freq, sd_freq, area_freq. */
    valCount += times;
    valSum += item*times;
    if (valCount == 0) {
        valMean = valMin = valMax = valSum = item;
        valSqrMean = item*item;
        valArea = 0.0;
        valArea_freq = 0.0;
        valSqrArea_freq = 0.0;
        freqSum_freq = 0.0;
    } else {
        if (first_time) {
            valLast = item;
            this->clock = clock;
            first_time = false;
        }
        valMean =   ((valCount-times)*valMean+item*times)/valCount;
        valSqrMean= ((valCount-times)*valSqrMean+item*item*times)/valCount;

        if (item < valMin) { valMin = item; }
        if (item > valMax) { valMax = item; }
        if (clock > this->clock) {
            valArea        += (clock-this->clock) * (item + valLast) * 0.5;

            valArea_freq    +=  (clock - this->clock) * valLast;
            valSqrArea_freq += (clock - this->clock) * valLast * valLast;
            freqSum_freq    += (clock - this->clock);
        }
    }
    valLast = item;
    this->clock = clock;
}

double Accumulator::min() {
    return valMin;
}

double Accumulator::max() {
    return valMax;
}

double Accumulator::last() {
    return valLast;
}

double Accumulator::count() {
    return valCount;
}

double Accumulator::sum() {
    return valSum;
}

double Accumulator::mean() {
    return valMean;
}

double Accumulator::sd() {
    double val = valSqrMean-valMean*valMean;
    if (val < 0) return 0;          /* Due to double imprecission */
    else         return sqrt(val);
}

double Accumulator::area() {
    return valArea;
}

double Accumulator::confidence_interval (double conf) {
    double alfa = 1 - conf;
    double p    = 1 - (alfa/2);
    double ndf  = valCount - 1;
    double stdev   = sd();

    return student(p, ndf, true )* sqrt((stdev*stdev)/(ndf+1));
}

double Accumulator::mean_freq() {
    return valArea_freq / freqSum_freq;
}

double Accumulator::sd_freq() {
    double valSqrMean_freq = valSqrArea_freq / freqSum_freq;
    double variance_freq   = valSqrMean_freq - (mean_freq() * mean_freq());
    if (variance_freq < 0) return 0;  /* Due to double imprecission */
    else                   return sqrt(variance_freq);
}

double Accumulator::area_freq() {
    return valArea_freq;
}
