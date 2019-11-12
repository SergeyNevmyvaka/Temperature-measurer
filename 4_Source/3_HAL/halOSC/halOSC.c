/*
 * halOSC.c
 */	

#include "halOSC.h"
#include "SI_EFM8SB2_Register_Enums.h"



void halOSC_InitLPO(void)
{

	CLKSEL = CLKSEL_CLKDIV__SYSCLK_DIV_4 | CLKSEL_CLKSL__LPOSC;
    
    while ((CLKSEL & CLKSEL_CLKRDY__BMASK) != CLKSEL_CLKRDY__SET);
    
   	HFO0CN &= ~HFO0CN_IOSCEN__ENABLED;
   	REG0CN &= ~REG0CN_OSCBIAS__FORCE_ENABLED;
}

void halOSC_InitHFO(unsigned char divider)
{
    
    REG0CN = REG0CN_OSCBIAS__FORCE_ENABLED;
        
    HFO0CN |= HFO0CN_IOSCEN__ENABLED;
    
    while ((HFO0CN & HFO0CN_IFRDY__BMASK) == HFO0CN_IFRDY__NOT_SET);
    
    CLKSEL = divider | CLKSEL_CLKSL__HFOSC;
	// Wait for the clock to be ready
    while ((CLKSEL & CLKSEL_CLKRDY__BMASK) != CLKSEL_CLKRDY__SET);
}

