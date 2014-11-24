#include "HostCharacteristics.hh"

HostCharacteristics::HostCharacteristics() {
    HostCharacteristics::unsetAllFeatures();
}

HostCharacteristics::~HostCharacteristics() {
}

double HostCharacteristics::getMaxLoad() {
    return maxLoad;;
}

double HostCharacteristics::getMinLoad() {
    return minLoad;
}

double HostCharacteristics::getMaxWorkload() {
    return maxWorkload;
}

double HostCharacteristics::getMinWorkload() {
    return minWorkload;
}

void HostCharacteristics::setMaxLoad (double maxLoad) {
    this->maxLoad= maxLoad;
}

void HostCharacteristics::setMinLoad (double minLoad) {
    this->minLoad = minLoad;
}

void HostCharacteristics::setMaxWorkload (double maxWorkload) {
    this->maxWorkload = maxWorkload;
}

void HostCharacteristics::setMinWorkload (double minWorkload) {
    this->minWorkload = minWorkload;
}

void HostCharacteristics::unsetAllFeatures () {
    Features::unsetAllFeatures();
    maxLoad= minLoad= maxWorkload= minWorkload= -1.0;
}
