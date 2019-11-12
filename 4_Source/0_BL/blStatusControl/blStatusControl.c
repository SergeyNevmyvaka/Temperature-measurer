
#include "blStatusControl.h"

#include "../../0_BL/blCheckReadingsCode/blCheckReadingsCode.h"
#include "../../1_Utl/utl_Calendar/utlCalendar.h"
#include "../../1_Utl/utlKey/utlKey.h"
#include "../../1_Utl/utlLPM/utlLPM.h"
#include "../../1_Utl/utlModBus/utlModBus.h"

#include <SI_EFM8SB2_Register_Enums.h>
#include "si_toolchain.h"


#define ADC_VREF    1.65

SI_SEGMENT_VARIABLE(SStatusShortArray [BL_STATUS_CONTROL_SHORT_NUMBER], static unsigned short, SI_SEG_XDATA);

unsigned long ADC_SUM;                      // Accumulates the ADC samples
unsigned char CONV_COMPLETE = 1;            // ADC accumulated result ready flag

unsigned long ADC_CountsAt_1_65V;
unsigned long ADC_CountsAt_VDD;

unsigned char HoldCounter;

void blStatusControl_SetConvCompleteFlag(unsigned char flag)
{
	CONV_COMPLETE = flag;
}
void blStatusControl_SetADC_ConvValue(unsigned long value)
{
	ADC_SUM = value;
}


void blStatusControl_SetStatus_short(unsigned char statusID, unsigned short status)
{
   	SStatusShortArray[statusID] = status;
}

unsigned short blStatusControl_GetStatus_short(unsigned char statusID)
{
	return SStatusShortArray[statusID];
}


void blStatusControl_Init(S_DEVICE *device)
{
	// инициализаци€ значений статусов

	//  \\        ******************         START CONDITION               *******************

	blStatusControl_SetStatus_short(BL_SC_SHORT_STATUS_VER_SOFT        , BL_SC_SHORT_STATUS_SOFTWARE_VERSION);  //     0x0105       //3 данные

	blStatusControl_SetStatus_short(BL_SC_SHORT_STATUS_DEVICE_ERROR    , 0x0000);  //     0x0003       //4 данные

	blStatusControl_SetStatus_short(BL_SC_SHORT_STATUS_START_READINGS_H, 0x0000);  //     0x000B       //12 данные
	blStatusControl_SetStatus_short(BL_SC_SHORT_STATUS_START_READINGS_L, 0x0000);  //     0x000C       //13 данные

	blStatusControl_SetStatus_short(BL_SC_SHORT_STATUS_MEASURE_TEMP    , 0x0000);  //     0x0015       //22 управление

	blStatusControl_SetStatus_short(BL_SC_SHORT_STATUS_TEMPR_MODBUS    , 0x0000);  //     0x001C       //27 управление

	blStatusControl_SetStatus_short(BL_SC_SHORT_STATUS_APPLY, RESET);  //


    device->SWITCH_INDICATION = RESET;
    device->INDICATION_MODE = RESET;
    device->SWITCH_MODBUS = SET;
    device->SWITCH_SLEEP = SET;

    device->TEMPERATURE_WITH_K = 25;

    device->CURRENT_MINUTE = 0;
    device->CURRENT_HOUR   = 0;
    device->CURRENT_DAY    = 1;
    device->CURRENT_MONTH  = 2;
    device->CURRENT_YEAR   = 16;

    device->SWITCH_CNTRL_BATTERY = SET;

    device->UPDATE_LCD = RESET;
    device->PARAMS_NEED_TO_SAVE = 0x10;//RESET;

    device->ERROR = 0;

    device->K1 = BL_TEMP_WITH_K_K1_DEFAULT_VALUE;

    utlKey_Init();


	utlLPM_InitSleep_work();

	utlModBus_Init();


}

