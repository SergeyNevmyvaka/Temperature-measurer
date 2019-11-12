/*
 * utlSysClock.cpp
 *
 *  Created on: 20.11.2012
 *      Author: a.tkachenko
 */
#include "drvSysClock.h"

#define SYSCLOCK_MAX_TIME           0xFFFFFFFF

static unsigned long currentTime;


void drvSysClock_run(void)
{
	currentTime++;
}


unsigned long drvSysClock_getTime(void)
{
    return currentTime;
}


unsigned long drvSysClock_getPastTime(unsigned long startTime)
{
    unsigned long pastTime;
    unsigned long retVal;

    if (startTime <= currentTime)
    {
        pastTime = currentTime - startTime;
    }
    else
    {
        pastTime = SYSCLOCK_MAX_TIME - startTime + currentTime + 1;
    }
    retVal = pastTime;

    return retVal;
}

