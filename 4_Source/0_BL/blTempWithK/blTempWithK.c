
#include "blTempWithK.h"

#include "../../0_BL/blStatusControl/blStatusControl.h"
#include "../../1_Utl/utlAverageTemp/utlAverageTemp.h"
#include "../../1_Utl/utlTimeToResistance/utlTimeToResistance.h"
#include "../../1_Utl/utlResistanceToTemp/utlResistanceToTemp.h"

#include "../../1_Utl/utlBCD/utlBCD.h"


void blTempWithK_Handler(S_DEVICE *device)
{
    unsigned char i;
	unsigned long StorageTemp;
    unsigned long Resistance;
    float Temperature;// = 0
    float AverTemperature[AVERAGE_BUFFER_SIZE] = {0, 0, 0, 0};//, 0};


    if (device->SWITCH_MEASUREMENT == SET)
    {
    	device->SWITCH_MEASUREMENT = RESET;

    	for (i = 0; i < AVERAGE_BUFFER_SIZE; i++)
    	{
    		Resistance = utlTimeToResistance_GetResistance();
    		AverTemperature[i] = utlResistanceToTemp_Convert(Resistance);
    	}

    	Temperature = utlAverageTemp_Calculation(AverTemperature);

    	if (Temperature < 0)//  -77.0 - break or Short-circuit on sensor
		{
    		device->ERROR |= BL_STATUS_ERROR_TERMOSENSOR;
			device->BREAK_SENSOR_STATE = RESET;
		}
		else
			device->ERROR &= ~BL_STATUS_ERROR_TERMOSENSOR;  // необходимость восстановления ошибки под вопросом

    	////resultOfCalculations = blTempWithK_Calculate (Temperature, device);
    	//resultOfCalculations = Temperature * device->K1 + device->K2;


    	//device->TEMPERATURE_WITH_K = resultOfCalculations;
		device->TEMPERATURE_WITH_K = Temperature * device->K1;// + device->K2;

        StorageTemp = (unsigned long)(device->TEMPERATURE_WITH_K * 100);// + 0.005);

        blStatusControl_SetStatus_short(BL_SC_SHORT_STATUS_TEMPR, (unsigned short)StorageTemp);

        device->UPDATE_LCD = SET;

        if (device->DEVICE_MODE == BL_STATUS_MODE_WORK)
        {
        	device->TEMPERATURE_READY = SET;
        }
        else
        {
        	blStatusControl_SetStatus_short(BL_SC_SHORT_STATUS_TEMPR_MODBUS,  (unsigned short)StorageTemp);
        }
    }
}




