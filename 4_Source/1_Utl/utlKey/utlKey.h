/*
 * modKey.h
 */

#ifndef UTLKEY_H_
#define UTLKEY_H_

#ifdef	__cplusplus
extern "C" {
#endif

#define UTLKEY_STATE_NOT_PUSHED		0
#define UTLKEY_STATE_PUSHED			1
#define UTLKEY_STATE_HOLD_			2
#define UTLKEY_STATE_HOLDED_TIME_1	3
#define UTLKEY_STATE_RELEASE		4
#define UTLKEY_STATE_RELEASED_		5

#define UTLKEY_COUNTER_VALUE_ON         4
#define UTLKEY_COUNTER_VALUE_OFF        2

#define UTLKEY_DEFAULT_HOLD_TIME_PERIOD    500//2000
#define UTLKEY_DEFAULT_RELEASED_TIMEOUT    10

#define UTLKEY_SLEEP_HOLD_TIME_PERIOD     2
#define UTLKEY_SLEEP_RELEASED_TIMEOUT     1

#define UTLKEY_BREAK_KEY         0
#define UTLKEY_MODE_KEY          1

void utlKey_Init(void);

unsigned char utlKey_getState(unsigned char keyNumber);

void utlKey_KeyDebounceHandler(void);

unsigned char utlKey_Handler(unsigned char keyID);

void utlKey_StartKeyModeSLEEP(void);

void utlKey_StartKeyModeWORK(void);

void utlKey_KeyHoldTimeSLEEP(void);

void utlKey_KeyHoldTimeACTIVE(void);

#ifdef	__cplusplus
}
#endif

#endif /* UTLKEY_H_ */

