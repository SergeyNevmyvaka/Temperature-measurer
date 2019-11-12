//-----------------------------------------------------------------------------
// power.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// Driver for the power management function.
//
// Target:         EFM8SB1
// Tool chain:     Generic
// Command Line:   None
//
// Release 1.0 (BL)
//    - Initial Release
//    - 9 JAN 2015
//
// Release 1.0
//    - Initial Revision (FB)
//    - 19 MAY 2010
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "SI_EFM8SB2_Register_Enums.h"
#include "smartclock.h"                // RTC Functionality
#include "power.h"                     // Power Management Functionality
 
//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

// Variables used for the RTC interface
unsigned char PMU0CF_Local;                       // Holds the desired Wake-up sources
 
// Define Wake-Up Flags
unsigned char RTC_Alarm;
unsigned char RTC_Failure;
unsigned char Comparator_Wakeup;
unsigned char Port_Match_Wakeup;

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------
void LPM(unsigned char mode);
void LPM_Init(void);
void LPM_Enable_Wakeup (unsigned char wakeup);

//-----------------------------------------------------------------------------
// LPM_Init ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function will initialize the low power functionality
//
//-----------------------------------------------------------------------------
void LPM_Init (void)
{
  PMU0CF = PMU0CF_CLEAR__ALL_FLAGS;
  PMU0CF_Local = 0;
  RTC_Alarm = 0;
  RTC_Failure = 0;
  Comparator_Wakeup = 0;
  Port_Match_Wakeup = 0;
}

//-----------------------------------------------------------------------------
// LPM_Enable_Wakeup ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function will enable wakeup sources
//
//-----------------------------------------------------------------------------
void LPM_Enable_Wakeup (unsigned char wakeup)
{
  PMU0CF_Local |= (wakeup & (RTC | PORT_MATCH | COMPARATOR));
}


//-----------------------------------------------------------------------------
// LPM
//-----------------------------------------------------------------------------
//
// Return Value : none
// Parameters   : 1) uint8_t mode -- can be SLEEP or SUSPEND
//
// This function places the device in a low power mode
//
//-----------------------------------------------------------------------------
void LPM(unsigned char mode)
{
    unsigned char b;
    //unsigned char CLKSEL_save;
    //unsigned char EA_save;
    unsigned char PMU0CF_snapshot;
    unsigned char RTC0CN0_snapshot;
    unsigned char IP_save;
    //unsigned char CLKSEL_save;
   
   IE_EA = 0;
   IP_save = IP;
   
   // Save current system clock selection and select the low power oscillator
   // divided by 2 as the system clock
   //CLKSEL_save = CLKSEL;
//   CLKSEL = 0x24;
   
   // Enable the Flash read one-shot timer   
    FLSCL &= ~BYPASS;                 // Clear the one-shot bypass bit
    IP  =   NON_ZERO;                 // Write a "dummy" value to IP for a 3-byte
                                      // instruction
   // Verify that the system clock setting has been applied
//   while(!(CLKSEL & 0x80));            // Wait until CLKRDY -> 1

//    CLKSEL = CLKSEL_CLKDIV__SYSCLK_DIV_4 | CLKSEL_CLKSL__LPOSC;
//    while ((CLKSEL & CLKSEL_CLKRDY__BMASK) != CLKSEL_CLKRDY__SET);
//
//  	HFO0CN &= ~HFO0CN_IOSCEN__ENABLED;
//  	REG0CN &= ~REG0CN_OSCBIAS__FORCE_ENABLED;
    //for(b = 255; b > 0; b--);

   //----------------------------------
   // ~~~ Device in Low Power Mode ~~~
   //
       PMU0CF = PMU0CF_CLEAR__ALL_FLAGS;
       PMU0CF = (mode | PMU0CF_Local);
   //
   // ~~~   Device is now Awake    ~~~
   //----------------------------------
   
   // Restore the System Clock
   //CLKSEL = CLKSEL_save;


   // Capture the wake-up source and clear all wake-up source flags   
   PMU0CF_snapshot = PMU0CF;
   PMU0CF = PMU0CF_CLEAR__ALL_FLAGS;

   // Capture RTC events that occured while PMU0CF was being cleared
   RTC0CN0_snapshot = RTC_Read(RTC0CN0); 

   // If an RTC Alarm occured while PMU0CF was being cleared, clear 
   // the PCU0CF flag again   
   if((RTC0CN0_snapshot & ALRM) && ((PMU0CF_snapshot & RTCAWK) == 0))
   {
      PMU0CF_snapshot |= PMU0CF;
      PMU0CF = PMU0CF_CLEAR__ALL_FLAGS;
   }

   //----------------------------------
   // Decode Wake-Up Sources
   //----------------------------------

   // Check for an RTC Alarm
   if((PMU0CF_snapshot & RTCAWK) || (RTC0CN0_snapshot & ALRM))
   {
      RTC_Alarm = 1;
   }
   // Check for an RTC Clock Failure
   if((PMU0CF_snapshot & RTCFWK) || (RTC0CN0_snapshot & OSCFAIL))
   {
      RTC_Failure = 1;  
   }
   // Check for a Port Match Wakeup
   if(PMU0CF_snapshot & PMATWK)
   {
      Port_Match_Wakeup = 1;  
   }
   // Restore Interrupt Priorities
   IP = IP_save;

   // Restore Interrupt State
   IE_EA = 1;
   
   // Check for a reset pin Wakeup
   if(PMU0CF_snapshot & RSTWK)
   {
      // Delay 15uS per datasheet recommendation
      for(b = 255; b > 0; b--);
   }

//   REG0CN = REG0CN_OSCBIAS__FORCE_ENABLED;
//   HFO0CN |= HFO0CN_IOSCEN__ENABLED;
//	while ((HFO0CN & HFO0CN_IFRDY__BMASK) == HFO0CN_IFRDY__NOT_SET);
//
//	CLKSEL = CLKSEL_CLKDIV__SYSCLK_DIV_8 | CLKSEL_CLKSL__HFOSC;
//	// Wait for the clock to be ready
//	while ((CLKSEL & CLKSEL_CLKRDY__BMASK) != CLKSEL_CLKRDY__SET);

}               
