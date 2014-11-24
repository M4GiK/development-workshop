#include "TraceDistribution.hh"

TraceDistribution::TraceDistribution (string trace, long int seed)
                  :Distribution(seed) {
    this->traceFile = trace;
    nextItem = 0;
    fd = NULL;
}

TraceDistribution::~TraceDistribution() {
    if (fd) {
        fclose(fd);
    }
    fd = NULL;
}

void TraceDistribution::fetch (int index) {
    double item;

    if (fd == NULL) {
        if ((fd = fopen (traceFile.c_str(), "r")) == NULL) {
            error ("TraceDistribution.cc: Can't open " + traceFile);
        }
    }

    if (((unsigned long int) index) < nextItem || nextItem == 0) {
        fseek (fd, 0L, SEEK_SET);
        nextItem = 0;
    }
    for(; nextItem < ((unsigned long int)index - 1); nextItem++) {
        int count = fscanf (fd,"%lg",&item);
        if (feof (fd) || count != 1) {
            fseek (fd, 0L, SEEK_SET);
            nextItem = 0;
            break;
        }
    }
}

double TraceDistribution::generate(double x) {
    int index = (int) x;
    int count;
    double item;

    fetch (index);
    count = fscanf (fd, "%lg", &item);
    if (feof (fd) || count != 1) {
        nextItem = 0;
    }
    nextItem++;
    return item;
}