void blStatusControl_ShiftParamsToModbus(S_DEVICE *device)
{
	unsigned long Buff;

	blStatusControl_SetStatus_short(BL_SC_SHORT_STATUS_DEVICE_SN_H     , device->DEVICE_ID >> 16);  //     0x0000       //1 данные
	blStatusControl_SetStatus_short(BL_SC_SHORT_STATUS_DEVICE_SN_L     , device->DEVICE_ID);        //     0x0001       //2 данные
	blStatusControl_SetStatus_short(BL_SC_SHORT_STATUS_DEVICE_ERROR    , device->ERROR);            //     0x0003       //4 данные

	Buff = (unsigned long)device->COUNTING_RATE;
	blStatusControl_SetStatus_short(BL_SC_SHORT_STATUS_READINGS_H      , Buff >> 16);               //     0x0008       //9 данные
	blStatusControl_SetStatus_short(BL_SC_SHORT_STATUS_READINGS_L      , Buff);                     //     0x0009       //10 данные
	blStatusControl_SetStatus_short(BL_SC_SHORT_STATUS_START_READINGS_H, Buff >> 16);               //     0x0008       //9 данные
	blStatusControl_SetStatus_short(BL_SC_SHORT_STATUS_START_READINGS_L, Buff);                     //     0x0009       //10 данные

	blStatusControl_SetStatus_short(BL_SC_SHORT_STATUS_MODE            , device->DEVICE_MODE);      //     0x000A       //11 данные

	Buff = *((unsigned long*) &device->K3);
	blStatusControl_SetStatus_short(BL_SC_SHORT_STATUS_K3_H            , Buff >> 16);               //     0x0017       //23 данные     // 0x16 - пропускаем
	blStatusControl_SetStatus_short(BL_SC_SHORT_STATUS_K3_L            , Buff);                     //     0x0018       //24 данные
	Buff = *((unsigned long*) &device->K4);
	blStatusControl_SetStatus_short(BL_SC_SHORT_STATUS_K4_H            , Buff >> 16);               //     0x0019       //25 данные
	blStatusControl_SetStatus_short(BL_SC_SHORT_STATUS_K4_L            , Buff);                     //     0x001A       //26 данные

	blStatusControl_SetStatus_short(BL_SC_SHORT_STATUS_CONTROL_TIME_DAY  , device->CONTROL_DAY);    //
	blStatusControl_SetStatus_short(BL_SC_SHORT_STATUS_CONTROL_TIME_MONTH, device->CONTROL_MONTH);  //

	blStatusControl_SetStatus_short(BL_SC_SHORT_STATUS_CURRENT_TIME_MIN  , device->CURRENT_MINUTE); // передаем врем€ в регистры MODBUS
	blStatusControl_SetStatus_short(BL_SC_SHORT_STATUS_CURRENT_TIME_HOUR , device->CURRENT_HOUR);
	blStatusControl_SetStatus_short(BL_SC_SHORT_STATUS_CURRENT_TIME_DAY  , device->CURRENT_DAY);
	blStatusControl_SetStatus_short(BL_SC_SHORT_STATUS_CURRENT_TIME_MONTH, device->CURRENT_MONTH);
	blStatusControl_SetStatus_short(BL_SC_SHORT_STATUS_CURRENT_TIME_YEAR , device->CURRENT_YEAR);
}

