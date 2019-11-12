//-----------------------------------------------------------------------------
// drvLCD.c
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <si_toolchain.h>
#include <SI_EFM8SB2_Register_Enums.h>                  // SI_SFR declarations

#include "drvLCD.h"
#include "../../3_HAL/halLCD/halLCD.h"

//-----------------------------------------------------------------------------
// Definitions
//-----------------------------------------------------------------------------
#define BASE_COUNT      P2_B7    // P0_B0
#define COM_0           P1_B1             //
#define COM_1           P1_B2             //
#define COM_2           P1_B4             // для новой платы на широком ЖКИ
#define COM_3           P1_B3             //

#define SEG_0           P0_B7
#define SEG_1           P1_B0
#define SIN             P0_B4             //
#define CLK             P0_B6             //
#define WR              P0_B5             //

#define ON              0x01
#define OFF             0x00

#define CADR_1          0x00
#define CADR_2          0x01
#define CADR_3          0x02
#define CADR_4          0x03
#define CADR_5          0x04
#define CADR_6          0x05
#define CADR_7          0x06
#define CADR_8          0x07
#define CADR_9          0x08
#define CADR_10         0x09
#define CADR_11         0x0A


#define GPIO_SIN        0x0E
#define GPIO_CLK        0x0F
#define GPIO_WR         0x10
#define GPIO_CLR        0x11


/*
        COMs         |   0   1   2   3      | SEGments    0   1   2   3   4   5   6   7   8   9
        PINs         |   1   2   3   4      |             5   6   7   8   9   10  11  12  13  14
------------------------------------------------------------------------------------------------
        COMs    COM1 |   COM1               |             5D  P5  4D  P1  1D  P2  2D  P3  3D  P4
                COM2 |       COM2           |             5C  5E  4C  1E  1C  2E  2C  3E  3C  4E
                COM3 |           COM3       |             5B  5G  4B  1G  1B  2G  2B  3G  3B  4G
                COM4 |               COM4   |             5A  5F  4A  1F  1A  2F  2A  3F  3A  4F
------------------------------------------------------------------------------------------------

 *                      1,2,3,4,5
                            -a-
                          f|   |b
                            -g-
                          e|   |c
                            -d-
P1 - "c"
P2 - "m"
P3 - "battery"
P4 - "dot1"
P5 - "dot2"
*/

#define LCD_INDICATION_FRAME_SEG_NUMBER     10
#define LCD_INDICATION_FRAME_SEG_CADRES     8

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------

SI_SEGMENT_VARIABLE(LCD_IndicationFrame_mass [LCD_INDICATION_FRAME_SEG_NUMBER][LCD_INDICATION_FRAME_SEG_CADRES], uint8_t,  SI_SEG_XDATA) =
 {    //          I          II          III         IV
//              COM1        COM2       COM3         COM4
//                A           B           C           D
/*P_SEG1*/    {ON, OFF,   ON, OFF,    ON, OFF,    ON, OFF},
//                E           F           G           SYMB
/*P_SEG2*/    {ON, OFF,   ON, OFF,    ON, OFF,    ON, OFF},
//                A           B           C           D
/*P_SEG3*/    {ON, OFF,   ON, OFF,    ON, OFF,    ON, OFF},
//                E           F           G           SYMB
/*P_SEG4*/    {ON, OFF,   ON, OFF,    ON, OFF,    ON, OFF},
//                A           B           C           D
/*P_SEG5*/    {ON, OFF,   ON, OFF,    ON, OFF,    ON, OFF},
//                E           F           G           SYMB
/*P_SEG6*/    {ON, OFF,   ON, OFF,    ON, OFF,    ON, OFF},
//                A           B           C           D
/*P_SEG7*/    {ON, OFF,   ON, OFF,    ON, OFF,    ON, OFF},
//                E           F           G           SYMB
/*P_SEG8*/    {ON, OFF,   ON, OFF,    ON, OFF,    ON, OFF},
//                A           B           C           D
/*P_SEG9*/    {ON, OFF,   ON, OFF,    ON, OFF,    ON, OFF},
//                E           F           G           SYMB
/*P_SEG10*/   {ON, OFF,   ON, OFF,    ON, OFF,    ON, OFF}
              
};                                              


