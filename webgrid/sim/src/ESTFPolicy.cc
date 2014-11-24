#include "ESTFPolicy.hh"

ESTFPolicy::ESTFPolicy () {
}

ESTFPolicy::~ESTFPolicy () {
}

void ESTFPolicy::specificSchedule () {
    if (executing == NULL && host->getState() == STATE_BUSY) {
        /* The first task is the task that is going to be executed, */
        /* so don't sort it!!                                       */
        sort(++scheduled.begin(), scheduled.end(), sortPolicy);
    } else {
        sort (scheduled.begin(), scheduled.end(), sortPolicy);
    }
}

bool ESTFPolicy::sortPolicy(Task* a, Task* b) {
    if (a->getSubmitTime() < b->getSubmitTime()) return true;
    else                                         return false;
}
