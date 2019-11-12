#include "blCountingRate.h"
#include "math.h"
#include "../../0_BL/blStatusControl/blStatusControl.h"

/*
 * К3  в пределах от  0,001 до 20,000 (формат  хх,ххх)
 * K4  в пределах от 1,00  до 1,50 (формат х,хх)
 *
 *
 */

void blCountingRate_Handler(S_DEVICE *device)
{
    float resultOfCalculations;
    float Buff;
    float R;

    if (device->TEMPERATURE_READY)
    {
    	device->TEMPERATURE_READY = RESET;

        device->COUNTING_START_TEMPERATURE = BL_CR_COUNTING_START_TEMPERATURE;

        if ((device->CURRENT_MONTH > 5) && (device->CURRENT_MONTH < 9))// from july to august
        {
        	device->COUNTING_START_TEMPERATURE = BL_CR_COUNTING_START_TEMPERATURE_SUMMER;
        }

        if (device->TEMPERATURE_WITH_K >= device->COUNTING_START_TEMPERATURE)
        {
			R = device->COUNTING_RATE;

			resultOfCalculations = device->TEMPERATURE_WITH_K - 20.0;                     // - dT
			resultOfCalculations = resultOfCalculations * 0.016667;	                      // / 60
			resultOfCalculations = pow(resultOfCalculations, device->K4);	              // ^ K4 (1.15)
			                Buff = pow(1.28, device->K4);                                 // 1.28 ^ K4
			resultOfCalculations = resultOfCalculations * 0.066 * Buff;                   // time and tempflow corr
			resultOfCalculations = resultOfCalculations * device->K3;                     // --//-- new K3, K4

			device->COUNTING_RATE = R + resultOfCalculations;

            if (device->COUNTING_RATE > 99999.0)  // overflow border
		    {
            	device->COUNTING_RATE -= 99999.0;
		    }
        }
    }
}





