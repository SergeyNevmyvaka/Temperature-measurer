/*
 * utlIndication.h
 */

#ifndef UTL_INDICATION_H_
#define UTL_INDICATION_H_

#ifdef	__cplusplus
extern "C" {
#endif

#define UTL_INDICATION_SHOW_MODE_CODE        4
#define UTL_INDICATION_SHOW_MODE_T           3
#define	UTL_INDICATION_SHOW_MODE_ERROR       2
#define	UTL_INDICATION_SHOW_MODE_VALUE       1
#define	UTL_INDICATION_SHOW_MODE_DATE_TIME   0



//#define UTL_INDICATION_TIMEOUT_PERIOD         2500//6500      //10 s // ~30 s   //5MHz
#define UTL_INDICATION_TIMEOUT_PERIOD         585//650      /~9 s//~10 s    //3MHz

#define UTL_INDICATION_CHANGE_VOLTAGE         2.9    // Volts



void utlIndication_Init(float voltage);
void utlIndication_Deinit(void);
void utlIndication_SetCardFlag(void);
void utlIndication_Handler(void);
void utlIndication_32bitHexToBCD(unsigned char *BCD, unsigned long value);
void utlIndication_ShowValue(unsigned long value, unsigned char modeID);
void utlIndication_ShowR(float R);
void utlIndication_ShowDate(unsigned short date);//(unsigned char month, unsigned char year);
void utlIndication_ShowT(float T);
void utlIndication_ShowCCodeLow(unsigned long cod);
void utlIndication_ShowCCodeHigh(unsigned long cod);
void utlIndication_ShowError (unsigned long error);
void utlIndication_ShowLabelSLEEP(void);
void utlIndication_ShowLabelERR(void);
void utlIndication_ShowAllLCD(void);
void utlIndication_ClearAllLCD(void);
void utlIndication_ShowLabelArh(unsigned char currentArchive);

unsigned char utlIndication_ConvertPlaceIntoSEG(unsigned char place, unsigned char symbol);
void utlIndication_ShowNumeral_1(unsigned char place);
void utlIndication_ShowNumeral_2(unsigned char place);
void utlIndication_ShowNumeral_3(unsigned char place);
void utlIndication_ShowNumeral_4(unsigned char place);
void utlIndication_ShowNumeral_5(unsigned char place);
void utlIndication_ShowNumeral_6(unsigned char place);
void utlIndication_ShowNumeral_7(unsigned char place);
void utlIndication_ShowNumeral_8(unsigned char place);
void utlIndication_ShowNumeral_9(unsigned char place);
void utlIndication_ShowNumeral_0(unsigned char place);
void utlIndication_ShowNumeral_A(unsigned char place);
void utlIndication_ShowNumeral_B(unsigned char place);
void utlIndication_ShowNumeral_C(unsigned char place);
void utlIndication_ShowNumeral_D(unsigned char place);
void utlIndication_ShowNumeral_E(unsigned char place);
void utlIndication_ShowNumeral_F(unsigned char place);


void utlIndication_ClearPlace(unsigned char place);
void utlIndication_ShowSymbol_BAT(void);
void utlIndication_HideSymbol_BAT(void);
void utlIndication_ClearSymbol_BAT(void);
void utlIndication_ResetBAT_SEG(void);
void utlIndication_Clear_ADEF(unsigned char place);

void utlIndication_ShowSymbol_M(void);
void utlIndication_ShowSymbol_Tyre(unsigned char place);
void utlIndication_ShowSymbol_Lowcod(void);
void utlIndication_ShowSymbol_HighCod(void);

void utlIndication_StartTimeout(void);
unsigned char utlIndication_CheckTimeout(void);


#ifdef	__cplusplus
}
#endif

#endif /* UTL_INDICATION_H_ */

