#include "blArchive.h"
#include "../../0_BL/blStatusControl/blStatusControl.h"
#include "../../1_Utl/utlArchive/utlArchive.h"
#include "../../1_Utl/utlBCD/utlBCD.h"
#include "SI_EFM8SB2_Register_Enums.h"
#include "si_toolchain.h"



void blArchive_Handler(S_DEVICE *device)
{
	unsigned char monthPlace_R;
	unsigned char monthPlace_Date;
	unsigned char monthPlace_Status;
	unsigned long tempBuff;

	unsigned char archMonth;
	unsigned char archYear;
	unsigned long CurrentMonth;
	unsigned long ArchCounter;


	if (device->PARAMS_NEED_TO_SAVE)
	{
		utlArchive_ReadArchiveArray();

		switch (device->PARAMS_NEED_TO_SAVE)
		{
			case BL_STATUS_SAVE_PARAMS_BREAK_EVENT :
				utlArchive_WriteDataInArray(device->DEVICE_MODE, ARCH_DATA_CURRENT_MODE);

			case BL_STATUS_SAVE_PARAMS_MONTH :
//----------// сохранение показаний за месяц
				CurrentMonth = utlArchive_ReadDataFromArray(ARCH_DATA_CURRENT_MONTH);

				if (CurrentMonth >= ARCH_MONTH_STORAGE_SIZE) ////60
				{
					CurrentMonth = 0;
				}
				else
				{
					CurrentMonth += 12;
				}

				monthPlace_R      = CurrentMonth;     //+0 //MonthPlacesArray[ARCH_MONTH_PLACES_SIZE]
				monthPlace_Date   = CurrentMonth + 4; //+4
				monthPlace_Status = CurrentMonth + 8; //+8

				utlArchive_WriteDataInArray((unsigned long)device->COUNTING_RATE, monthPlace_R);

				archYear = device->CURRENT_YEAR;
				archMonth = device->CURRENT_MONTH - 1;

				if (archMonth == 0) // т.к. команда сохранения выдается в первый день текущего месяца, а нужно сохранить данные за прошедший
				{
					archMonth = 12;
					archYear = archYear - 1;
				}

				if (device->PARAMS_NEED_TO_SAVE == BL_STATUS_SAVE_PARAMS_BREAK_EVENT)
				{
					archYear = device->CURRENT_YEAR;
					archMonth = device->CURRENT_MONTH;
				}

				tempBuff  = (unsigned long)archMonth << 8;
				tempBuff |= archYear;

				utlArchive_WriteDataInArray(tempBuff, monthPlace_Date);

				utlArchive_WriteDataInArray(device->ERROR, monthPlace_Status);

				utlArchive_WriteDataInArray(CurrentMonth, ARCH_DATA_CURRENT_MONTH);
				break;
			case BL_STATUS_SAVE_PARAMS_CONTROL :
//----------// сохранение показаний на контрольную дату

				utlArchive_WriteDataInArray((unsigned long)device->COUNTING_RATE, ARCH_DATA_CONTROL_R);

				device->CONTROL_COUNTING_RATE = device->COUNTING_RATE;
				device->COUNTING_RATE = 0;
				blStatusControl_SetStatus_short(BL_SC_SHORT_STATUS_CONTROL_R_OLD, RESET);

				tempBuff  = (unsigned long)device->CONTROL_DAY << 8;///CURRENT_YEAR << 8;
				tempBuff |= device->CONTROL_MONTH;///CURRENT_MONTH;
				utlArchive_WriteDataInArray(tempBuff, ARCH_DATA_CONTROL_DATE);
				break;
//			case BL_STATUS_SAVE_PARAMS_BREAK_EVENT :
////----------// сохранение показаний при переходе в ERROR
//				//utlArchive_WriteDataInArray((unsigned long)device->COUNTING_RATE, ARCH_DATA_BREAK_R);
//
//				tempBuff  = device->CURRENT_DAY;
//				tempBuff |= device->CURRENT_MONTH << 8;
//				tempBuff |= device->CURRENT_YEAR << 16;
//
//				utlArchive_WriteDataInArray(tempBuff, ARCH_DATA_BREAK_DATE);
//
//				utlArchive_WriteDataInArray(device->ERROR, ARCH_DATA_BREAK_STATUS);
//
//				device->PARAMS_NEED_TO_SAVE &= ~BL_STATUS_SAVE_PARAMS_BREAK_EVENT;
//				break;
			case BL_STATUS_SAVE_PARAMS_DEVICE :
//----------//сохраниение остальных параметров ВА

					utlArchive_WriteDataInArray(device->DEVICE_ID, ARCH_DATA_SN);

					utlArchive_WriteDataInArray(device->DEVICE_MODE, ARCH_DATA_CURRENT_MODE);

					utlArchive_WriteDataInArray((unsigned long)device->COUNTING_RATE, ARCH_DATA_BREAK_R);

					utlArchive_WriteDataInArray(device->CONTROL_DATE, ARCH_DATA_CONTROL_DATE);
					//tempBuff = *((unsigned long*) &device->K3);
					utlArchive_WriteDataInArray(*((unsigned long*) &device->K3), ARCH_DATA_K3);
					//tempBuff = *((unsigned long*) &device->K4);
					utlArchive_WriteDataInArray(*((unsigned long*) &device->K4), ARCH_DATA_K4);
				break;
			default :
				break;
		}
//------------------------------------------------------------------------------------------------------------
		// обнуление архивов только один раз
		ArchCounter = utlArchive_ReadDataFromArray(ARCH_DATA_DELETE_ARCHIVE_COUNTER);
		if (ArchCounter >= ARCH_MONTH_DELETE_ARCH_VALUE)
		{
			device->PARAMS_NEED_TO_SAVE = BL_STATUS_SAVE_PARAMS_MONTH;
			ArchCounter--;
			utlArchive_WriteDataInArray(ArchCounter, ARCH_DATA_DELETE_ARCHIVE_COUNTER);

			blStatusControl_SetStatus_short(BL_SC_SHORT_STATUS_CHECK_TAMPER, device->BREAK_SENSOR_STATE);
			utlArchive_WriteDataInArray(device->BREAK_SENSOR_STATE, ARCH_DATA_TAMPER_STATE);
		}
		else
		{
			device->PARAMS_NEED_TO_SAVE = RESET;
		}

		utlArchive_WriteArchiveArray();
	}
}


