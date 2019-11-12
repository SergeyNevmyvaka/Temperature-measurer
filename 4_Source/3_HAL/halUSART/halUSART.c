/**************************************************************************//**
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#include "halUSART.h"
#include "../../1_Utl/utlModBus/utlModBus.h"

#define SET_PAGE(p)     do                                                    \
                        {                                                     \
                          savedPage = SFRPAGE;  /* save current SFR page */   \
                          SFRPAGE = (p);        /* set SFR page */            \
                        } while(0)
// exit autopage section
#define RESTORE_PAGE    do                                                    \
                        {                                                     \
                          SFRPAGE = savedPage;  /* restore saved SFR page */  \
                        } while(0)


void UART0_init(UART0_RxEnable_t rxen, UART0_Width_t width, UART0_Multiproc_t mce)
{


  P0MDOUT |= P0MDOUT_B4__PUSH_PULL;
  P0MDOUT &= ~P0MDOUT_B5__PUSH_PULL;
  P0MASK |= P0MASK_B5__COMPARED;
  P0SKIP &= ~(P0SKIP_B5__SKIPPED | P0SKIP_B5__SKIPPED);

  XBR2 = 0x40;//XBR2_WEAKPUD__PULL_UPS_ENABLED | XBR2_XBARE__ENABLED;
  XBR0 |= XBR0_URT0E__ENABLED;

//  // $[Timer Initialization]
  	TCON &= ~TCON_TR1__BMASK;
//
//   	// $[CKCON0 - Clock Control 0]

	CKCON0 = 0x4C; //CKCON0_SCA__SYSCLK_DIV_12 | CKCON0_T0M__PRESCALE
			//| CKCON0_T2MH__EXTERNAL_CLOCK | CKCON0_T2ML__EXTERNAL_CLOCK
			//| CKCON0_T3MH__EXTERNAL_CLOCK | CKCON0_T3ML__SYSCLK
			//| CKCON0_T0M__SYSCLK | CKCON0_T1M__SYSCLK;

///	CKCON0 |= CKCON0_SCA__SYSCLK_DIV_4;
///	CKCON0 |= CKCON0_T1M__PRESCALE;

	TMOD = 0x20;//TMOD_T0M__MODE0 | TMOD_T1M__MODE2 | TMOD_CT0__TIMER
  			//| TMOD_GATE0__DISABLED | TMOD_CT1__TIMER | TMOD_GATE1__DISABLED;

///	TH1 = 0xB0;
	TH1 = (0x60 << TH1_TH1__SHIFT);
	TL1 = (0x2C << TL1_TL1__SHIFT);

	TCON |= TCON_TR1__RUN;

  	// $[IE - Interrupt Enable]
  	IE |= IE_EA__ENABLED | IE_ES0__ENABLED;

    // Enable the Flash read one-shot timer
    FLSCL &= ~BYPASS;                   // Clear the one-shot bypass bit
    IP  =   NON_ZERO;                 // Write a "dummy" value to IP for a 3-byte
                                      // instruction

  	// init MCU clocking
//    CLKSEL = 0x24;//CLKSEL_CLKDIV__SYSCLK_DIV_4 | CLKSEL_CLKSL__LPOSC;
//    while ((CLKSEL & CLKSEL_CLKRDY__BMASK) != CLKSEL_CLKRDY__SET);

//  	REG0CN = REG0CN_OSCBIAS__FORCE_ENABLED;
    HFO0CN |= HFO0CN_IOSCEN__ENABLED;
    while ((HFO0CN & HFO0CN_IFRDY__BMASK) == HFO0CN_IFRDY__NOT_SET);
    CLKSEL = CLKSEL_CLKDIV__SYSCLK_DIV_8 | CLKSEL_CLKSL__HFOSC;
    while ((CLKSEL & CLKSEL_CLKRDY__BMASK) != CLKSEL_CLKRDY__SET);

///  	HFO0CN &= ~HFO0CN_IOSCEN__ENABLED;
///   	REG0CN &= ~REG0CN_OSCBIAS__FORCE_ENABLED;

    SCON0 &= ~0xB0;//(SCON0_SMODE__BMASK | SCON0_MCE__BMASK | SCON0_REN__BMASK);
    SCON0 = mce | rxen | width;

}

//=========================================================
// Interrupt API
//=========================================================

/**
 * Internal variable for trackig buffer transfers. transferLenth[UART0_TX_TRANSFER] = bytes remaining in transfer.
 */
SI_SEGMENT_VARIABLE(txRemaining, static uint8_t,  SI_SEG_XDATA)=0;
SI_SEGMENT_VARIABLE(rxRemaining, static uint8_t,  SI_SEG_XDATA)=0;
SI_SEGMENT_VARIABLE_SEGMENT_POINTER(txBuffer,    static uint8_t, EFM8PDL_UART0_TX_BUFTYPE, SI_SEG_XDATA);
SI_SEGMENT_VARIABLE_SEGMENT_POINTER(rxBuffer,    static uint8_t, EFM8PDL_UART0_RX_BUFTYPE, SI_SEG_XDATA);


SI_INTERRUPT(UART0_ISR, UART0_IRQn)
{
  uint8_t flags;
//  DECL_PAGE;
//  SET_PAGE(0x00);
  //Buffer and clear flags immediately so we don't miss an interrupt while processing
  flags = SCON0 & (UART0_RX_IF | UART0_TX_IF);
  SCON0 &= ~flags;

  if (rxRemaining && (flags &  SCON0_RI__SET))
  {
    *rxBuffer = SBUF0;
    ++rxBuffer;
    --rxRemaining;
    if (!rxRemaining)
    {
      UART0_receiveCompleteCb();
    }
  }

  if ((flags & SCON0_TI__SET))
  {
    if (txRemaining){
      SBUF0 = *txBuffer;
      ++txBuffer;
      --txRemaining;
    }
    else
    {
      UART0_transmitCompleteCb();
    }
  }
//  RESTORE_PAGE;
}

void UART0_writeBuffer(SI_VARIABLE_SEGMENT_POINTER(buffer, uint8_t, EFM8PDL_UART0_TX_BUFTYPE), uint8_t length)
{
  txBuffer = buffer;
  txRemaining = length;

  //Send initial byte
  SBUF0 = *buffer;
  txRemaining--;
  ++txBuffer;
}

void UART0_readBuffer(SI_VARIABLE_SEGMENT_POINTER(buffer, uint8_t, EFM8PDL_UART0_RX_BUFTYPE), uint8_t length)
{
  //Init internal data
  rxBuffer = buffer;
  rxRemaining = length;
}

