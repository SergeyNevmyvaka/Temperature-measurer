/*
 * drvTermoSensor.c
 */	

#include "drvThermoSensor.h"
#include "../../3_HAL/halGPIO/halGPIO.h"
#include "../../3_HAL/halComparator/halComparator.h"
#include "../../3_HAL/halTimer/halTimer.h"
#include "../../3_HAL/halOSC/halOSC.h"
#include "SI_EFM8SB2_Register_Enums.h"


static unsigned long DischargeTime;

#define SENSOR_DELAYS_CORRECTION       20

unsigned long drvTermoSensor_GetDischargeTime (unsigned char DischargePinID)
{

    unsigned short numberOverflows;// = 0;

    DischargeTime = 0;
    // P_0_2
    halGPIO_SensPinUnused();
    // P_0_0
//    halGPIO_RefPinUnused();
    // P_0_1
    halGPIO_CompPinUnused();

///////////////////////////////////////////////////////////////////////////////
    halTimer3_InitAlarm();

    halGPIO_RefPinCharge();

    halTimer3_Start();

    while(halTimer3_GetAlarmFlag());

    halTimer3_ResetAlarmFlag();

    halGPIO_RefPinUnused();

    halTimer3_Deinit();

    halTimer3_Stop();
////////////////////////////////////////////////////////////////////////////////

    halGPIO_CompPinAnalog();

    halComparator1_Init();

    halTimer0_InitOverflowCount();

    halTimer3_InitCapture();

// Critical code section BEGIN

    TMR3CN0 |= TMR3CN0_TR3__RUN;

    if (DischargePinID == SENSOR_REF_CAP_PIN)
    {
    	P0MDOUT |= P0MDOUT_B0__PUSH_PULL;
		P0_B0 = 0;
    }
    else
    {
		P0MDOUT |= P0MDOUT_B2__PUSH_PULL;
		P0_B2 = 0;
    }

    halTimer0_Start(); //TCON |= TCON_TR0__RUN;

    while(halTimer3_GetCompareFlag());

    halTimer0_Stop();
    halTimer3_Stop();

// Critical code section END

    numberOverflows = halTimer3_GetAlarmCount();

    if (numberOverflows > 0)
    {
    	DischargeTime = numberOverflows * 65535;
        DischargeTime += halTimer3_GetCaptureValue();
    }
    else
    {
        DischargeTime = halTimer3_GetCaptureValue();
    }

    halTimer3_ResetAlarmFlag();
////////////////////////////////////////////////////////////////////////////////

    if (DischargePinID == SENSOR_REF_CAP_PIN)
    {
    	halGPIO_RefPinUnused();
    }
    else
    {
    	halGPIO_SensPinUnused();
    }
    
    halComparator1_Deinit();

    halGPIO_CompPinUnused();

    halTimer3_Deinit ();
////////////////////////////////////////////////////////////////////////////////    

    return (DischargeTime - SENSOR_DELAYS_CORRECTION);
}


void drvTermoSensor_MeasureModeOn(void)
{
    volatile unsigned char i;// = 0;

    halGPIO_DisableWeakUp();

    halOSC_InitHFO(CLKSEL_CLKDIV__SYSCLK_DIV_1);

    for (i = 0; i < 20; i++);
}


void drvTermoSensor_MeasureModeOff(void)
{
    halGPIO_EnableWeakUp();

    halOSC_InitHFO(CLKSEL_CLKDIV__SYSCLK_DIV_8);
    //halOSC_InitLPO();
}


