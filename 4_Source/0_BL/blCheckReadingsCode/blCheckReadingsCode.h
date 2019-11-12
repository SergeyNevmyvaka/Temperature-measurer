/*
 * blTempWithK.h
 */

#ifndef BL_CHECK_READINGS_CODE_H__
#define BL_CHECK_READINGS_CODE_H__

#ifdef	__cplusplus
extern "C" {
#endif

#include "../blStatusControl/blStatusControl.h"



unsigned char blCheckReadingsCode_CalculateReadings (unsigned long currReadings);
unsigned char blCheckReadingsCode_CalculateDate (unsigned char currDay, unsigned char currMonth);

void blCheckReadingsCode_Handler(S_DEVICE *device);

#ifdef	__cplusplus
}
#endif

#endif /* CHECK_READINGS_CODE */

