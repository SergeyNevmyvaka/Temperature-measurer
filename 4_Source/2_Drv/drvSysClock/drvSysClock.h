/*
 * modSysClock.h
 *
 *  Created on: 20.11.2012
 *      Author: a.tkachenko
 */

#ifndef __DRV_SYS_CLK_H__
#define __DRV_SYS_CLK_H__

#ifdef	__cplusplus
extern "C" {
#endif

#define SYSCLOCK_GET_TIME_MKS_10          50 //0
#define SYSCLOCK_GET_TIME_MKS_100         500 //1
#define SYSCLOCK_GET_TIME_MS_1            5000 //2
#define SYSCLOCK_GET_TIME_MS_10           50000 //3
#define SYSCLOCK_GET_TIME_MS_100          500000 //4
#define SYSCLOCK_GET_TIME_S_1             5000000 //5
#define SYSCLOCK_GET_TIME_S_10            50000000 //6
#define SYSCLOCK_GET_TIME_M_1             300000000 //7


void drvSysClock_setRunPeriod(unsigned long period);

void drvSysClock_run(void);

unsigned long drvSysClock_getTime(void);

unsigned long drvSysClock_getPastTime(unsigned long startTime/*, unsigned long retFormat*/);


#ifdef	__cplusplus
}
#endif

#endif  /*__DRV_SYS_CLK_H__*/