unsigned char CadrParts;


//-----------------------------------------------------------------------------
// Routine
// ----------------------------------------------------------------------------

void drvLCD_SetCadrParts (unsigned char partsNumber)
{
	CadrParts = partsNumber;
}

unsigned char drvLCD_LCDbyGPIO_InCadr_Handler(unsigned char InCadr, unsigned char state)
{
	if ((state == ON) && (InCadr > CadrParts))
	{
		return OFF;
	}
	else if (state == ON)
	{
		return ON;
	}
	else if (state == OFF)
	{
		return OFF;
	}
	else
	{
		return OFF;
	}
}

void drvLCD_LCDbyGPIO_COM_Handler(unsigned char Cadr, unsigned char InCADRNum)
{

    switch (Cadr)
    {
        case CADR_1 :

        	BASE_COUNT = drvLCD_LCDbyGPIO_InCadr_Handler(InCADRNum, ON);//ON;

        	COM_0 = drvLCD_LCDbyGPIO_InCadr_Handler(InCADRNum, OFF);//OFF;
			COM_1 = drvLCD_LCDbyGPIO_InCadr_Handler(InCADRNum, ON);//ON;
			COM_2 = drvLCD_LCDbyGPIO_InCadr_Handler(InCADRNum, OFF);//OFF;
			COM_3 = drvLCD_LCDbyGPIO_InCadr_Handler(InCADRNum, OFF);//OFF;
            break;
        case CADR_2 :

        	BASE_COUNT = drvLCD_LCDbyGPIO_InCadr_Handler(InCADRNum, OFF);//OFF;

        	COM_0 = drvLCD_LCDbyGPIO_InCadr_Handler(InCADRNum, ON);//ON;
			COM_1 = drvLCD_LCDbyGPIO_InCadr_Handler(InCADRNum, OFF);//OFF;
			COM_2 = drvLCD_LCDbyGPIO_InCadr_Handler(InCADRNum, ON);//ON;
			COM_3 = drvLCD_LCDbyGPIO_InCadr_Handler(InCADRNum, ON);//ON;
            break;
        case CADR_3 :

        	BASE_COUNT = drvLCD_LCDbyGPIO_InCadr_Handler(InCADRNum, ON);//ON;

        	COM_0 = drvLCD_LCDbyGPIO_InCadr_Handler(InCADRNum, OFF);//OFF;
			COM_1 = drvLCD_LCDbyGPIO_InCadr_Handler(InCADRNum, OFF);//OFF;
			COM_2 = drvLCD_LCDbyGPIO_InCadr_Handler(InCADRNum, ON);//ON;
			COM_3 = drvLCD_LCDbyGPIO_InCadr_Handler(InCADRNum, OFF);//OFF;
            break;
        case CADR_4 :

        	BASE_COUNT = drvLCD_LCDbyGPIO_InCadr_Handler(InCADRNum, OFF);//OFF;

        	COM_0 = drvLCD_LCDbyGPIO_InCadr_Handler(InCADRNum, ON);//ON;
			COM_1 = drvLCD_LCDbyGPIO_InCadr_Handler(InCADRNum, ON);//ON;
			COM_2 = drvLCD_LCDbyGPIO_InCadr_Handler(InCADRNum, OFF);//OFF;
			COM_3 = drvLCD_LCDbyGPIO_InCadr_Handler(InCADRNum, ON);//ON;
        	break;
        case CADR_5 :

        	BASE_COUNT = drvLCD_LCDbyGPIO_InCadr_Handler(InCADRNum, ON);//ON;

        	COM_0 = drvLCD_LCDbyGPIO_InCadr_Handler(InCADRNum, OFF);//OFF;
        	COM_1 = drvLCD_LCDbyGPIO_InCadr_Handler(InCADRNum, OFF);//OFF;
			COM_2 = drvLCD_LCDbyGPIO_InCadr_Handler(InCADRNum, OFF);//OFF;
			COM_3 = drvLCD_LCDbyGPIO_InCadr_Handler(InCADRNum, ON);//ON;
            break;
        case CADR_6 :

        	BASE_COUNT = drvLCD_LCDbyGPIO_InCadr_Handler(InCADRNum, OFF);//OFF;

        	COM_0 = drvLCD_LCDbyGPIO_InCadr_Handler(InCADRNum, ON);//ON;
			COM_1 = drvLCD_LCDbyGPIO_InCadr_Handler(InCADRNum, ON);//ON;
			COM_2 = drvLCD_LCDbyGPIO_InCadr_Handler(InCADRNum, ON);//ON;
			COM_3 = drvLCD_LCDbyGPIO_InCadr_Handler(InCADRNum, OFF);//OFF;
            break;
        case CADR_7 :

        	BASE_COUNT = drvLCD_LCDbyGPIO_InCadr_Handler(InCADRNum, ON);//ON;

        	COM_0 = drvLCD_LCDbyGPIO_InCadr_Handler(InCADRNum, ON);//ON;
			COM_1 = drvLCD_LCDbyGPIO_InCadr_Handler(InCADRNum, OFF);//OFF;
			COM_2 = drvLCD_LCDbyGPIO_InCadr_Handler(InCADRNum, OFF);//OFF;
			COM_3 = drvLCD_LCDbyGPIO_InCadr_Handler(InCADRNum, OFF);//OFF;
            break;
        case CADR_8 :

        	BASE_COUNT = drvLCD_LCDbyGPIO_InCadr_Handler(InCADRNum, OFF);//OFF;

        	COM_0 = drvLCD_LCDbyGPIO_InCadr_Handler(InCADRNum, OFF);//OFF;
			COM_1 = drvLCD_LCDbyGPIO_InCadr_Handler(InCADRNum, ON);//ON;
			COM_2 = drvLCD_LCDbyGPIO_InCadr_Handler(InCADRNum, ON);//ON;
			COM_3 = drvLCD_LCDbyGPIO_InCadr_Handler(InCADRNum, ON);//ON;
            break;

        default :
            break;
    }
}


