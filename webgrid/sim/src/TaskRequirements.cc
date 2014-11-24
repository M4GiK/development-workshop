#include "TaskRequirements.hh"

TaskRequirements::TaskRequirements() {
    TaskRequirements::unsetAllFeatures();
}

TaskRequirements::~TaskRequirements() {
}

double TaskRequirements::getMaxCost() {
	return maxCost;
}

double TaskRequirements::getCostFactor() {
	return costFactor;
}

int TaskRequirements::getNumberCpus() {
	return numberCpus;
}

double TaskRequirements::getMaxQueueTime() {
    return maxQueueTime;
}

double TaskRequirements::getEstimatedWorkload() {
    return estimatedWorkload;
}

double TaskRequirements::getEarliestStartTime() {
    return earliestStartTime;
}

double TaskRequirements::getDeadline() {
    return deadline;
}

void TaskRequirements::setMaxCost (double maxCost) {
	this->maxCost = maxCost;
}

void TaskRequirements::setCostFactor (double costFactor) {
	this->costFactor = costFactor;
}

void TaskRequirements::setNumberCpus (int nCpus) {
	this->numberCpus = nCpus;
}

void TaskRequirements::setMaxQueueTime (double maxQueueTime) {
    this->maxQueueTime = maxQueueTime;
}

void TaskRequirements::setEstimatedWorkload (double estimatedWorkload) {
    this->estimatedWorkload = estimatedWorkload;
}

void TaskRequirements::setEarliestStartTime (double earliestStartTime) {
    this->earliestStartTime = earliestStartTime;
}

void TaskRequirements::setDeadline (double deadline) {
    this->deadline = deadline;
}

void TaskRequirements::unsetAllFeatures () {
    Features::unsetAllFeatures();
    maxQueueTime= estimatedWorkload= earliestStartTime= deadline= -1.0;
}
