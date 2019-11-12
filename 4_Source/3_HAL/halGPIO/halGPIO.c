/*
 * halGPIO.c
 */	

//Standard device includes
#include "SI_EFM8SB2_Register_Enums.h"
 
#include "halGPIO.h"


// маленькие специализированные функции вместо больших универсальных
// специализация - помодульная

// P_0_2
void halGPIO_SensPinUnused(void)
{
	P0MDIN |= P0MDIN_B2__DIGITAL;
	P0MDOUT &= ~P0MDOUT_B2__PUSH_PULL;
	P0_B2 = PIN_ON;
}

// P_0_0
void halGPIO_RefPinUnused(void)
{
	P0MDIN |= P0MDIN_B0__DIGITAL;
	P0MDOUT &= ~P0MDOUT_B0__PUSH_PULL;
	P0_B0 = PIN_ON;
}

// P_0_1
void halGPIO_CompPinUnused(void)
{
    P0MDIN |= P0MDIN_B1__DIGITAL;
    P0MDOUT &= ~P0MDOUT_B1__PUSH_PULL;
    P0_B1 = PIN_ON;
}

// P_0_0
void halGPIO_RefPinCharge(void)
{
    P0MDIN |= P0MDIN_B0__DIGITAL;
    P0MDOUT |= P0MDOUT_B0__PUSH_PULL;
	P0_B0 = PIN_ON;

	SFRPAGE = 0x0F;
    P0DRV |= P0DRV_B0__HIGH_DRIVE | P0DRV_B1__HIGH_DRIVE | P0DRV_B2__HIGH_DRIVE;
    SFRPAGE = 0x00;
}

// P_0_1
void halGPIO_CompPinAnalog(void)
{
    P0MDIN &= ~P0MDIN_B1__DIGITAL;
    P0MDOUT &= ~P0MDOUT_B1__PUSH_PULL;
    P0_B1 = PIN_ON;
    P0SKIP |= P0SKIP_B1__SKIPPED;
}

// P_1_5
void halGPIO_ButtModePinAnalog(void)
{
    P1MDIN &= ~P0MDIN_B5__DIGITAL;
    P1MDOUT &= ~P1MDOUT_B5__PUSH_PULL;
    P1_B5 = PIN_OFF;
    P1SKIP |= P1SKIP_B5__SKIPPED;
}

// P_0_3
void halGPIO_TamperPinAnalog(void)
{
    P0MDIN &= ~P0MDIN_B3__DIGITAL;
    P0MDOUT &= ~P0MDOUT_B3__PUSH_PULL;
    P0_B3 = PIN_OFF;

    P1MDIN &= ~P0MDIN_B6__DIGITAL;
    P1MDOUT &= ~P1MDOUT_B6__PUSH_PULL;
    P1_B6 = PIN_OFF;
}

// P_1_5
void halGPIO_ModeButtPinIn(void)
{
    P1MDIN |= P1MDIN_B5__DIGITAL;
    P1MDOUT &= ~P1MDOUT_B5__PUSH_PULL;
    P1_B5 = PIN_ON;
}

// P_0_3
void halGPIO_TamperPinIn(void)
{
    P0MDIN |= P0MDIN_B3__DIGITAL;
    P0MDOUT &= ~P0MDOUT_B3__PUSH_PULL;
    P0_B3 = PIN_ON;

    P1MDIN |= P1MDIN_B6__DIGITAL;
    P1MDOUT &= ~P1MDOUT_B6__PUSH_PULL;
    P1_B6 = PIN_ON;
}

void halGPIO_InitPortMatchPinMode(void)
{
    P1MASK |= P1MASK_B5__COMPARED;
    P1MAT |= P1MAT_B5__HIGH;

    P0MASK &= ~P0MASK_B3__COMPARED;
}

void halGPIO_InitPortMatchPinTamper(void)
{
    P0MASK |= P0MASK_B3__COMPARED;
    P0MAT |= P0MAT_B3__HIGH;
}

// P_1_5
unsigned char halGPIO_ModeButtPinGetState(void)
{
    unsigned char returnValue;

    if (P1_B5 == PIN_ON)
    	returnValue = PIN_ON;
    else
    	returnValue = PIN_OFF;

    return returnValue;
}

// P_0_3
unsigned char halGPIO_TamperPinGetState(void)
{
    unsigned char returnValue;

    if (P0_B3 == PIN_OFF)
    	returnValue = PIN_ON;
    else
    	returnValue = PIN_OFF;

    return returnValue;
}

void halGPIO_EnableWeakUp(void)
{
    XBR2 &= ~XBR2_WEAKPUD__PULL_UPS_DISABLED;

}

void halGPIO_DisableWeakUp(void)
{
    XBR2 |= XBR2_WEAKPUD__PULL_UPS_DISABLED;

}


