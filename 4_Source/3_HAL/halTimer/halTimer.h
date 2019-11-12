

#ifndef __HAL_TIMER_H__
#define __HAL_TIMER_H__

#ifdef	__cplusplus
extern "C" {
#endif


#define TIMER3_REF_CAP_CHARGE_TIME          3//4//charge through refcap 18//7tau    //13//5tau


void halTimer3_InitAlarm (void);

void halTimer3_InitCapture (void);

void halTimer3_Start (void);

void halTimer3_Stop (void);

unsigned char halTimer3_GetAlarmFlag (void);

unsigned char halTimer3_GetCompareFlag (void);

unsigned char halTimer3_GetAlarmCount (void);

void halTimer3_ResetAlarmFlag (void);

unsigned short halTimer3_GetCaptureValue (void);

void halTimer3_Deinit(void);

void halTimer0_InitOverflowCount (void);

void halTimer0_Start (void);

void halTimer0_Stop (void);


#ifdef	__cplusplus
}
#endif

#endif //__HAL_TIMER_H__



