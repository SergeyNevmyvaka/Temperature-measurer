#include "blModeControl.h"
#include "../../0_BL/blStatusControl/blStatusControl.h"
#include "../../0_BL/blVisualizer/blVisualizer.h"
#include "../../1_Utl/utlLPM/utlLPM.h"
#include "../../1_Utl/utlKey/utlKey.h"


void blModeControl_ChangeModeHandler(S_DEVICE *device)
{
    switch(device->DEVICE_MODE)
    {
        case BL_MODE_CONTROL_MODE_SLEEP:

        	if (device->BREAK_SENSOR_STATE == SET)
        	{
            	device->ERROR = RESET;
            	device->UPDATE_LCD = SET;
            	device->DEVICE_MODE = BL_MODE_CONTROL_MODE_WORK;
            	utlKey_StartKeyModeWORK();
            }
            break;
        case BL_MODE_CONTROL_MODE_WORK:
            if (device->BREAK_SENSOR_STATE == RESET)
            {
            	device->DEVICE_MODE = BL_MODE_CONTROL_MODE_ERROR;
            	utlKey_StartKeyModeSLEEP();

            	device->MENU_BUTTON_STATE = SET;
            	device->PARAMS_NEED_TO_SAVE = BL_STATUS_SAVE_PARAMS_BREAK_EVENT;
            	if (!(device->ERROR & BL_STATUS_ERROR_TERMOSENSOR))
            		device->ERROR |= BL_STATUS_ERROR_TAMPER_BREAK_EVENT;
            }
            break;
        case BL_MODE_CONTROL_MODE_ERROR:

            break;
        default:
        	device->DEVICE_MODE = BL_MODE_CONTROL_MODE_SLEEP;
            break;

    }

}


void blModeControl_ModeHandler(S_DEVICE *device)
{

	blModeControl_ChangeModeHandler(device);


    switch (device->WAKE_UP_EVENT)
	{
		case BL_STATUS_WAKE_EVENT_RTC:
			if (device->OCCUR_RTC_ALARM == SET)
			{
				device->ADD_TIME = SET; // считаем время
				device->SWITCH_CNTRL_BATTERY = SET; // считаем батарейку

				if (device->DEVICE_MODE == BL_MODE_CONTROL_MODE_WORK)
				{
					device->SWITCH_MEASUREMENT = SET; // измеряем температуру
					device->SWITCH_CNTRL_DATE = SET; // считаем проверяем даты

					device->SWITCH_WRITE_ARCHIVE = SET;
				}

				device->OCCUR_RTC_ALARM = RESET;
				device->SWITCH_SLEEP = SET;
				device->WAKE_UP_EVENT = RESET;
			}
			break;
		case BL_STATUS_WAKE_EVENT_PORT_MATCH:
			if (device->MENU_BUTTON_STATE == SET)
			{
				device->SWITCH_INDICATION = SET;
				device->WAKE_UP_EVENT = RESET;
			}
			device->SWITCH_SLEEP = RESET;
			break;
		default:
			break;
	}
}


