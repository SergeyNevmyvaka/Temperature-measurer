
#include "utlIndication.h"
#include "si_toolchain.h"
#include "../../2_Drv/drvLCD/drvLCD.h"
#include "../../2_Drv/drvSysClock/drvSysClock.h"
#include <SI_EFM8SB2_Register_Enums.h>                  // SI_SFR declarations


static unsigned char NextLCDCadr = 0;
static unsigned char CADRNum;
static unsigned char InCadrNum;
static unsigned long TimeoutTicks;


void utlIndication_Init(float voltage)
{
    CADRNum = 7;
    InCadrNum = 2;

	// увеличить €ркость, если батарейка разр€жена
    drvLCD_SetCadrParts(CADRES_PIECES_NUMBER_2);

    if (voltage < UTL_INDICATION_CHANGE_VOLTAGE)
	{
		drvLCD_SetCadrParts(CADRES_PIECES_NUMBER_3);
	}

    drvLCD_InitLCD();
}

void utlIndication_Deinit(void)
{
    drvLCD_DeinitLCD();
}

/////////////////Timeout functions//////////////////////////////////////////////////////////////
void utlIndication_StartTimeout(void)
{
	TimeoutTicks = drvSysClock_getTime();
}

unsigned char utlIndication_CheckTimeout(void)
{
	if (drvSysClock_getPastTime(TimeoutTicks/*, SYSCLOCK_GET_TIME_MS_1*/) > UTL_INDICATION_TIMEOUT_PERIOD)
	{
		return 1;
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////

void utlIndication_Handler(void)
{

    if (NextLCDCadr)
    {
        NextLCDCadr = 0;

        PCON0 |= PCON0_IDLE__IDLE;         // set IDLE bit
        PCON0 = PCON0;                     // ... followed by a 3-cycle dummy instruction to prevent wrong interupt-wake
    }
}

//-----------------------------------------------------------------------------
// TIMER2_ISR
//-----------------------------------------------------------------------------
//
// TIMER2 ISR Content goes here. Remember to clear flag bits:
// TMR2CN0::TF2H (Timer # High Byte Overflow Flag)
// TMR2CN0::TF2L (Timer # Low Byte Overflow Flag)
//
//-----------------------------------------------------------------------------
SI_INTERRUPT (TIMER2_ISR, TIMER2_IRQn)
{
	TMR2CN0 &= ~TMR2CN0_TF2H__SET;
	NextLCDCadr = 1;

	InCadrNum++;

	if (InCadrNum > CADRES_PIECES_NUMBER_4)// 3 2 1 0   CADRES_PIECES_NUMBER_4
	{
		InCadrNum = 0;
		CADRNum++;

		if (CADRNum > CADRES_NUMBER_8)//7 8 9 10)
	    	CADRNum = 0;
	}

    drvLCD_ShiftRegisterHandler(CADRNum, InCadrNum);
    drvLCD_LCDbyGPIO_SEG_Handler(CADRNum, InCadrNum, GPIO_SEG_0);
    drvLCD_LCDbyGPIO_SEG_Handler(CADRNum, InCadrNum, GPIO_SEG_1);
    drvLCD_LCDbyGPIO_COM_Handler(CADRNum, InCadrNum);
}


void utlIndication_32bitHexToBCD(unsigned char *BCD, unsigned long value)
{
    unsigned char i;

    for (i = 5; i > 0; i--)
    {
        BCD[i-1] = value % 10;
        value /= 10;
    }
}

void utlIndication_ShowValue(unsigned long value, unsigned char modeID)
{
    unsigned char BCD[5] = {0,0,0,0,0};
    unsigned char i;
    unsigned char month1;
    unsigned char month2;

    drvLCD_SetAllSegments(DRVLCD_CLEAR_ALL);

    if ((modeID == UTL_INDICATION_SHOW_MODE_VALUE)
    		|| (modeID == UTL_INDICATION_SHOW_MODE_T))
//    		|| (modeID == UTL_INDICATION_SHOW_MODE_ERROR))
    {
		utlIndication_32bitHexToBCD(BCD, value);

//		if (modeID == UTL_INDICATION_SHOW_MODE_T)
//		{
//			BCD[0] = 0x10;
//		}
    }
    else if (modeID == UTL_INDICATION_SHOW_MODE_DATE_TIME)
    {
    	utlIndication_32bitHexToBCD(BCD, value >> 8);
    	month2 = BCD[3];
    	month1 = BCD[4];

    	utlIndication_32bitHexToBCD(BCD, value & 0xFF);
    	BCD[1] = month2;
    	BCD[2] = month1;
//**//   	BCD[0] = 0x10;
    }
//    else if (modeID == UTL_INDICATION_SHOW_MODE_ERROR)
//    {
//       	BCD[4] = value & 0xF;
//       	BCD[3] = (value >> 4) & 0xF;
//       	BCD[2] = (value >> 8) & 0xF;
//    	BCD[1] = 0x10;
//    	BCD[0] = 0x10;
//
//    	//utlIndication_ClearPlace(1);
//    	utlIndication_ClearPlace(2);
//    }
//    else if (modeID == UTL_INDICATION_SHOW_MODE_T)
//    {
//    	utlIndication_32bitHexToBCD(BCD, value);
//    	BCD[0] = 0x10;
//    }
    else if ((modeID == UTL_INDICATION_SHOW_MODE_CODE)
		|| (modeID == UTL_INDICATION_SHOW_MODE_ERROR))
    {
//		utlIndication_32bitHexToBCD(BCD, value);
    	BCD[4] = value & 0xF;
    	BCD[3] = (value >> 4) & 0xF;
    	BCD[2] = (value >> 8) & 0xF;
    	BCD[1] = (value >> 12) & 0xF;
//**//	BCD[0] = 0x10;


    }
    
    if (modeID != UTL_INDICATION_SHOW_MODE_VALUE)
	{
		BCD[0] = 0x10;
	}

    if (modeID == UTL_INDICATION_SHOW_MODE_ERROR)
	{
    	BCD[1] = 0x10;
    	utlIndication_ClearPlace(2);
	}



    for (i = 0; i < 5; i++)
    {
        switch(BCD[i])
        {
            case 0:
                utlIndication_ShowNumeral_0(i+1);
                break;
            case 1:
                utlIndication_ShowNumeral_1(i+1);
                break;
            case 2:
                utlIndication_ShowNumeral_2(i+1);
                break;
            case 3:
                utlIndication_ShowNumeral_3(i+1);
                break;
            case 4:
                utlIndication_ShowNumeral_4(i+1);
                break;
            case 5:
                utlIndication_ShowNumeral_5(i+1);
                break;
            case 6:
                utlIndication_ShowNumeral_6(i+1);
                break;
            case 7:
                utlIndication_ShowNumeral_7(i+1);
                break;
            case 8:
                utlIndication_ShowNumeral_8(i+1);
                break;
            case 9:
                utlIndication_ShowNumeral_9(i+1);
                break;
            case 0xA:
                utlIndication_ShowNumeral_A(i+1);
                break;
            case 0xB:
                utlIndication_ShowNumeral_B(i+1);
                break;
            case 0xC:
                utlIndication_ShowNumeral_C(i+1);
                break;
            case 0xD:
                utlIndication_ShowNumeral_D(i+1);
                break;
            case 0xE:
                utlIndication_ShowNumeral_E(i+1);
                break;
            case 0xF:
                utlIndication_ShowNumeral_F(i+1);
                break;
            case 0x10: //E
            	utlIndication_ClearPlace(1);
                break;
//            case 0x11: //F
//            	utlIndication_ShowSymbol_Tyre(i+1);
//                break;
            default:
                break;
        }
    }
}


void utlIndication_ShowR(float R)
{
    utlIndication_ShowValue((unsigned long)(R + 0.5), UTL_INDICATION_SHOW_MODE_VALUE);
}

void utlIndication_ShowDate(unsigned short date)
{
    
    utlIndication_ShowValue(date, UTL_INDICATION_SHOW_MODE_DATE_TIME);
    drvLCD_Set_Segment(LCD_SEG_DOT1);
}

void utlIndication_ShowT(float T)
{
    unsigned long showValue;
    
    showValue = (unsigned long)(T * 100.0);

    if(T >= 100)
    {
    	utlIndication_ShowValue(showValue, UTL_INDICATION_SHOW_MODE_VALUE);
	}
	else
	{
		utlIndication_ShowValue(showValue, UTL_INDICATION_SHOW_MODE_T);
	}
////------------------------------------------------------////
    drvLCD_Set_Segment(LCD_SEG_C);
    drvLCD_Set_Segment(LCD_SEG_DOT1);
}

void utlIndication_ShowCCodeLow(unsigned long cod)
{
    utlIndication_ShowValue(cod, UTL_INDICATION_SHOW_MODE_CODE);
    utlIndication_ShowSymbol_Lowcod();
}

void utlIndication_ShowCCodeHigh(unsigned long cod)
{
    unsigned long showValue;
    
    showValue = (unsigned long)(cod >> 16);
    utlIndication_ShowValue(showValue, UTL_INDICATION_SHOW_MODE_CODE);
    utlIndication_ShowSymbol_HighCod();
}

void utlIndication_ShowError (unsigned long error)
{
    utlIndication_ShowValue(error, UTL_INDICATION_SHOW_MODE_ERROR);
}

void utlIndication_ShowLabelSLEEP(void)
{
	drvLCD_SetAllSegments(DRVLCD_CLEAR_ALL);

    utlIndication_ShowNumeral_5(1);

    utlIndication_ShowNumeral_C(2);
    drvLCD_Reset_Segment(LCD_SEG_2_A);

    utlIndication_ShowNumeral_E(3);

    utlIndication_ShowNumeral_E(4);

    utlIndication_ShowNumeral_8(5);
    drvLCD_Reset_Segment(LCD_SEG_5_C);
    drvLCD_Reset_Segment(LCD_SEG_5_D);
}

void utlIndication_ShowLabelERR(void)
{
	drvLCD_SetAllSegments(DRVLCD_CLEAR_ALL);

	utlIndication_ShowNumeral_E(3);

    drvLCD_Set_Segment(LCD_SEG_4_E);
    drvLCD_Set_Segment(LCD_SEG_4_G);

    drvLCD_Set_Segment(LCD_SEG_5_E);
    drvLCD_Set_Segment(LCD_SEG_5_G);

    utlIndication_ClearPlace(1);
    utlIndication_ClearPlace(2);
}

void utlIndication_ShowAllLCD(void)
{
    drvLCD_SetAllSegments(DRVLCD_FILL_ALL);
}

void utlIndication_ClearAllLCD(void)
{
	drvLCD_ClearAllSegments();
}

void utlIndication_ShowLabelArh(unsigned char currentArchive)
{
	drvLCD_SetAllSegments(DRVLCD_CLEAR_ALL);

	utlIndication_ShowValue(currentArchive, UTL_INDICATION_SHOW_MODE_VALUE);

	// A
	utlIndication_ShowNumeral_A(1);
	// r
	utlIndication_ShowNumeral_F(2);
	drvLCD_Reset_Segment(LCD_SEG_2_A);
	drvLCD_Reset_Segment(LCD_SEG_2_F);
	// h
	utlIndication_ShowNumeral_B(3);
	drvLCD_Reset_Segment(LCD_SEG_3_D);
}

unsigned char utlIndication_ConvertPlaceIntoSEG(unsigned char place, unsigned char symbol)
{
    unsigned char returnValue;// = 0;
    //unsigned char buff = 0;
    unsigned char shift;

    if (symbol < SOFT_SEG_G)
    {
        shift = 0;
    }
    else
    {
        shift = 1;
    }

    switch(place)
    {
        case 1:
        	if (shift)
        	{  //EFG SYMB
        		returnValue = SOFT_SEG_1;//buff = (place + 7); // << 4;   //8
        	}
        	else
        	{      // ABCD
        		returnValue = SOFT_SEG_0;//buff = (place + 8);    //9
        	}
            break;
        case 2:
        	if (shift)
        	{  //EFG SYMB
        		returnValue = SOFT_SEG_3;//buff = (place + 4);    //6
        	}
        	else
        	{      // ABCD
        		returnValue = SOFT_SEG_2;//buff = (place + 5);    //7
        	}
            break;
        case 3:
        	if (shift)
        	{  //EFG SYMB
        		returnValue = SOFT_SEG_5;//buff = (place + 1);    //4
        	}
        	else
        	{      // ABCD
        		returnValue = SOFT_SEG_4;//buff = (place + 2);    //5
        	}
            break;
        case 4:
        	if (shift)
        	{  //EFG SYMB
        		returnValue = SOFT_SEG_8;//buff = (place - 3);    //1
        	}
        	else
        	{      // ABCD
        		returnValue = SOFT_SEG_6;//buff = (place - 1);    //3
        	}
            break;
        case 5:
        	if (shift)
        	{  //EFG SYMB
        		returnValue = SOFT_SEG_7;//buff = (place - 3);    //2
        	}
        	else
        	{      // ABCD
        		returnValue = SOFT_SEG_9;//buff = (place - 5);    //0
        	}
            break;
        default:
        	//buff = place;
            break;
    }

    //returnValue = buff << 4;
    returnValue |= symbol & 0x0F;

    return returnValue;
}

////////////////////////////////////////////////////////////////////////////////
void utlIndication_ShowNumeral_1(unsigned char place)
{
	unsigned char buff;

    buff = utlIndication_ConvertPlaceIntoSEG(place, SOFT_SEG_B);
    drvLCD_Set_Segment(buff);

    buff = utlIndication_ConvertPlaceIntoSEG(place, SOFT_SEG_C);
    drvLCD_Set_Segment(buff);

    //resettings
    utlIndication_Clear_ADEF(place);
}

void utlIndication_ShowNumeral_2(unsigned char place)
{
    unsigned char buff;

    utlIndication_ShowNumeral_8(place);

    buff = utlIndication_ConvertPlaceIntoSEG(place, SOFT_SEG_C);
    drvLCD_Reset_Segment(buff);

    buff = utlIndication_ConvertPlaceIntoSEG(place, SOFT_SEG_F);
    drvLCD_Reset_Segment(buff);
}
void utlIndication_ShowNumeral_3(unsigned char place)
{
	unsigned char buff;

    utlIndication_ShowNumeral_8(place);

    buff = utlIndication_ConvertPlaceIntoSEG(place, SOFT_SEG_E);
    drvLCD_Reset_Segment(buff);

    buff = utlIndication_ConvertPlaceIntoSEG(place, SOFT_SEG_F);
    drvLCD_Reset_Segment(buff);
}
void utlIndication_ShowNumeral_4(unsigned char place)
{
	unsigned char buff;

    utlIndication_ShowNumeral_8(place);

    buff = utlIndication_ConvertPlaceIntoSEG(place, SOFT_SEG_A);
    drvLCD_Reset_Segment(buff);

    buff = utlIndication_ConvertPlaceIntoSEG(place, SOFT_SEG_D);
    drvLCD_Reset_Segment(buff);

    buff = utlIndication_ConvertPlaceIntoSEG(place, SOFT_SEG_E);
    drvLCD_Reset_Segment(buff);
}
void utlIndication_ShowNumeral_5(unsigned char place)
{
	unsigned char buff;

    utlIndication_ShowNumeral_8(place);

    buff = utlIndication_ConvertPlaceIntoSEG(place, SOFT_SEG_B);
    drvLCD_Reset_Segment(buff);

    buff = utlIndication_ConvertPlaceIntoSEG(place, SOFT_SEG_E);
    drvLCD_Reset_Segment(buff);

}
void utlIndication_ShowNumeral_6(unsigned char place)
{
	unsigned char buff;

    utlIndication_ShowNumeral_8(place);

    buff = utlIndication_ConvertPlaceIntoSEG(place, SOFT_SEG_B);
    drvLCD_Reset_Segment(buff);

}
void utlIndication_ShowNumeral_7(unsigned char place)
{
	unsigned char buff;

    buff = utlIndication_ConvertPlaceIntoSEG(place, SOFT_SEG_A);
    drvLCD_Set_Segment(buff);

    buff = utlIndication_ConvertPlaceIntoSEG(place, SOFT_SEG_B);
    drvLCD_Set_Segment(buff);

    buff = utlIndication_ConvertPlaceIntoSEG(place, SOFT_SEG_C);
    drvLCD_Set_Segment(buff);
    
}
void utlIndication_ShowNumeral_8(unsigned char place)
{
	unsigned char buff;

    buff = utlIndication_ConvertPlaceIntoSEG(place, SOFT_SEG_A);
    drvLCD_Set_Segment(buff);

    buff = utlIndication_ConvertPlaceIntoSEG(place, SOFT_SEG_B);
    drvLCD_Set_Segment(buff);

    buff = utlIndication_ConvertPlaceIntoSEG(place, SOFT_SEG_C);
    drvLCD_Set_Segment(buff);

    buff = utlIndication_ConvertPlaceIntoSEG(place, SOFT_SEG_D);
    drvLCD_Set_Segment(buff);

    buff = utlIndication_ConvertPlaceIntoSEG(place, SOFT_SEG_E);
    drvLCD_Set_Segment(buff);

    buff = utlIndication_ConvertPlaceIntoSEG(place, SOFT_SEG_F);
    drvLCD_Set_Segment(buff);

    buff = utlIndication_ConvertPlaceIntoSEG(place, SOFT_SEG_G);
    drvLCD_Set_Segment(buff);
    
}
void utlIndication_ShowNumeral_9(unsigned char place)
{
	unsigned char buff;

    utlIndication_ShowNumeral_8(place);

    buff = utlIndication_ConvertPlaceIntoSEG(place, SOFT_SEG_E);
    drvLCD_Reset_Segment(buff);
}
void utlIndication_ShowNumeral_0(unsigned char place)
{
	unsigned char buff;

    utlIndication_ShowNumeral_8(place);

    buff = utlIndication_ConvertPlaceIntoSEG(place, SOFT_SEG_G);
    drvLCD_Reset_Segment(buff);
}


void utlIndication_ShowNumeral_A(unsigned char place)
{
	unsigned char buff;

    utlIndication_ShowNumeral_8(place);

    buff = utlIndication_ConvertPlaceIntoSEG(place, SOFT_SEG_D);
    drvLCD_Reset_Segment(buff);
}

void utlIndication_ShowNumeral_B(unsigned char place)
{
	unsigned char buff;

    utlIndication_ShowNumeral_8(place);

    buff = utlIndication_ConvertPlaceIntoSEG(place, SOFT_SEG_A);
    drvLCD_Reset_Segment(buff);

    buff = utlIndication_ConvertPlaceIntoSEG(place, SOFT_SEG_B);
    drvLCD_Reset_Segment(buff);
}

void utlIndication_ShowNumeral_C(unsigned char place)
{
	unsigned char buff;

    utlIndication_ShowNumeral_8(place);

    buff = utlIndication_ConvertPlaceIntoSEG(place, SOFT_SEG_B);
    drvLCD_Clear_Segment(buff);

    buff = utlIndication_ConvertPlaceIntoSEG(place, SOFT_SEG_C);
    drvLCD_Clear_Segment(buff);

    buff = utlIndication_ConvertPlaceIntoSEG(place, SOFT_SEG_G);
    drvLCD_Clear_Segment(buff);

    buff = utlIndication_ConvertPlaceIntoSEG(place, SOFT_SEG_SYMB);
    drvLCD_Clear_Segment(buff);

}

void utlIndication_ShowNumeral_D(unsigned char place)
{
	unsigned char buff;

    utlIndication_ShowNumeral_8(place);

    buff = utlIndication_ConvertPlaceIntoSEG(place, SOFT_SEG_A);
    drvLCD_Reset_Segment(buff);

    buff = utlIndication_ConvertPlaceIntoSEG(place, SOFT_SEG_F);
    drvLCD_Reset_Segment(buff);
}

void utlIndication_ShowNumeral_E(unsigned char place)
{
	unsigned char buff;

    utlIndication_ShowNumeral_6(place);

    buff = utlIndication_ConvertPlaceIntoSEG(place, SOFT_SEG_C);
    drvLCD_Reset_Segment(buff);
}

void utlIndication_ShowNumeral_F(unsigned char place)
{
	unsigned char buff;

    utlIndication_ShowNumeral_6(place);

    buff = utlIndication_ConvertPlaceIntoSEG(place, SOFT_SEG_C);
    drvLCD_Reset_Segment(buff);

    buff = utlIndication_ConvertPlaceIntoSEG(place, SOFT_SEG_D);
    drvLCD_Reset_Segment(buff);
}


void utlIndication_ClearPlace(unsigned char place)
{
    unsigned char buff;


   buff = utlIndication_ConvertPlaceIntoSEG(place, SOFT_SEG_B);
   drvLCD_Clear_Segment(buff);

   buff = utlIndication_ConvertPlaceIntoSEG(place, SOFT_SEG_C);
   drvLCD_Clear_Segment(buff);

   buff = utlIndication_ConvertPlaceIntoSEG(place, SOFT_SEG_G);
   drvLCD_Clear_Segment(buff);

   buff = utlIndication_ConvertPlaceIntoSEG(place, SOFT_SEG_SYMB);
   drvLCD_Clear_Segment(buff);

   utlIndication_Clear_ADEF(place);

}

void utlIndication_ShowSymbol_BAT(void)
{
    drvLCD_Set_Segment(LCD_SEG_BAT);
}

void utlIndication_ResetBAT_SEG(void)
{
    drvLCD_Reset_Segment(LCD_SEG_2_B);
    drvLCD_Reset_Segment(LCD_SEG_2_C);
    drvLCD_Reset_Segment(LCD_SEG_2_G);
}

void utlIndication_ClearSymbol_BAT(void)
{
	drvLCD_Clear_Segment(LCD_SEG_BAT);
}

void utlIndication_HideSymbol_BAT(void)
{
	drvLCD_Reset_Segment(LCD_SEG_BAT);
}

void utlIndication_ShowSymbol_M(void)
{
    drvLCD_Set_Segment(LCD_SEG_M);
}

void utlIndication_Clear_ADEF(unsigned char place)
{
	unsigned char buff;

	//resettings
	buff = utlIndication_ConvertPlaceIntoSEG(place, SOFT_SEG_A);
	drvLCD_Clear_Segment(buff);
	buff = utlIndication_ConvertPlaceIntoSEG(place, SOFT_SEG_D);
	drvLCD_Clear_Segment(buff);
	buff = utlIndication_ConvertPlaceIntoSEG(place, SOFT_SEG_E);
	drvLCD_Clear_Segment(buff);
	buff = utlIndication_ConvertPlaceIntoSEG(place, SOFT_SEG_F);
	drvLCD_Clear_Segment(buff);
}

void utlIndication_ShowSymbol_Tyre(unsigned char place)
{
    unsigned char buff;

    buff = utlIndication_ConvertPlaceIntoSEG(place, SOFT_SEG_G);
    drvLCD_Set_Segment(buff);

    utlIndication_Clear_ADEF(place);
}

void utlIndication_ShowSymbol_Lowcod(void)
{
	// L
	drvLCD_Clear_Segment(LCD_SEG_1_B);
    drvLCD_Clear_Segment(LCD_SEG_1_C);
    drvLCD_Clear_Segment(LCD_SEG_1_G);
    drvLCD_Clear_Segment(LCD_SEG_C);

    drvLCD_Set_Segment(LCD_SEG_1_D);
    drvLCD_Set_Segment(LCD_SEG_1_E);
    drvLCD_Set_Segment(LCD_SEG_1_F);
    drvLCD_Reset_Segment(LCD_SEG_1_A);
}

void utlIndication_ShowSymbol_HighCod(void)
{
	// H
    drvLCD_Set_Segment(LCD_SEG_1_B);
    drvLCD_Set_Segment(LCD_SEG_1_C);
    drvLCD_Set_Segment(LCD_SEG_1_E);
    drvLCD_Set_Segment(LCD_SEG_1_F);
    drvLCD_Set_Segment(LCD_SEG_1_G);

    drvLCD_Reset_Segment(LCD_SEG_1_A);
    drvLCD_Reset_Segment(LCD_SEG_1_D);
    drvLCD_Reset_Segment(LCD_SEG_C);
}


