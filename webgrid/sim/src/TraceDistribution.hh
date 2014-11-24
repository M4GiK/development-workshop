#ifndef __TRACE_DISTRIBUTION_H__
#define __TRACE_DISTRIBUTION_H__
class TraceDistribution;

#include "Distribution.hh"
#include "RandomPackage.hh"
#include <string>
using namespace std;

class TraceDistribution: public Distribution {
    public:
        TraceDistribution(string traceFile, long int seed=0);
        virtual ~TraceDistribution();

        virtual double generate(double x = 0.0);

    protected:
        virtual void fetch (int index);

        FILE* fd;                           /*  file handler  */
        unsigned long int nextItem;         /*  item index    */
        string traceFile;                   /*  trace file    */
};
#endif
