/*
 * drvKey.h
 */

#ifndef DRVKEY_H_
#define DRVKEY_H_

#ifdef	__cplusplus
extern "C" {
#endif

#define DRVKEY_NUMBER_KEY                   2

#define DRVKEY_COUNTER_VALUE_MAX        6//10
#define DRVKEY_COUNTER_VALUE_MIN        1

#define DRVKEY_TAMPER_KEY         0
#define DRVKEY_MODE_KEY         1



void drvKey_InitModeSLEEP(void);

void drvKey_InitModeWORK(void);

void drvKey_TamperOn(void);

void drvKey_TamperOff(void);

void drvKey_contactBounce(unsigned char keyNumber, unsigned char pinState);

void drvKey_HandlerModeSLEEP(void);

void drvKey_HandlerModeWORK(void);

unsigned char drvKey_getBounceCounter (unsigned char keyNumber);


#ifdef	__cplusplus
}
#endif

#endif /* DRVKEY_H_ */
