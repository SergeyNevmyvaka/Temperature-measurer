
#include "blVisualizer.h"

#include "../../0_BL/blStatusControl/blStatusControl.h"

#include "../../1_Utl/utlArchive/utlArchive.h"
#include "../../1_Utl/utlIndication/utlIndication.h"
#include "../../1_Utl/utlKey/utlKey.h"

#include "si_toolchain.h"


static unsigned char CurrentPageLevel_0;
static unsigned char CurrentPageLevel_1;
static unsigned char CurrentArchive;
static unsigned char UpdateLCDFlag;

#define V_NEXT_PAGE(x) x++;

#define V_LEVEL_INIT_VALUE                  0
#define V_ARCHIVE_INIT_VALUE                11//12-1 //5

#define V_LEVEL_0_PAGE_1_T       		1
#define V_LEVEL_0_PAGE_2_R_VALUE        2
#define V_LEVEL_0_PAGE_3_CURR_DATE      3
#define V_LEVEL_0_PAGE_4_CNTRL_R        4
#define V_LEVEL_0_PAGE_5_CNTRL_DATE     5
#define V_LEVEL_0_PAGE_6_CODE_H         6
#define V_LEVEL_0_PAGE_7_CODE_L         7
//#define V_LEVEL_0_PAGE_8_TEST           8


#define V_LEVEL_1_PAGE_0_ARCH               1
#define V_LEVEL_1_PAGE_1_ARCH_R             2
#define V_LEVEL_1_PAGE_2_ARCH_DATE          3
#define V_LEVEL_1_PAGE_3_ARCH_ERROR         4

void blVisualizer_ChangePageHandler(unsigned char buttonState);


void blVisualizer_Init(S_DEVICE *device)
{

    CurrentPageLevel_0 = V_LEVEL_INIT_VALUE;
    CurrentPageLevel_1 = V_LEVEL_INIT_VALUE;
    CurrentArchive     = V_ARCHIVE_INIT_VALUE;
    
    utlIndication_Init(device->BATTERY_VDD);
}

void blVisualizer_ChangePageHandler(unsigned char buttonState)
{

    switch(buttonState)
     {
        case BL_VISUALIZER_BUTTON_NOT_PRESSED :
            break;
        case BL_VISUALIZER_BUTTON_PRESSED_SHORT :
            // короткое нажатие кнопки
            UpdateLCDFlag = SET;
            utlIndication_StartTimeout();

        	if (((CurrentPageLevel_0 + 1) <= BL_VISUALIZER_LEVEL_0_ITEMS_NUMBER) && (CurrentPageLevel_1 == V_LEVEL_INIT_VALUE)) // шаг по 0му уровню
            {
                V_NEXT_PAGE(CurrentPageLevel_0)
            }
            else
            {
                CurrentPageLevel_0 = V_LEVEL_0_PAGE_1_T;
            }
            
            if (CurrentPageLevel_1 != V_LEVEL_INIT_VALUE) // шаг по 1му уровню
            {
                if ((CurrentPageLevel_1 + 1) <= BL_VISUALIZER_LEVEL_1_ITEMS_NUMBER)
                {
                    V_NEXT_PAGE(CurrentPageLevel_1)
                }
                else
                {
                    CurrentPageLevel_1 = V_LEVEL_1_PAGE_0_ARCH;
                }
            }
            break;
        case BL_VISUALIZER_BUTTON_PRESSED_LONG :
        	// длинное нажатие кнопки
        	UpdateLCDFlag = SET;
        	utlIndication_StartTimeout();

			if ((CurrentPageLevel_1 != V_LEVEL_INIT_VALUE) && (CurrentPageLevel_0 != V_LEVEL_INIT_VALUE)) // возврат на 0й уровень
			{
				CurrentPageLevel_1 = V_LEVEL_INIT_VALUE;
				CurrentPageLevel_0 = V_LEVEL_0_PAGE_1_T;
			}
			else if ((CurrentPageLevel_0 != V_LEVEL_INIT_VALUE) && (CurrentPageLevel_1 == V_LEVEL_INIT_VALUE)) // переход на 1й уровень
			{
				V_NEXT_PAGE(CurrentPageLevel_1)
				CurrentArchive     = V_ARCHIVE_INIT_VALUE;
			}
            break;
        default :
            break;
    }
}

