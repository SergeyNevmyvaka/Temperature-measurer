#include "utlCalendar.h"

#include "../../0_BL/blStatusControl/blStatusControl.h"

#include "../../2_Drv/drvSysClock/drvSysClock.h"
#include "../utlBCD/utlBCD.h"


#define DEFAULT_PERIOD      4


void utlCalendar_AddSecond(S_DEVICE *device)
{
	drvSysClock_run();

	if (device->ADD_TIME)
	{
		device->CURRENT_MINUTE += DEFAULT_PERIOD;

        if (device->CURRENT_MINUTE > 59)
        {
        	device->CURRENT_MINUTE -= 60;
        	device->CURRENT_HOUR++;

            if (device->CURRENT_HOUR > 23)
            {
            	device->CURRENT_HOUR = 0;
            	device->CURRENT_DAY++;

                if (device->CURRENT_MONTH == UTL_CALENDAR_APRIL ||
                	device->CURRENT_MONTH == UTL_CALENDAR_JUNE ||
                	device->CURRENT_MONTH == UTL_CALENDAR_SEPTEMBER ||
                	device->CURRENT_MONTH == UTL_CALENDAR_NOVEMBER)
                {
                    if (device->CURRENT_DAY > 30)
                    {
                    	device->CURRENT_DAY = 1;
                    	device->CURRENT_MONTH++;
                    }
                }
                else if (device->CURRENT_MONTH == UTL_CALENDAR_FEBRUARY)
                {
                    if ((device->CURRENT_YEAR % 4 == 0) && (device->CURRENT_YEAR != UTL_CALENDAR_EXTRA_LEAP_YEAR)) // условие корректно до 2400 года
                    {
                        if (device->CURRENT_DAY > 29)
                        {
                        	device->CURRENT_DAY = 1;
                        	device->CURRENT_MONTH++;
                        }
                    }
                    else
                    {
                        if (device->CURRENT_DAY > 28)
                        {
                        	device->CURRENT_DAY = 1;
                        	device->CURRENT_MONTH++;
                        }

                    }
                }
                else
                {
                    if (device->CURRENT_DAY > 31)
                    {
                    	device->CURRENT_DAY = 1;
                    	device->CURRENT_MONTH++;
                    }
                }

                if (device->CURRENT_MONTH > UTL_CALENDAR_DECEMBER )
                {
                	device->CURRENT_MONTH = UTL_CALENDAR_JANUARY;
                	device->CURRENT_YEAR++;
                }
            }
        }
        else
        {
        }
    }

	device->ADD_TIME = 0;

}

