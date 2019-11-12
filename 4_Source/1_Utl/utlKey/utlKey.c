#include "utlKey.h"
#include "../../2_Drv/drvKey/drvKey.h" 
#include "../../2_Drv/drvSysClock/drvSysClock.h" 

typedef struct{
    unsigned long   holdTimeStart;
    unsigned short  holdTimePeriod;
    unsigned long   releasedTimeStart;
    unsigned char   keyMode;
    unsigned long   releaseTimeOut;

}T_KEY_STRUCT;

static unsigned char KeyModeType = 0;

static T_KEY_STRUCT keyboard[DRVKEY_NUMBER_KEY];
//SI_SEGMENT_VARIABLE(keyboard[DRVKEY_NUMBER_KEY], T_KEY_STRUCT, SI_SEG_XDATA);
//SI_LOCATED_VARIABLE_NO_INIT(keyboard[DRVKEY_NUMBER_KEY], T_KEY_STRUCT, SI_SEG_XDATA);

void utlKey_Init(void)
{
    unsigned char i;// = 0;

    utlKey_StartKeyModeSLEEP();
    utlKey_KeyHoldTimeSLEEP();

    for (i = 0; i < DRVKEY_NUMBER_KEY; i++)
    {
        keyboard[i].holdTimeStart       = 0;
        keyboard[i].keyMode             = UTLKEY_STATE_NOT_PUSHED;
    }
}


void utlKey_StartKeyModeSLEEP(void)
{
	KeyModeType = 0;
	drvKey_InitModeSLEEP();
}

void utlKey_StartKeyModeWORK(void)
{
	KeyModeType = 1;
	drvKey_InitModeWORK();
}

void utlKey_KeyHoldTimeSLEEP(void)
{
	unsigned char i;

    for (i = 0; i < DRVKEY_NUMBER_KEY; i++)
    {
        keyboard[i].holdTimePeriod      = UTLKEY_SLEEP_HOLD_TIME_PERIOD;
        keyboard[i].releaseTimeOut      = UTLKEY_SLEEP_RELEASED_TIMEOUT;
    }
}

void utlKey_KeyHoldTimeACTIVE(void)
{
	unsigned char i;

    for (i = 0; i < DRVKEY_NUMBER_KEY; i++)
    {
        keyboard[i].holdTimePeriod      = UTLKEY_DEFAULT_HOLD_TIME_PERIOD;
        keyboard[i].releaseTimeOut      = UTLKEY_DEFAULT_RELEASED_TIMEOUT;
    }
}



void utlKey_KeyDebounceHandler(void)
{
    if (KeyModeType)
    {
    	drvKey_HandlerModeWORK();
    }
    else
    {
    	drvKey_HandlerModeSLEEP();
    }
}

unsigned char utlKey_Handler(unsigned char keyID)
{
    unsigned char keyBounce;
    unsigned char nextState;
    unsigned char returnValue;

    nextState = keyboard[keyID].keyMode;

	keyBounce = drvKey_getBounceCounter(keyID);

	switch(keyboard[keyID].keyMode)
	{
		case UTLKEY_STATE_NOT_PUSHED :
			if (keyBounce > UTLKEY_COUNTER_VALUE_ON)
			{
				nextState = UTLKEY_STATE_PUSHED;
			}
			break;
		case UTLKEY_STATE_PUSHED :
			keyboard[keyID].holdTimeStart = 0;
			nextState = UTLKEY_STATE_HOLD_;
			break;
		case UTLKEY_STATE_HOLD_ :
			if (keyBounce > UTLKEY_COUNTER_VALUE_ON)
			{
				keyboard[keyID].holdTimeStart++;
				if (keyboard[keyID].holdTimeStart > keyboard[keyID].holdTimePeriod)
				{
					nextState = UTLKEY_STATE_HOLDED_TIME_1;
				}
			}

			if (keyBounce < UTLKEY_COUNTER_VALUE_OFF)
			{
				keyboard[keyID].releasedTimeStart = 0;
				nextState = UTLKEY_STATE_RELEASE;
			}
			break;
		case UTLKEY_STATE_HOLDED_TIME_1 :
			keyboard[keyID].holdTimeStart = 0;
			nextState = UTLKEY_STATE_HOLD_;
			break;
		case UTLKEY_STATE_RELEASE :
			keyboard[keyID].releasedTimeStart++;
			if (keyboard[keyID].releasedTimeStart > keyboard[keyID].releaseTimeOut)
			{
				nextState = UTLKEY_STATE_RELEASED_;
			}
			break;
		case UTLKEY_STATE_RELEASED_ :

			keyboard[keyID].releasedTimeStart = 0;
			nextState = UTLKEY_STATE_NOT_PUSHED;
			break;
		default :
			keyboard[keyID].keyMode = UTLKEY_STATE_NOT_PUSHED;
			break;
	}

	returnValue = keyboard[keyID].keyMode;

	keyboard[keyID].keyMode = nextState;

	return returnValue;
 }

