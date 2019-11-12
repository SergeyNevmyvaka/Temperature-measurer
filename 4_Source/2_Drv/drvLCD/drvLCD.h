
#ifndef __DRV_LCD_H__
#define __DRV_LCD_H__

#ifdef	__cplusplus
extern "C" {
#endif

#define CADRES_NUMBER_8            7
#define CADRES_NUMBER_8_IDLE_1     8
#define CADRES_NUMBER_8_IDLE_2     9

#define CADRES_PIECES_NUMBER_4     3
#define CADRES_PIECES_NUMBER_3     2
#define CADRES_PIECES_NUMBER_2     1
#define CADRES_PIECES_NUMBER_1     0
// pins

#define SOFT_SEG_0      0x90
#define SOFT_SEG_1      0x80
#define SOFT_SEG_2      0x70
#define SOFT_SEG_3      0x60
#define SOFT_SEG_4      0x50
#define SOFT_SEG_5      0x40
#define SOFT_SEG_6      0x30
#define SOFT_SEG_7      0x20
#define SOFT_SEG_8      0x10
#define SOFT_SEG_9      0x00



#define SOFT_SEG_A      0x06
#define SOFT_SEG_F      0x00
#define SOFT_SEG_E      0x02
#define SOFT_SEG_D      0x04

#define SOFT_SEG_B      0x16
#define SOFT_SEG_G      0x10
#define SOFT_SEG_C      0x12
#define SOFT_SEG_SYMB   0x14
//||

//C DOT1 DOT2 M T
// табличный SEG            // назначенный в ПО SEG
#define LCD_SEG_1_A     (SOFT_SEG_0 | SOFT_SEG_A)              // 0x90  | 0x06
#define LCD_SEG_1_F     (SOFT_SEG_0 | SOFT_SEG_F)              // 0x90  | 0x00
#define LCD_SEG_1_E     (SOFT_SEG_0 | SOFT_SEG_E)              // 0x90  | 0x02
#define LCD_SEG_1_D     (SOFT_SEG_0 | SOFT_SEG_D)              // 0x90  | 0x04

#define LCD_SEG_1_B     (SOFT_SEG_1 | (SOFT_SEG_B & 0x0F))     // 0x80  | 0x16
#define LCD_SEG_1_G     (SOFT_SEG_1 | (SOFT_SEG_G & 0x0F))     // 0x80  | 0x10
#define LCD_SEG_1_C     (SOFT_SEG_1 | (SOFT_SEG_C & 0x0F))     // 0x80  | 0x12
#define LCD_SEG_C       (SOFT_SEG_1 | (SOFT_SEG_SYMB & 0x0F))  // 0x80  | 0x14

#define LCD_SEG_2_A     (SOFT_SEG_2 | SOFT_SEG_A)              // 0x70  | 0x06
#define LCD_SEG_2_F     (SOFT_SEG_2 | SOFT_SEG_F)              // 0x70  | 0x00
#define LCD_SEG_2_E     (SOFT_SEG_2 | SOFT_SEG_E)              // 0x70  | 0x02
#define LCD_SEG_2_D     (SOFT_SEG_2 | SOFT_SEG_D)              // 0x70  | 0x04

#define LCD_SEG_2_B     (SOFT_SEG_3 | (SOFT_SEG_B & 0x0F))     // 0x60  | 0x16
#define LCD_SEG_2_G     (SOFT_SEG_3 | (SOFT_SEG_G & 0x0F))     // 0x60  | 0x10
#define LCD_SEG_2_C     (SOFT_SEG_3 | (SOFT_SEG_C & 0x0F))     // 0x60  | 0x12
#define LCD_SEG_BAT     (SOFT_SEG_3 | (SOFT_SEG_SYMB & 0x0F))  // 0x60  | 0x14

#define LCD_SEG_3_A     (SOFT_SEG_4 | SOFT_SEG_A)              // 0x50  | 0x06
#define LCD_SEG_3_F     (SOFT_SEG_4 | SOFT_SEG_F)              // 0x50  | 0x00
#define LCD_SEG_3_E     (SOFT_SEG_4 | SOFT_SEG_E)              // 0x50  | 0x02
#define LCD_SEG_3_D     (SOFT_SEG_4 | SOFT_SEG_D)              // 0x50  | 0x04

#define LCD_SEG_3_B     (SOFT_SEG_5 | (SOFT_SEG_B & 0x0F))     // 0x40  | 0x16
#define LCD_SEG_3_G     (SOFT_SEG_5 | (SOFT_SEG_G & 0x0F))     // 0x40  | 0x10
#define LCD_SEG_3_C     (SOFT_SEG_5 | (SOFT_SEG_C & 0x0F))     // 0x40  | 0x12
#define LCD_SEG_DOT1    (SOFT_SEG_5 | (SOFT_SEG_SYMB & 0x0F))  // 0x40  | 0x14

#define LCD_SEG_4_A     (SOFT_SEG_6 | SOFT_SEG_A)              // 0x30  | 0x06
#define LCD_SEG_4_F     (SOFT_SEG_6 | SOFT_SEG_F)              // 0x30  | 0x00
#define LCD_SEG_4_E     (SOFT_SEG_6 | SOFT_SEG_E)              // 0x30  | 0x02
#define LCD_SEG_4_D     (SOFT_SEG_6 | SOFT_SEG_D)              // 0x30  | 0x04

#define LCD_SEG_4_B     (SOFT_SEG_8 | (SOFT_SEG_B & 0x0F))     // 0x10  | 0x16
#define LCD_SEG_4_G     (SOFT_SEG_8 | (SOFT_SEG_G & 0x0F))     // 0x10  | 0x10
#define LCD_SEG_4_C     (SOFT_SEG_8 | (SOFT_SEG_C & 0x0F))     // 0x10  | 0x12
#define LCD_SEG_M       (SOFT_SEG_8 | (SOFT_SEG_SYMB & 0x0F))  // 0x10  | 0x14

#define LCD_SEG_5_A     (SOFT_SEG_9 | SOFT_SEG_A)              // 0x00  | 0x06
#define LCD_SEG_5_F     (SOFT_SEG_9 | SOFT_SEG_F)              // 0x00  | 0x00
#define LCD_SEG_5_E     (SOFT_SEG_9 | SOFT_SEG_E)              // 0x00  | 0x02
#define LCD_SEG_5_D     (SOFT_SEG_9 | SOFT_SEG_D)              // 0x00  | 0x04

#define LCD_SEG_5_B     (SOFT_SEG_7 | (SOFT_SEG_B & 0x0F))       // 0x20  | 0x16
#define LCD_SEG_5_G     (SOFT_SEG_7 | (SOFT_SEG_G & 0x0F))       // 0x20  | 0x10
#define LCD_SEG_5_C     (SOFT_SEG_7 | (SOFT_SEG_C & 0x0F))       // 0x20  | 0x12
//#define LCD_SEG_DOT2    (SOFT_SEG_9 | (SOFT_SEG_SYMB & 0x0F))  // 0x20  | 0x14

#define GPIO_SEG_0      0x00
#define GPIO_SEG_1      0x01

#define GPIO_COM_0      0x0A
#define GPIO_COM_1      0x0B
#define GPIO_COM_2      0x0C
#define GPIO_COM_3      0x0D

#define DRVLCD_CLEAR_ALL	0
#define DRVLCD_FILL_ALL		1


// USER PROTOTYPES

void drvLCD_LCDbyGPIO_COM_Handler(unsigned char CADRNum, unsigned char InCADRNum);
void drvLCD_LCDbyGPIO_SEG_Handler(unsigned char CADRNum, unsigned char InCADRNum, unsigned char SelectPin);
void drvLCD_ShiftRegisterHandler(unsigned char CADRNum, unsigned char InCADRNum);
unsigned char drvLCD_LCDbyGPIO_InCadr_Handler(unsigned char InCadr, unsigned char state);
void drvLCD_Set_Segment(unsigned char segmentID);
void drvLCD_Reset_Segment(unsigned char segmentID);
void drvLCD_Clear_Segment (unsigned char segmentID);
void drvLCD_UpdateIndicationTable (unsigned char segmentID, unsigned char tablePlace1, unsigned char tablePlace2);
void drvLCD_SetCadrParts (unsigned char partsNumber);
void drvLCD_Set_GPIO(unsigned char GPIO_ID, unsigned char state);
void drvLCD_SetAllSegments(unsigned char value);
void drvLCD_ClearAllSegments(void);
void drvLCD_InitLCD(void);
void drvLCD_DeinitLCD(void);


#ifdef	__cplusplus
}
#endif

#endif  /*__DRV_LCD_H__*/