void blStatusControl_Handler(S_DEVICE *device)
{
	unsigned long Buff;

	unsigned char modeKeyState;// = 0;
	unsigned char breakKeyState;// = 0;

///////////////////////////////////////  “екущее врем€  //////////////////////////////////////
	utlCalendar_AddSecond(device);
//////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////   онтрольные даты  ///////////////////////////////////
	if ((device->SWITCH_WRITE_ARCHIVE == SET))// || blStatusControl_GetStatus_short(BL_SC_SHORT_STATUS_DELETE_ARCHIVE == 0x52))
	{
		// если контрольна€ дата (сохр и обнуление)
		if ((device->CURRENT_DAY == device->CONTROL_DAY) && (device->CURRENT_MONTH == device->CONTROL_MONTH))
		{
			if (device->CURRENT_HOUR == 23 && device->CURRENT_MINUTE > 55)
			{
				device->PARAMS_NEED_TO_SAVE = BL_STATUS_SAVE_PARAMS_CONTROL;
			}
		}//end calculation date

		// если начало нового мес€ца 00:00, сохранить показани€ за прошедший
		if (device->CURRENT_DAY == 1)
		{
			if ((device->CURRENT_HOUR == 0) && (device->CURRENT_MINUTE < 4))
			{
				device->PARAMS_NEED_TO_SAVE = BL_STATUS_SAVE_PARAMS_MONTH;
			}
		}//end calculation date

		device->SWITCH_WRITE_ARCHIVE = RESET;
	}

	if (blStatusControl_GetStatus_short(BL_SC_SHORT_STATUS_CONTROL_R_OLD))
		device->PARAMS_NEED_TO_SAVE = BL_STATUS_SAVE_PARAMS_CONTROL;
//////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////// ”становка параметров конфигурации //////////////////////////////
	if (SStatusShortArray[BL_SC_SHORT_STATUS_APPLY])
	{
		blStatusControl_SetStatus_short(BL_SC_SHORT_STATUS_APPLY, RESET);

		device->DEVICE_ID  = (unsigned long)blStatusControl_GetStatus_short(BL_SC_SHORT_STATUS_DEVICE_SN_H) << 16;  //
		device->DEVICE_ID |= (unsigned long)blStatusControl_GetStatus_short(BL_SC_SHORT_STATUS_DEVICE_SN_L);  //

		device->ERROR = blStatusControl_GetStatus_short(BL_SC_SHORT_STATUS_DEVICE_ERROR);

		device->DEVICE_MODE = blStatusControl_GetStatus_short(BL_SC_SHORT_STATUS_MODE);  //

		Buff  = (unsigned long)blStatusControl_GetStatus_short(BL_SC_SHORT_STATUS_START_READINGS_H) << 16;  //
		Buff |= (unsigned long)blStatusControl_GetStatus_short(BL_SC_SHORT_STATUS_START_READINGS_L);  //
		device->COUNTING_RATE = (float)Buff;

		device->CONTROL_DAY   = blStatusControl_GetStatus_short(BL_SC_SHORT_STATUS_CONTROL_TIME_DAY);  //
		device->CONTROL_MONTH = blStatusControl_GetStatus_short(BL_SC_SHORT_STATUS_CONTROL_TIME_MONTH);  //
		//device->CONTROL_YEAR  = blStatusControl_GetStatus_short(BL_SC_SHORT_STATUS_CCONTROL_TIME_YEAR);  //

		Buff  = (unsigned long)blStatusControl_GetStatus_short(BL_SC_SHORT_STATUS_K3_H) << 16;  //
		Buff |= (unsigned long)blStatusControl_GetStatus_short(BL_SC_SHORT_STATUS_K3_L);  //
		device->K3 = *((float*) &Buff);

		Buff  = (unsigned long)blStatusControl_GetStatus_short(BL_SC_SHORT_STATUS_K4_H) << 16;  //
		Buff |= (unsigned long)blStatusControl_GetStatus_short(BL_SC_SHORT_STATUS_K4_L);  //
		device->K4 = *((float*) &Buff);

		device->CURRENT_MINUTE = blStatusControl_GetStatus_short(BL_SC_SHORT_STATUS_CURRENT_TIME_MIN); 	// берем врем€ из регистров MODBUS
		device->CURRENT_HOUR   = blStatusControl_GetStatus_short(BL_SC_SHORT_STATUS_CURRENT_TIME_HOUR);
		device->CURRENT_DAY    = blStatusControl_GetStatus_short(BL_SC_SHORT_STATUS_CURRENT_TIME_DAY);
		device->CURRENT_MONTH  = blStatusControl_GetStatus_short(BL_SC_SHORT_STATUS_CURRENT_TIME_MONTH);
		device->CURRENT_YEAR   = blStatusControl_GetStatus_short(BL_SC_SHORT_STATUS_CURRENT_TIME_YEAR);

		device->PARAMS_NEED_TO_SAVE = BL_STATUS_SAVE_PARAMS_DEVICE;
	}
//////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////   нопка меню и тампер  ///////////////////////////////////

	utlKey_KeyDebounceHandler();

	breakKeyState = utlKey_Handler(UTLKEY_BREAK_KEY);
	modeKeyState = utlKey_Handler(UTLKEY_MODE_KEY);

	if (breakKeyState == UTLKEY_STATE_HOLDED_TIME_1)
	{
		device->BREAK_SENSOR_STATE = SET;
	}
	else if (breakKeyState == UTLKEY_STATE_NOT_PUSHED)
	{
		device->BREAK_SENSOR_STATE = RESET;
	}

	if (modeKeyState == UTLKEY_STATE_PUSHED)
	{
		device->MENU_BUTTON_STATE = SET;
	}
	else if (modeKeyState == UTLKEY_STATE_HOLDED_TIME_1)
	{
		device->MENU_BUTTON_STATE = BL_STATUS_BUTTON_PRESSED_LONG;
	}


//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**
//**//**//**//**//**//**//**//**//**    BATTERY CONTROL     //**//**//**//**//**//**//**//**//**
//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**
	if (device->SWITCH_CNTRL_BATTERY)
	{
//----------------------0
		// init ADC
		ADC0MX = ADC0MX_ADC0MX__TEMP;
		ADC0AC = ADC0AC_ADRPT__ACC_1 | ADC0AC_ADAE__ACC_DISABLED
				| ADC0AC_ADSJST__LEFT_NO_SHIFT;
		ADC0CF = ADC0CF_AD8BE__NORMAL | ADC0CF_ADGN__GAIN_1
				| (0x00 << ADC0CF_ADSC__SHIFT) | ADC0CF_ADTM__TRACK_NORMAL;
		ADC0GTH = 0x00;
		ADC0GTL = 0x00;
		ADC0CN0 |= ADC0CN0_ADEN__ENABLED;
		REF0CN = REF0CN_TEMPE__TEMP_ENABLED | REF0CN_REFSL__HIGH_SPEED_VREF;
		EIE1 |= EIE1_EADC0__ENABLED;
		ADC0CN0 |= ADC0CN0_ADBUSY__SET;    //start  conversion
//----------------------1
		while(CONV_COMPLETE);    //wait for conversion

		ADC_CountsAt_1_65V = ADC_SUM;
		CONV_COMPLETE = SET;
		ADC0CN0 |= ADC0CN0_ADBUSY__SET;    //start  conversion
//----------------------2
		while(CONV_COMPLETE);    //wait for conversion

		ADC_CountsAt_1_65V = ADC_SUM;
		CONV_COMPLETE = SET;
//----------------------3
		ADC0CN0 &= ~ADC0CN0_ADEN__ENABLED;
		REF0CN = REF0CN_TEMPE__TEMP_ENABLED | REF0CN_REFSL__VDD_PIN;
		ADC0CN0 |= ADC0CN0_ADEN__ENABLED;
		CONV_COMPLETE = SET;
		ADC0CN0 |= ADC0CN0_ADBUSY__SET;    //start  conversion
//----------------------4
		while(CONV_COMPLETE);    //wait for conversion

		ADC_CountsAt_VDD = ADC_SUM;
		CONV_COMPLETE = SET;
		ADC0CN0 |= ADC0CN0_ADBUSY__SET;    //start  conversion
//----------------------5
		while(CONV_COMPLETE);    //wait for conversion

		ADC_CountsAt_VDD = ADC_SUM;
		ADC0CN0 &= ~ADC0CN0_ADEN__ENABLED;
		REF0CN &= ~REF0CN_TEMPE__TEMP_ENABLED;
		CONV_COMPLETE = SET;
		device->BATTERY_VDD = (ADC_VREF * ADC_CountsAt_1_65V) / ADC_CountsAt_VDD;
		// Set MODBUS reg for stand
		blStatusControl_SetStatus_short(BL_SC_SHORT_STATUS_BATTERY_VOLTAGE,  (unsigned short)(device->BATTERY_VDD * 100));

		if (device->BATTERY_VDD < BL_SC_STATUS_BATTERY_DISCHARGE_VOLTAGE)// установить ошибку батарейки, если она разр€жена
		{
			device->ERROR |= BL_STATUS_ERROR_BATTERY_DISCHARGED;
		}
		else
		{
			device->ERROR &= ~BL_STATUS_ERROR_BATTERY_DISCHARGED;
		}
//----------------------end
		device->SWITCH_CNTRL_BATTERY = RESET;
		device->UPDATE_LCD = SET;
	}
//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**

//////////////////////////////   –асчет кода дл€ пользовател€   ////////////////////////////////
	blCheckReadingsCode_Handler(device);
////////////////////////////////////////////////////////////////////////////////////////////////
}


void blPower_Handler(S_DEVICE *device)
{
	if (device->SWITCH_SLEEP && !device->SWITCH_INDICATION && !device->PARAMS_NEED_TO_SAVE)
	{
        utlLPM_FallAsleep();

        device->WAKE_UP_EVENT = utlLPM_WakingUp();
	}
}
