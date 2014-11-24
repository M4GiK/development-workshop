#include "SPTFPolicy.hh"

SPTFPolicy::SPTFPolicy () {
}

SPTFPolicy::~SPTFPolicy () {
}

void SPTFPolicy::specificSchedule () {
    if (executing == NULL && host->getState() == STATE_BUSY) {
        /* The first task is the task that is going to be executed, */
        /* so don't sort it!!                                       */
        sort(++scheduled.begin(), scheduled.end(), sortPolicy);
    } else {
        sort (scheduled.begin(), scheduled.end(), sortPolicy);
    }
}

bool SPTFPolicy::sortPolicy(Task* a, Task* b) {
    if ((a->getEstimatedExecutionTime())<(b->getEstimatedExecutionTime())){
        return true;
    } else {
        return false;
    }

}
