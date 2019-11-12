/*
 * utlLPM.c
 */	

#include "utlLPM.h"
#include "../../2_Drv/drvLPM/drvLPM.h"


void utlLPM_InitSleep_work(void)
{
	drvLPM_InitPeriodicalSleep_4min();
}


void utlLPM_FallAsleep(void)
{
  drvLPM_GoToSLEEP();
}


unsigned char utlLPM_WakingUp(void)
{
    return drvLPM_WakeSource();
}

