/*
 * halTimer.c
 */	

#include "halTimer.h"
//Standard device includes
#include "SI_EFM8SB2_Register_Enums.h"

#define TIMER3_CPRTR_FLAG_SET        0
#define TIMER3_ALRM_FLAG_SET         0
#define TIMER3_ALRM_FLAG_RESET       1
#define TIMER3_CPRTR_FLAG_ERROR      0//2


#define TIMER3_DISCHARGE_TIME_FOR_ERROR     5//15   через 13,5 мс считаем что обрыв датчика

static unsigned char alarmFlag = TIMER3_ALRM_FLAG_RESET;
static unsigned char alarmCount = 0;
static unsigned char compareFlag = TIMER3_ALRM_FLAG_RESET;

static unsigned char captureValueHigh = 0;
static unsigned char captureValueLow = 0;

#define TIMER3_TMR3RL_VALUE_H 0x58 // переполнение через 1.75 мс при частоте 24.5 ћ√ц
#define TIMER3_TMR3RL_VALUE_L 0x65
void halTimer3_InitAlarm (void)
{
    // Stop Timer
    TMR3CN0 &= ~(TMR3CN0_TR3__BMASK);
    // [Timer Initialization]$

    TMR3CN0 &= ~TMR3CN0_T3XCLK__FMASK;
    TMR3CN0 = 0;//|= TMR3CN0_T3XCLK__SYSCLK_DIV_12_CAP_CMP1 | TMR3CN0_T3SPLIT__16_BIT_RELOAD;
    
    TMR3H = 0x00;
    TMR3L = 0x00;

   	TMR3RLH = TIMER3_TMR3RL_VALUE_H; // переполнение через 1.75 мс при частоте 24.5 ћ√ц
    TMR3RLL = TIMER3_TMR3RL_VALUE_L;

    TMR3CN0 &= ~(TMR3CN0_TF3H__SET | TMR3CN0_TF3L__SET);
    
    // [Timer Restoration]$

    CKCON0 |= CKCON0_T3MH__EXTERNAL_CLOCK | CKCON0_T3ML__SYSCLK;

    IE |= IE_EA__ENABLED;
    EIE1 |= EIE1_ET3__ENABLED;
}

void halTimer3_InitCapture (void)
{
    // $[Timer Initialization]
    // Stop Timer
    TMR3CN0 &= ~(TMR3CN0_TR3__BMASK);
    // [Timer Initialization]$

    // $[TMR2CN0 - Timer 2 Control]
    /*
     // TF2CEN (Timer 2 Capture Enable) = ENABLED (Enable capture mode.)
     // T2XCLK (Timer 2 External Clock Select) = SYSCLK_DIV_12_CAP_CMP
     //     (External Clock is SYSCLK/12. Capture trigger is Comparator 0.)
     */
	TMR3CN0 &= ~TMR3CN0_T3XCLK__FMASK;
	TMR3CN0 |= TMR3CN0_TF3CEN__ENABLED | TMR3CN0_T3XCLK__SYSCLK_DIV_12_CAP_CMP1;

    // [TMR2CN0 - Timer 2 Control]$
    TMR3H = 0x00;
    TMR3L = 0x00;

   	TMR3RLH = 0x00;
    TMR3RLL = 0x00;

    CKCON0 |= CKCON0_T3MH__EXTERNAL_CLOCK | CKCON0_T3ML__SYSCLK;

    IE |= IE_EA__ENABLED;
    EIE1 |= EIE1_ET3__ENABLED;
}

void halTimer3_Start (void)
{
    TMR3CN0 |= TMR3CN0_TR3__RUN;
}

void halTimer3_Stop (void)
{
    TMR3CN0 &= ~TMR3CN0_TR3__RUN;
}


unsigned short halTimer3_GetCaptureValue (void)
{
    unsigned short captureValue = 0;

    captureValueHigh = TMR3RLH;
    captureValueLow = TMR3RLL;

    captureValue = captureValueHigh;
    captureValue = captureValue << 8;
    captureValue |= captureValueLow;
            
    return captureValue;
}