void drvLCD_LCDbyGPIO_SEG_Handler (unsigned char Cadr, unsigned char InCADRNum, unsigned char SelectPin)
{
	switch (Cadr)
	{
		case CADR_1 :
			drvLCD_Set_GPIO(SelectPin, drvLCD_LCDbyGPIO_InCadr_Handler(InCADRNum, LCD_IndicationFrame_mass[SelectPin][Cadr]));
			break;
		case CADR_2 :
			drvLCD_Set_GPIO(SelectPin, drvLCD_LCDbyGPIO_InCadr_Handler(InCADRNum, LCD_IndicationFrame_mass[SelectPin][Cadr]));
			break;
		case CADR_3 :
			drvLCD_Set_GPIO(SelectPin, drvLCD_LCDbyGPIO_InCadr_Handler(InCADRNum, LCD_IndicationFrame_mass[SelectPin][Cadr]));
			break;
		case CADR_4 :
			drvLCD_Set_GPIO(SelectPin, drvLCD_LCDbyGPIO_InCadr_Handler(InCADRNum, LCD_IndicationFrame_mass[SelectPin][Cadr]));
			break;
		case CADR_5 :
			drvLCD_Set_GPIO(SelectPin, drvLCD_LCDbyGPIO_InCadr_Handler(InCADRNum, LCD_IndicationFrame_mass[SelectPin][Cadr]));
			break;
		case CADR_6 :
			drvLCD_Set_GPIO(SelectPin, drvLCD_LCDbyGPIO_InCadr_Handler(InCADRNum, LCD_IndicationFrame_mass[SelectPin][Cadr]));
			break;
		case CADR_7 :
			drvLCD_Set_GPIO(SelectPin, drvLCD_LCDbyGPIO_InCadr_Handler(InCADRNum, LCD_IndicationFrame_mass[SelectPin][Cadr]));
			break;
		case CADR_8 :
			drvLCD_Set_GPIO(SelectPin, drvLCD_LCDbyGPIO_InCadr_Handler(InCADRNum, LCD_IndicationFrame_mass[SelectPin][Cadr]));
			break;
		default :
			break;
	}
}