void blArchive_RestoreData(S_DEVICE *device)
{
	unsigned long tempBuff;

	utlArchive_ReadArchiveArray();

//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--
	device->DEVICE_ID = utlArchive_ReadDataFromArray(ARCH_DATA_SN);
	if (device->DEVICE_ID == 0xFFFFFFFF)
	{
		device->DEVICE_ID = 0;
	}
//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--
	device->DEVICE_MODE = utlArchive_ReadDataFromArray(ARCH_DATA_CURRENT_MODE);
	if (device->DEVICE_MODE > BL_STATUS_MODE_ERROR)
	{
		device->DEVICE_MODE = BL_STATUS_MODE_SLEEP;
	}
//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--
	tempBuff = utlArchive_ReadDataFromArray(ARCH_DATA_BREAK_R);
	if (tempBuff == 0xFFFFFFFF)
	{
		tempBuff = 0;
	}
	device->COUNTING_RATE = tempBuff;

//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--
	tempBuff = utlArchive_ReadDataFromArray(ARCH_DATA_CONTROL_R);
	if (tempBuff == 0xFFFFFFFF)
	{
		tempBuff = 0;
	}
	device->CONTROL_COUNTING_RATE = tempBuff;

//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--
	tempBuff = utlArchive_ReadDataFromArray(ARCH_DATA_CONTROL_DATE);
	device->CONTROL_DAY   = tempBuff >> 8;
	device->CONTROL_MONTH = tempBuff;

	if ((device->CONTROL_DAY > 31) || (device->CONTROL_MONTH > 12))
	{
		device->CONTROL_DAY   = 31;
		device->CONTROL_MONTH = 12;
	}
//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--
	tempBuff = utlArchive_ReadDataFromArray(ARCH_DATA_K3);
	if (tempBuff == 0xFFFFFFFF)
	{
		tempBuff = BL_TEMP_WITH_K_K3_DEFAULT_VALUE;
	}
	device->K3 = *((float*) &tempBuff);
//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--
	tempBuff = utlArchive_ReadDataFromArray(ARCH_DATA_K4);
	if (tempBuff == 0xFFFFFFFF)
	{
		tempBuff = BL_TEMP_WITH_K_K4_DEFAULT_VALUE;
	}
	device->K4 = *((float*) &tempBuff);

	blStatusControl_ShiftParamsToModbus(device);
//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--
	blStatusControl_SetStatus_short(BL_SC_SHORT_STATUS_CHECK_TAMPER, utlArchive_ReadDataFromArray(ARCH_DATA_TAMPER_STATE));
//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--

	//-------------------------------- Check Tamper ---------------------------------------------
	//-------------------------------------------------------------------------------------------
		if (P0_B3 == SET)
		{
			device->ERROR |= BL_STATUS_ERROR_TAMPER;
		}
	//--------------------------------------------------------------------------------------------

}



