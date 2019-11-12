/*
 * drvLPM.h
 */

#ifndef DRV_LPM_H_
#define DRV_LPM_H_

#ifdef	__cplusplus
extern "C" {
#endif

#define DRVPWR_WAKE_EVENT_COMPARATOR_RE 0x01    /// Wake-up from a Comparator 0 rising edge.
#define DRVPWR_WAKE_EVENT_PORT_MATCH    0x02    /// Wake-up from a Port Match event.
#define DRVPWR_WAKE_EVENT_RTC_ALARM     0x04    /// Wake-up from an RTC Alarm.
#define DRVPWR_WAKE_EVENT_RTC_OSC_FAIL  0x08    /// Wake-up from an RTC oscillator fail event.
#define DRVPWR_WAKE_EVENT_RESET         0x10    /// Wake-up from a glitch detected on RSTb.


void drvLPM_InitPeriodicalSleep_4min(void);

void drvLPM_GoToSLEEP(void);

unsigned char drvLPM_WakeSource(void);


#ifdef	__cplusplus
}
#endif

#endif /* DRV_LPM_H_ */

	
