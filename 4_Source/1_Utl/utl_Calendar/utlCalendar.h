/*
 * utlCalendar.h
 */

#ifndef UTL_CALENDAR_H_
#define UTL_CALENDAR_H_

#ifdef	__cplusplus
extern "C" {
#endif


#include "../../0_BL/blStatusControl/blStatusControl.h"


#define UTL_CALENDAR_TRUE           1
#define UTL_CALENDAR_FALSE          0

#define UTL_CALENDAR_MONDAY         0
#define UTL_CALENDAR_TUESDAY        1
#define UTL_CALENDAR_WEDNESDAY      2
#define UTL_CALENDAR_THURSDAY       3
#define UTL_CALENDAR_FRIDAY         4
#define UTL_CALENDAR_SATURDAY       5
#define UTL_CALENDAR_SUNDAY         6

#define UTL_CALENDAR_JANUARY        1
#define UTL_CALENDAR_FEBRUARY       2
#define UTL_CALENDAR_MARCH          3
#define UTL_CALENDAR_APRIL          4
#define UTL_CALENDAR_MAY            5
#define UTL_CALENDAR_JUNE           6
#define UTL_CALENDAR_JULY           7
#define UTL_CALENDAR_AUGUST         8
#define UTL_CALENDAR_SEPTEMBER      9
#define UTL_CALENDAR_OCTOBER        10
#define UTL_CALENDAR_NOVEMBER       11
#define UTL_CALENDAR_DECEMBER       12

#define UTL_CALENDAR_DEF_YEAR       16

#define UTL_CALENDAR_EXTRA_LEAP_YEAR 00


typedef struct {
    unsigned char day;
    unsigned char month;
    unsigned char year;
} SCalendarDate;

typedef struct {
    unsigned char second;
    unsigned char minute;
    unsigned char hour;
} SCalendarTime;


void utlCalendar_AddSecond (S_DEVICE *device);


#ifdef	__cplusplus
}
#endif

#endif /* UTL_CALENDAR_H_ */

