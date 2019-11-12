#include "utlTimeToResistance.h"
#include "../../2_Drv/drvThermoSensor/drvThermoSensor.h"
#include "SI_EFM8SB2_Register_Enums.h"



 
unsigned long utlTimeToResistance_GetResistance (void)
{
    unsigned long returnValue;
    unsigned long RefCapacitorTime;
    unsigned long SensorCapacitorTime;
    float R;

    drvTermoSensor_MeasureModeOn();

    RefCapacitorTime = drvTermoSensor_GetDischargeTime(SENSOR_REF_CAP_PIN);

   	SensorCapacitorTime = drvTermoSensor_GetDischargeTime(SENSOR_SENS_PIN);

    if (RefCapacitorTime > 0)
    {
        R = (10000.0 * (float)SensorCapacitorTime) / (float)RefCapacitorTime;
    }
    else
    {
        R = 0;
    }

    returnValue = (unsigned long)(R + 0.5);

	drvTermoSensor_MeasureModeOff();

    return returnValue;
}






