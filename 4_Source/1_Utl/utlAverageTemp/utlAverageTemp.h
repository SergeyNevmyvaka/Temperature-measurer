/*
 * utlAverageTemp.h
 */

#ifndef UTL_AVERAGE_TEMP_H_
#define UTL_AVERAGE_TEMP_H_

#ifdef	__cplusplus
extern "C" {
#endif

#define AVERAGE_BUFFER_SIZE  	4
#define AVERAGE_BUFFER_DIVIDER	(AVERAGE_BUFFER_SIZE - 2)


float utlAverageTemp_Calculation (float tempr[]);
 

#ifdef	__cplusplus
}
#endif

#endif /* UTL_AVERAGE_TEMP_CALCULATION_H_ */