void drvLCD_Set_GPIO (unsigned char GPIO_ID, unsigned char state)
{
    switch (GPIO_ID)
    {
        case GPIO_SEG_0 :
			SEG_0 = state;
			break;
        case GPIO_SEG_1 :
			SEG_1 = state;
			break;
        case GPIO_SIN :
			SIN = state;
			break;
        case GPIO_CLK :
			CLK = state;
			break;
        case GPIO_WR :
			WR = state;
			break;
        default :
			break;
    }
}



void drvLCD_Set_Segment (unsigned char segmentID)
{
    drvLCD_UpdateIndicationTable (segmentID, OFF, ON);
}

void drvLCD_Reset_Segment (unsigned char segmentID)
{
    drvLCD_UpdateIndicationTable (segmentID, ON, OFF);
}

void drvLCD_Clear_Segment (unsigned char segmentID)
{
    drvLCD_UpdateIndicationTable (segmentID, OFF, OFF);
}

void drvLCD_UpdateIndicationTable (unsigned char segmentID, unsigned char tablePlace1, unsigned char tablePlace2)
{
    unsigned char segment;
    unsigned char symbol;

    segment = segmentID >> 4;
    symbol = segmentID & 0x0F;

    LCD_IndicationFrame_mass[segment][symbol] = tablePlace1;
    LCD_IndicationFrame_mass[segment][symbol+1] = tablePlace2;
}

void drvLCD_ShiftRegisterHandler (unsigned char CADRNum, unsigned char InCADRNum)
{
	unsigned char i;

	for (i = 0; i < 8; i++)
	{
		if (CADRNum > CADR_8)
		{
			drvLCD_Set_GPIO(GPIO_SIN, OFF);
		}
		else
		{
			drvLCD_Set_GPIO(GPIO_SIN, drvLCD_LCDbyGPIO_InCadr_Handler(InCADRNum, LCD_IndicationFrame_mass[i+2][CADRNum]));
		}

		drvLCD_Set_GPIO(GPIO_CLK, ON);

		drvLCD_Set_GPIO(GPIO_CLK, OFF);
	}
	drvLCD_Set_GPIO(GPIO_WR, OFF);
	drvLCD_Set_GPIO(GPIO_WR, ON);
}

void drvLCD_SetAllSegments(unsigned char value)
{
    unsigned char i;
    unsigned char j;

    for (i = 0; i < LCD_INDICATION_FRAME_SEG_CADRES; i++)
    {
        value = !value;

        for (j = 0; j < LCD_INDICATION_FRAME_SEG_NUMBER; j++)
        {
            LCD_IndicationFrame_mass[j][i] = value;
        }
    }
}

void drvLCD_ClearAllSegments(void)
{
    unsigned char i;
    unsigned char j;

    for (i = 0; i < LCD_INDICATION_FRAME_SEG_CADRES; i++)
       {
           for (j = 0; j < LCD_INDICATION_FRAME_SEG_NUMBER; j++)
           {
               LCD_IndicationFrame_mass[j][i] = 0;
           }
       }
}


void drvLCD_InitLCD(void)
{
	halLCD_InitLCD();
}

void drvLCD_DeinitLCD(void)
{
	halLCD_DeinitLCD();
}


