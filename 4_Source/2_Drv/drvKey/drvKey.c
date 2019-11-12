#include "drvKey.h"
#include "../../3_HAL/halGPIO/halGPIO.h"


#define DRVKEY_PIN_STATE_ACTIVE         0


unsigned char bounce_counter[DRVKEY_NUMBER_KEY] = {DRVKEY_COUNTER_VALUE_MIN, DRVKEY_COUNTER_VALUE_MIN};
unsigned char pin_state[DRVKEY_NUMBER_KEY];


void drvKey_HandlerModeSLEEP (void)
{
    drvKey_TamperOn();

    pin_state[DRVKEY_TAMPER_KEY] = halGPIO_TamperPinGetState();    // P_0_3
    pin_state[DRVKEY_MODE_KEY] = halGPIO_ModeButtPinGetState();  // P_1_5

    drvKey_TamperOff();

    drvKey_contactBounce(DRVKEY_TAMPER_KEY, pin_state[DRVKEY_TAMPER_KEY]);

    drvKey_contactBounce(DRVKEY_MODE_KEY, pin_state[DRVKEY_MODE_KEY]);
}

void drvKey_HandlerModeWORK (void)
{
    pin_state[DRVKEY_TAMPER_KEY] = halGPIO_TamperPinGetState();    // P_0_3
    pin_state[DRVKEY_MODE_KEY] = halGPIO_ModeButtPinGetState();  // P_1_5

    drvKey_contactBounce(DRVKEY_TAMPER_KEY, pin_state[DRVKEY_TAMPER_KEY]);
    drvKey_contactBounce(DRVKEY_MODE_KEY, pin_state[DRVKEY_MODE_KEY]);

}

void drvKey_InitModeSLEEP(void)
{
	// P_1_5
	halGPIO_ModeButtPinIn();

	halGPIO_InitPortMatchPinMode();

	drvKey_TamperOn();
}


void drvKey_InitModeWORK(void)
{
	// P_1_5
	halGPIO_ModeButtPinIn();

	halGPIO_InitPortMatchPinMode();

	drvKey_TamperOn();

	halGPIO_InitPortMatchPinTamper();
}

void drvKey_TamperOn(void)
{
	unsigned char i;
	// P_0_3
	halGPIO_TamperPinIn();

	for (i = 0; i < 10; i++);
}

void drvKey_TamperOff(void)
{
	// P_0_3
	halGPIO_TamperPinAnalog();
}


void drvKey_contactBounce(unsigned char keyNumber, unsigned char pinState)
{
	if (pinState == DRVKEY_PIN_STATE_ACTIVE)
	{
		if (bounce_counter[keyNumber] < DRVKEY_COUNTER_VALUE_MAX)
			bounce_counter[keyNumber]++;
	}
	else
	{
		if (bounce_counter[keyNumber] > DRVKEY_COUNTER_VALUE_MIN)
			bounce_counter[keyNumber]--;

	}
}

unsigned char drvKey_getBounceCounter (unsigned char keyNumber)
{
    return bounce_counter[keyNumber];
}


