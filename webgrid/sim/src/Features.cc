#include "Features.hh"

Features::Features() {
    unsetAllFeatures();
}

Features::~Features() {
}

double Features::getRamSize() {
    return ramSize;
}

double Features::getRamAvailable() {
    return ramAvailable;
}

double Features::getVirtualMemorySize() {
    return virtualMemorySize;
}

double Features::getVirtualMemoryAvailable() {
    return virtualMemoryAvailable;
}

string Features::getCpuVendor() {
    return cpuVendor;
}

double Features::getCpuClockSpeed() {
    return cpuClockSpeed;
}

double Features::getCacheL1() {
    return cacheL1;
}

double Features::getCacheL2() {
    return cacheL2;
}

double Features::getFileSystemSize() {
    return fileSystemSize;
}

double Features::getFileSystemAvailableSize() {
    return fileSystemAvailableSize;
}

string Features::getFileSystemType() {
    return fileSystemType;
}

double Features::getLoadLast15Min() {
    return loadLast15Min;
}

double Features::getLoadLast5Min() {
    return loadLast5Min;
}

double Features::getLoadLast1Min() {
    return loadLast1Min;
}

string Features::getOsName() {
    return osName;
}


void Features::setRamSize(double ramSize) {
    this->ramSize = ramSize;
}

void Features::setRamAvailable(double ramAvailable) {
    this->ramAvailable = ramAvailable;
}

void Features::setVirtualMemorySize(double virtualMemorySize) {
    this->virtualMemorySize = virtualMemorySize;
}

void Features::setVirtualMemoryAvailable (double virtualMemoryAvailable) {
    this->virtualMemoryAvailable = virtualMemoryAvailable;
}

void Features::setCpuVendor(string cpuVendor) {
    this->cpuVendor = cpuVendor;
}

void Features::setCpuClockSpeed(double cpuClockSpeed) {
    this->cpuClockSpeed = cpuClockSpeed;
}

void Features::setCacheL1(double cacheL1) {
    this->cacheL1 = cacheL1;
}

void Features::setCacheL2(double cacheL2) {
    this->cacheL2 = cacheL2;
}

void Features::setFileSystemSize(double fileSystemSize) {
    this->fileSystemSize = fileSystemSize;
}

void Features::setFileSystemAvailableSize (double fileSystemAvailableSize){
    this->fileSystemAvailableSize = fileSystemAvailableSize;
}

void Features::setFileSystemType(string fileSystemType) {
    this->fileSystemType = fileSystemType;
}

void Features::setLoadLast15Min(double loadLast15Min) {
    this->loadLast15Min = loadLast15Min;
}

void Features::setLoadLast5Min(double loadLast5Min) {
    this->loadLast5Min = loadLast5Min;
}

void Features::setLoadLast1Min(double loadLast1Min) {
    this->loadLast1Min = loadLast1Min;
}

void Features::setOsName(string osName) {
    this->osName = osName;
}

void Features::unsetAllFeatures() {
    ramSize= ramAvailable= virtualMemorySize= virtualMemoryAvailable= -1.0;
    cpuVendor = "";  cpuClockSpeed= cacheL1= cacheL2= -1.0;
    fileSystemSize= fileSystemAvailableSize= -1.0;
    fileSystemType= "";
    loadLast15Min= loadLast5Min= loadLast1Min = -1.0;
    osName = "";
}