void blVisualizer_Handler(S_DEVICE *device)
{
	unsigned char currentMode;
	unsigned short currentDate;
	unsigned long G;


	if (device->SWITCH_INDICATION)
	{
		if (device->INDICATION_MODE != SET)
		{
			utlKey_KeyHoldTimeACTIVE();

			utlArchive_ReadArchiveArray(); // считать архивные данные

			blVisualizer_Init(device);

			utlIndication_ShowAllLCD();
			//for (G = 0; G < 20000; G++); //5MHz
			for (G = 0; G < 3333; G++); //3MHz

			device->INDICATION_MODE = SET;
			device->SWITCH_MODBUS = RESET;
			device->SWITCH_CNTRL_BATTERY = SET;
		}

		if (device->UPDATE_LCD)
			UpdateLCDFlag = device->UPDATE_LCD;

		blVisualizer_ChangePageHandler(device->MENU_BUTTON_STATE);

		if (UpdateLCDFlag)
		{
			UpdateLCDFlag = RESET;
			device->MENU_BUTTON_STATE = RESET;
			device->UPDATE_LCD = RESET;

			device->UPDATE_TIME = SET;

			currentMode = device->DEVICE_MODE;

			switch(currentMode)
			{
				case BL_STATUS_MODE_SLEEP :
					utlIndication_ShowLabelSLEEP();
					break;
				case BL_STATUS_MODE_WORK :
				case BL_STATUS_MODE_ERROR :

					if (CurrentPageLevel_1)
					{
							switch(CurrentPageLevel_1)
							{
								case V_LEVEL_1_PAGE_0_ARCH :

									CurrentArchive++;

									if (CurrentArchive > V_ARCHIVE_INIT_VALUE)
									{
										CurrentArchive = 0;
									}
									utlIndication_ShowLabelArh(CurrentArchive + 1);
									break;
								case V_LEVEL_1_PAGE_1_ARCH_R :

									utlIndication_ShowR(utlArchive_ReadDataFromArray(CurrentArchive * 12));
									break;
								case V_LEVEL_1_PAGE_2_ARCH_DATE :

									utlIndication_ShowDate(utlArchive_ReadDataFromArray(CurrentArchive * 12 + 4));
									break;
								case V_LEVEL_1_PAGE_3_ARCH_ERROR :

									utlIndication_ShowError(utlArchive_ReadDataFromArray(CurrentArchive * 12 + 8)); //if you need more code space, you can indicate XXXX error code (16 bytes)
									break;
								default :
								    break;
							}

					}
					else if (CurrentPageLevel_0)
					{
						if (currentMode == BL_STATUS_MODE_ERROR)
						{
							utlIndication_ShowLabelERR();
						}
						else
						{
							switch(CurrentPageLevel_0)
							{
								case V_LEVEL_0_PAGE_1_T :
									utlIndication_ShowT(device->TEMPERATURE_WITH_K);
									break;
								case V_LEVEL_0_PAGE_2_R_VALUE :
									utlIndication_ShowR(device->COUNTING_RATE);
									break;
								case V_LEVEL_0_PAGE_3_CURR_DATE :
									//currentDate = (unsigned short)device->CURRENT_MONTH << 8;
									///currentDate |= device->CURRENT_YEAR;
									currentDate = (unsigned short)device->CURRENT_DAY << 8;
									currentDate |= device->CURRENT_MONTH;
									utlIndication_ShowDate(currentDate);
									break;
								case V_LEVEL_0_PAGE_4_CNTRL_R :
//									if (blStatusControl_GetStatus_short(BL_SC_SHORT_STATUS_CONTROL_R_OLD))
//										utlIndication_ShowR(0);
//									else
										utlIndication_ShowR(device->CONTROL_COUNTING_RATE);

									utlIndication_ShowSymbol_M();
									break;
								case V_LEVEL_0_PAGE_5_CNTRL_DATE :
									currentDate = (unsigned short)device->CONTROL_DAY << 8;
									currentDate |= device->CONTROL_MONTH;
									utlIndication_ShowDate(currentDate);
									utlIndication_ShowSymbol_M();
									break;
								case V_LEVEL_0_PAGE_6_CODE_H :
									utlIndication_ShowCCodeHigh(device->CHECK_READINGS_CODE);
									break;
								case V_LEVEL_0_PAGE_7_CODE_L :
									utlIndication_ShowCCodeLow(device->CHECK_READINGS_CODE);
									break;
								default :
									break;
							}
						}
					}
					break;
				default :
					break;
			}

			if (CurrentPageLevel_1 == V_LEVEL_INIT_VALUE)
			{
				if (device->ERROR & BL_STATUS_ERROR_BATTERY_DISCHARGED)// показать символ батарейки, если она разр€жена
				{
					utlIndication_ShowSymbol_BAT();
					if (currentMode != BL_STATUS_MODE_WORK)
						utlIndication_ResetBAT_SEG();
				}
				else
				{
					utlIndication_HideSymbol_BAT();
					if (currentMode != BL_STATUS_MODE_WORK)
						utlIndication_ClearSymbol_BAT();
				}
			}
		}


		if (utlIndication_CheckTimeout())
		{
			device->INDICATION_MODE = 2;
			utlIndication_ClearAllLCD();
		}

		utlIndication_Handler();

		if (device->INDICATION_MODE == 2)
		{
			device->INDICATION_MODE   = RESET;
			device->SWITCH_INDICATION = RESET;
			device->SWITCH_MODBUS     = SET;
			device->SWITCH_SLEEP      = SET;

			utlIndication_Deinit();
			utlKey_KeyHoldTimeSLEEP();
		}
	}
}


