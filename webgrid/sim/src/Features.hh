#ifndef __FEATURES_H__
#define __FEATURES_H__
class Features;

#include <string>
using namespace std;
class Features {
    public:
        Features ();
        virtual ~Features();

        /* Get Methods */
        virtual double   getRamSize();
        virtual double   getRamAvailable();
        virtual double   getVirtualMemorySize();
        virtual double   getVirtualMemoryAvailable();
        virtual string   getCpuVendor();
        virtual double   getCpuClockSpeed();
        virtual double   getCacheL1();
        virtual double   getCacheL2();

        virtual double   getFileSystemSize();
        virtual double   getFileSystemAvailableSize();
        virtual string   getFileSystemType();
        virtual double   getLoadLast15Min();
        virtual double   getLoadLast5Min();
        virtual double   getLoadLast1Min();
        virtual string   getOsName();

    protected:
        /* Set Methods */
        virtual void    setRamSize(double ramSize);
        virtual void    setRamAvailable(double ramAvailable);
        virtual void    setVirtualMemorySize (double vMemorySize);
        virtual void    setVirtualMemoryAvailable(double vMemoryAvailable);
        virtual void    setCpuVendor(string cpuVendor);
        virtual void    setCpuClockSpeed(double cpuClockSpeed);
        virtual void    setCacheL1(double cacheL1);
        virtual void    setCacheL2(double cacheL2);

        virtual void    setFileSystemSize (double fileSystemSize);
        virtual void    setFileSystemAvailableSize(double fsAvailableSize);
        virtual void    setFileSystemType(string fileSystemType);
        virtual void    setLoadLast15Min(double loadLast15Min);
        virtual void    setLoadLast5Min(double loadLast5Min);
        virtual void    setLoadLast1Min(double loadLast1Min);
        virtual void    setOsName(string osName);

        /* Other Methods */
        virtual void    unsetAllFeatures ();

        double     ramSize;                 /*  RAM size                 */
        double     ramAvailable;            /*  Available RAM            */
        double     virtualMemorySize;       /*  Virtual Memory Size      */
        double     virtualMemoryAvailable;  /*  Virtual memory Available */
        string     cpuVendor;               /*CPU vendor and architecture*/
        double     cpuClockSpeed;           /*  CPU clock speed          */
        double     cacheL1;                 /*  L1 Cache size            */
        double     cacheL2;                 /*  L2 Cache size            */

        double     fileSystemSize;          /*  File System Size         */
        double     fileSystemAvailableSize; /* File System Available Size*/
        string     fileSystemType;          /*  File System Type         */
        double     loadLast15Min;           /*  Load During last 15 Min  */
        double     loadLast5Min;            /*  Load During last 5 Min   */
        double     loadLast1Min;            /*  Load During last 1 Min   */
        string     osName;                  /*  OS Name                  */
};
#endif