void halTimer3_Deinit (void)
{
    // $[Timer Initialization]
    
    TMR3CN0 &= ~TMR3CN0_T3XCLK__FMASK;

    // Stop Timer
    TMR3CN0 = TMR3CN0_TR3__STOP;
    
    EIE1 &= ~EIE1_ET3__ENABLED;
}

unsigned char halTimer3_GetAlarmFlag (void)
{
    return alarmFlag;
}
unsigned char halTimer3_GetCompareFlag (void)
{
    return compareFlag;
}
unsigned char halTimer3_GetAlarmCount (void)
{
    return alarmCount;
}
void halTimer3_ResetAlarmFlag (void)
{
    alarmFlag = TIMER3_ALRM_FLAG_RESET;
    compareFlag = TIMER3_ALRM_FLAG_RESET;
    alarmCount = 0;
}

//-----------------------------------------------------------------------------
// TIMER3_ISR
//-----------------------------------------------------------------------------
//
// TIMER3 ISR Content goes here. Remember to clear flag bits:
// TMR3CN0::TF3H (Timer # High Byte Overflow Flag)
// TMR3CN0::TF3L (Timer # Low Byte Overflow Flag)
//
//-----------------------------------------------------------------------------
SI_INTERRUPT (TIMER3_ISR, TIMER3_IRQn)
{
	TCON &= ~TCON_TR0__RUN;

	captureValueHigh = TMR3RLH;
	captureValueLow = TMR3RLL;

	if (CMP1CN0 & CMP1CN0_CPRIF__RISING_EDGE)
	{
		TMR3CN0 &= ~TMR3CN0_TF3H__SET;
		TMR3CN0 &= ~TMR3CN0_TR3__RUN;
		compareFlag = TIMER3_CPRTR_FLAG_SET;
	}
	else if (TMR3CN0 & TMR3CN0_TF3H__SET)
	{
		alarmCount++;
		TMR3CN0 &= ~TMR3CN0_TF3H__SET;
	}

	if (alarmCount >= TIMER3_REF_CAP_CHARGE_TIME)
	{
		alarmFlag = TIMER3_ALRM_FLAG_SET;
	}
}




void halTimer0_InitOverflowCount (void)
{
	//Stop Timers
	TCON |= 0x50;//TCON &= ~TCON_TR0__BMASK & ~TCON_TR1__BMASK;

	TH0 = 0x00;
	TL0 = 0x00;

	CKCON0 |= CKCON0_T0M__SYSCLK;

	TMOD |= TMOD_T0M__MODE1 | TMOD_CT0__TIMER | TMOD_GATE0__DISABLED;
}


void halTimer0_Start (void)
{
	IE |= (IE_EA__ENABLED | IE_ET0__ENABLED);
	TCON |= TCON_TR0__RUN;
}

void halTimer0_Stop (void)
{
	TCON &= ~TCON_TR0__RUN;
    IE &= ~IE_ET0__ENABLED;
}


//-----------------------------------------------------------------------------
// TIMER0_ISR
//-----------------------------------------------------------------------------
//
// TIMER0 ISR Content goes here. Remember to clear flag bits:
// TCON::TF0 (Timer 0 Overflow Flag)
//
//-----------------------------------------------------------------------------
SI_INTERRUPT (TIMER0_ISR, TIMER0_IRQn)
{

	TCON &= ~TCON_TR0__RUN;
	TCON &= ~TCON_TF0__SET;

	TH0 = 0x00;
	TL0 = 0x00;

	alarmCount++;

	TCON |= TCON_TR0__RUN;

	if (alarmCount >= TIMER3_DISCHARGE_TIME_FOR_ERROR)
	{
		compareFlag = TIMER3_CPRTR_FLAG_ERROR;
		TCON &= ~TCON_TR0__RUN;
	}
}


