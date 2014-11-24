#ifndef __GRID_CHARACTERISTICS_H__
#define __GRID_CHARACTERISTICS_H__
class GridCharacteristics;

class Calendar; // forward declaration of a not implemented class...

#include "Distribution.hh"
#include <vector>
using namespace std;

class GridCharacteristics {
    friend class Grid;

    public:
        GridCharacteristics();
        virtual ~GridCharacteristics();

        /* ---Grid Characteristics methods --- */
        /* Gets the predicted current load */
        virtual double getPredictedCurrentLoad () = 0;
        /* Gets the average cost at this moment */
        virtual double getAverageCostRate () = 0;
        /* Gets the mean cost at the specified absolut time and timezone */
        virtual double getAverageCostRate (double time, double timeZone)=0;
        /* Indicates if it' holiday now */
        virtual bool isHoliday   () = 0;
        /* Indicates if it will be holiday at the specified time */
        virtual bool isHoliday   (double time, double timeZone) = 0;
        /* Indicates if the entity is working at this moment */
        virtual bool isUp () = 0;
        /* Indicates if the entity will be working at the specified time */
        virtual bool isUp (double time, double timeZone) = 0;
        /* Gets the calendar for this moment */
        virtual Calendar* getCalendar () = 0;
        /* Gets the calendar for the specified moment */
        virtual Calendar* getCalendar (double time, double timeZone) = 0;
        /* Computes the load given at the current time */
        virtual void  computeLoad() = 0;

        /* Get methods */
        virtual double          getTimeZone();
        virtual double          getPeakLoadWeekday();
        virtual double          getOffLoadWeekday();
        virtual double          getPeakLoadHoliday();
        virtual double          getOffLoadHoliday();
        virtual Distribution*   getLoadAdjusting();
        virtual vector<double>& getWeekdayLoad();
        virtual vector<double>& getHolidayLoad();

    protected:
        /* Set methods */
        virtual void setTimeZone (double timeZone);
        virtual void setPeakLoadWeekday (double peakLoadWeekday);
        virtual void setOffLoadWeekday  (double offLoadWeekday);
        virtual void setPeakLoadHoliday (double peakLoadHoliday);
        virtual void setLoadAdjusting (Distribution* loadAdjusting);
        virtual void setOffLoadHoliday  (double offLoadHoliday);
        virtual void setHolidayLoad (vector<double>& holidayLoad);
        virtual void setWeekdayLoad (vector<double>& weekdayLoad);
        virtual void setCalendar (Calendar* calendar);

        double         timeZone;         /* time zone                    */
        double         peakLoadWeekday;  /* peak during weekdays         */
        double         offLoadWeekday;   /* off peak during weekdays     */
        double         peakLoadHoliday;  /* peak during holidays         */
        double         offLoadHoliday;   /* off peak during holidays     */
        Distribution*  loadAdjusting;    /* Parameter to adjust the load */
        vector<double> weekdayLoad;      /* 24 positions, for every hour */
        vector<double> holidayLoad;      /* 24 positions, for every hour */
        Calendar*      calendar;         /* Grid resource calendar       */
};
#endif
