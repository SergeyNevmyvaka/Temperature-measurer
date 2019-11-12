/*
 * drvLPM.c
 */	

#include "drvLPM.h"
#include "../../3_HAL/halPower_smart/power.h"
#include "../../3_HAL/halPower_smart/SmaRTClock.h"
#include "../../3_HAL/halOSC/halOSC.h"
#include "../../3_HAL/halComparator/halComparator.h"
#include "SI_EFM8SB2_Register_Enums.h"

void drvLPM_InitPeriodicalSleep_4min(void)
{
    // init RTC 
	PMU0CF_Local = 0;
	RTC0CN0_Local = 0xC0;                // Initialize Local Copy of RTC0CN0

	halRTC_InitSleepRTC();

	RTC0CN0_SetBits(RTC_SLEEP_BITS);// Enable Counter, Alarm, and Auto-Reset

	LPM_Init();                         // Initialize Power Management

	LPM_Enable_Wakeup( RTC | PORT_MATCH ); // | COMPARATOR
	RTC_Alarm = 1;                      // Set the RTC Alarm Flag on startup

}


void drvLPM_GoToSLEEP(void)
{
	halOSC_InitLPO();
    LPM(SLEEP);                      // Enter Sleep Until Next Alarm
    halOSC_InitHFO(CLKSEL_CLKDIV__SYSCLK_DIV_8);
}


unsigned char drvLPM_WakeSource(void)
{
    unsigned char returnValue = DRVPWR_WAKE_EVENT_PORT_MATCH;

    if (RTC_Alarm == 1)
    {
        returnValue = DRVPWR_WAKE_EVENT_RTC_ALARM;
        RTC_Alarm = 0;
    
    }
    else if (Port_Match_Wakeup == 1)
    {
        returnValue = DRVPWR_WAKE_EVENT_PORT_MATCH;
        Port_Match_Wakeup = 0;
    
    }
    else if (RTC_Failure == 1)
    {
        returnValue = DRVPWR_WAKE_EVENT_RTC_OSC_FAIL;
        RTC_Failure = 0;
    }
    
    return returnValue;
}

