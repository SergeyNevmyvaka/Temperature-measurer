/*
 * blCountingRate.h
 */

#ifndef BL_COUNTING_RATE_H_
#define BL_COUNTING_RATE_H_

#ifdef	__cplusplus
extern "C" {
#endif

#include "../blStatusControl/blStatusControl.h"

#define BL_CR_COUNTING_START_TEMPERATURE           28//30
#define BL_CR_COUNTING_START_TEMPERATURE_SUMMER    40


void blCountingRate_Handler(S_DEVICE *device);


#ifdef	__cplusplus
}
#endif

#endif /* UTL_COUNTING_RATE_H_ */

