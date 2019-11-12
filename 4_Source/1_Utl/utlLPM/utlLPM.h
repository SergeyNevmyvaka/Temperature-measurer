/*
 * utlLPM.h
 */

#ifndef UTL_LPM_H_
#define UTL_LPM_H_

#ifdef	__cplusplus
extern "C" {
#endif


#define UTLPWR_WAKE_EVENT_COMPARATOR_RE 0x01   /// Wake-up from a Comparator 0 rising edge.


#define UTLPWR_WAKE_EVENT_PORT_MATCH    0x02   /// Wake-up from a Port Match event.


#define UTLPWR_WAKE_EVENT_RTC_ALARM     0x04   /// Wake-up from an RTC Alarm.


#define UTLPWR_WAKE_EVENT_RTC_OSC_FAIL  0x08   /// Wake-up from an RTC oscillator fail event.


#define UTLPWR_WAKE_EVENT_RESET         0x10   /// Wake-up from a glitch detected on RSTb.


void utlLPM_InitSleep_work(void);

void utlLPM_FallAsleep(void);

unsigned char utlLPM_WakingUp(void);


#ifdef	__cplusplus
}
#endif

#endif /* UTL_LPM_H_ */

	
