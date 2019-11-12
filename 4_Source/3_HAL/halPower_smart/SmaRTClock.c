//-----------------------------------------------------------------------------
// SmaRTClock.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// Driver for the SmaRTClock peripheral.
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
unsigned char RTC0CN0_Local;                       // Holds the desired RTC0CN0 settings
 
//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

unsigned char   RTC_Read (unsigned char reg);
void RTC_Write (unsigned char reg, unsigned char value);
void RTC0CN0_SetBits(unsigned char bits);

//-----------------------------------------------------------------------------
// RTC_Read ()
//-----------------------------------------------------------------------------
//
// Return Value : RTC0DAT
// Parameters   : 1) uint8_t reg - address of RTC register to read
//
// This function will read one byte from the specified RTC register.
// Using a register number greater that 0x0F is not permited.
//
//  Timing of SmaRTClock register read with short bit set
//
//     mov      RTC0ADR, #094h
//     nop
//     nop
//     nop
//     mov      A, RTC0DAT
//
//-----------------------------------------------------------------------------
unsigned char RTC_Read (unsigned char reg)
{

   RTC0ADR  = (0x90 | reg);            // pick register and set BUSY to 
                                       // initiate the read
                              
   NOP(); NOP(); NOP();                // delay 3 system clocks
   
   return RTC0DAT;                     // return value
}

//-----------------------------------------------------------------------------
// RTC_Write ()
//-----------------------------------------------------------------------------
//
// Return Value : none
// Parameters   : 1) uint8_t reg - address of RTC register to write
//                2) uint8_t value - the value to write to <reg>
//
// This function will Write one byte to the specified RTC register.
// Using a register number greater that 0x0F is not permited.
//
// Timing of SmaRTClock register write with short bit set
//
//       mov      RTC0ADR, #014h
//       mov      RTC0DAT, #088h
//       nop
//
//-----------------------------------------------------------------------------
void RTC_Write (unsigned char reg, unsigned char value)
{
   RTC0ADR  = (0x10 | reg);            // pick register
   RTC0DAT = value;                    // write data
}



//-----------------------------------------------------------------------------
// RTC0CN0_SetBits()
//-----------------------------------------------------------------------------
//
// Return Value : none 
// Parameters   : 
//
// This function sets bits in the RTC0CN0 register
//-----------------------------------------------------------------------------
void RTC0CN0_SetBits(unsigned char bits)
{
   RTC0CN0_Local |= (bits & ~0x03);
   RTC_Write( RTC0CN0, RTC0CN0_Local | bits);
}



void halRTC_InitSleepRTC(void)
{
	// $[RTC Initialization]
	// A variable for providing a delay for external oscillator startup
	uint16_t delayCounter;
	// Save the system clock (the system clock will be slowed during the startup delay)
	uint8_t CLKSEL_save;
	CLKSEL_save = CLKSEL;

	// Delay for ~ 0.5 s  // ждем пока пройдет дребезг по питанию
	for (delayCounter = 0x2000; delayCounter != 0; delayCounter--)
		;

	// Unlock the RTC Interface
	RTC0KEY = RTC0KEY_RTC0ST__KEY1;
	RTC0KEY = RTC0KEY_RTC0ST__KEY2;

	// Enable power to the SmaRTClock oscillator circuit (RTC0EN = 1)
	// [RTC Initialization]$

	// $[RTC0XCN0 - RTC Oscillator Control: Initial start-up setting]
	// Set SmaRTClock to Crystal Mode (XMODE = 1).
	// Disable Automatic Gain Control (AGCEN) and enable Bias Doubling (BIASX2) for fast crystal startup.
	RTC0ADR = RTC0XCN0;
	RTC0DAT = RTC0XCN0_XMODE__CRYSTAL | RTC0XCN0_BIASX2__ENABLED;
	while ((RTC0ADR & RTC0ADR_BUSY__BMASK) == RTC0ADR_BUSY__SET)
		;    //Poll Busy Bit
	// [RTC0XCN0 - RTC Oscillator Control: Initial start-up setting]$

	// $[RTC0XCF - RTC Oscillator Configuration]
	/*
	 // AUTOSTP (Automatic Load Capacitance Stepping Enable) = ENABLED (Enable
	 //     load capacitance stepping.)
	 // LOADCAP (Load Capacitance Programmed Value) = 0x0E
	 */
	RTC0ADR = RTC0XCF;
	RTC0DAT = RTC0XCF_AUTOSTP__ENABLED | (0x0E << RTC0XCF_LOADCAP__SHIFT); // 12.5 pF
	while ((RTC0ADR & RTC0ADR_BUSY__BMASK) == RTC0ADR_BUSY__SET)
		;    //Poll Busy Bit
	// [RTC0XCF - RTC Oscillator Configuration]$


	// просыпаемся раз в (4 m - 2 ticks)
	RTC0ADR = ALARM0;
	RTC0DAT = (0xFE << ALARM0_ALARM0__SHIFT);
	while ((RTC0ADR & RTC0ADR_BUSY__BMASK) == RTC0ADR_BUSY__SET);
	RTC0ADR = ALARM1;
	RTC0DAT = (0xFF << ALARM1_ALARM1__SHIFT);
	while ((RTC0ADR & RTC0ADR_BUSY__BMASK) == RTC0ADR_BUSY__SET);
	RTC0ADR = ALARM2;
	RTC0DAT = (0x77 << ALARM2_ALARM2__SHIFT);
	while ((RTC0ADR & RTC0ADR_BUSY__BMASK) == RTC0ADR_BUSY__SET);

	// $[RTC0CN - RTC Control]
	/*
	 // RTC0EN (RTC Enable) = ENABLED (Enable RTC oscillator.)
	 */
	RTC0ADR = RTC0CN0;
	RTC0DAT = 0;
	while ((RTC0ADR & RTC0ADR_BUSY__BMASK) == RTC0ADR_BUSY__SET)
		;    //Poll Busy Bit

	RTC0ADR = RTC0CN0;
	RTC0DAT |= RTC0CN0_RTC0EN__ENABLED;
	while ((RTC0ADR & RTC0ADR_BUSY__BMASK) == RTC0ADR_BUSY__SET)
		;    //Poll Busy Bit

	// [RTC0CN - RTC Control]$

	// $[External Oscillator Setup]
	// Set the clock to a known value for the delay
	CLKSEL = CLKSEL_CLKSL__LPOSC | CLKSEL_CLKDIV__SYSCLK_DIV_128;

	// Delay for > 20 ms
	for (delayCounter = 0x150; delayCounter != 0; delayCounter--)
		;

	// Poll the SmaRTClock Clock Valid Bit (CLKVLD) until the crystal oscillator stabilizes
	do {
		RTC0ADR = RTC0ADR_BUSY__SET | RTC0ADR_ADDR__RTC0XCN0;
		while ((RTC0ADR & RTC0ADR_BUSY__BMASK) == RTC0ADR_BUSY__SET)
			;    //Poll Busy Bit
	} while ((RTC0DAT & RTC0XCN0_CLKVLD__BMASK) == RTC0XCN0_CLKVLD__NOT_SET);

	// Poll the SmaRTClock Load Capacitance Ready Bit (LOADRDY) until the load capacitance reaches its programmed value
	do {
		RTC0ADR = RTC0ADR_BUSY__SET | RTC0ADR_ADDR__RTC0XCF;
		while ((RTC0ADR & RTC0ADR_BUSY__BMASK) == RTC0ADR_BUSY__SET)
			;    //Poll Busy Bit
	} while ((RTC0DAT & RTC0XCF_LOADRDY__BMASK) == RTC0XCF_LOADRDY__NOT_SET);

	// Enable Automatic Gain Control (AGCEN) and disable Bias Doubling (BIASX2) for maximum power savings
	/*
	 // XMODE (RTC Oscillator Mode) = CRYSTAL (Crystal Mode selected.)
	 // AGCEN (RTC Oscillator Automatic Gain Control (AGC) Enable) = ENABLED
	 //     (Enable AGC.)
	 // BIASX2 (RTC Oscillator Bias Double Enable) = DISABLED (Disable the
	 //     Bias Double feature.)
	 */
	RTC0ADR = RTC0XCN0;
	RTC0DAT = RTC0XCN0_XMODE__CRYSTAL | RTC0XCN0_AGCEN__ENABLED
			| RTC0XCN0_BIASX2__DISABLED;
	while ((RTC0ADR & RTC0ADR_BUSY__BMASK) == RTC0ADR_BUSY__SET)
		;    //Poll Busy Bit

	// Enable the SmaRTClock missing clock detector.
	/*
	 // MCLKEN (Missing RTC Detector Enable) = ENABLED (Enable missing RTC
	 //     detector.)
	 */
	RTC0ADR = RTC0CN0;
	RTC0DAT |= RTC0CN0_MCLKEN__ENABLED;
	while ((RTC0ADR & RTC0ADR_BUSY__BMASK) == RTC0ADR_BUSY__SET)
		;    //Poll Busy Bit

	// Delay for > 2 ms
	for (delayCounter = 0x100; delayCounter != 0; delayCounter--)
		;

	// Clear PMU wake-up source flags
	PMU0CF = PMU0CF_CLEAR__ALL_FLAGS;

	// Restore system clock
	CLKSEL = CLKSEL_save;
	// Poll CLKRDY to wait for the clock to stabilize
	while (!((CLKSEL & CLKSEL_CLKRDY__BMASK) == CLKSEL_CLKRDY__SET))
		;

	/*
	 // RTC0EN (RTC Enable) = ENABLED (Enable RTC oscillator.)
	 // RTC0TR (RTC Timer Run Control) = RUN (RTC timer is running.)
	 // MCLKEN (Missing RTC Detector Enable) = ENABLED (Enable missing RTC
	 //     detector.)
	 // RTC0AEN (RTC Alarm Enable) = DISABLED (Disable RTC alarm.)
	 // ALRM (RTC Alarm Event Flag and Auto Reset Enable) = SET (Alarm event
	 //     flag is set or enable the auto reset function.)
	 // RTC0CAP (RTC Timer Capture) = NOT_SET (Do not start a capture
	 //     operation.)
	 // RTC0SET (RTC Timer Set) = NOT_SET (Do not start a set operation.)
	 */
	RTC0ADR = RTC0CN0;
	RTC0DAT = RTC0CN0_RTC0EN__ENABLED | RTC0CN0_RTC0TR__RUN
			| RTC0CN0_MCLKEN__ENABLED | RTC0CN0_RTC0AEN__DISABLED
			| RTC0CN0_ALRM__SET | RTC0CN0_RTC0CAP__NOT_SET
			| RTC0CN0_RTC0SET__NOT_SET;
	while ((RTC0ADR & RTC0ADR_BUSY__BMASK) == RTC0ADR_BUSY__SET)
		;    //Poll Busy Bit

	// [External Oscillator Setup]$

	// Enable Interrupt for RTC in active mode
	EIE1  |= EIE1_ERTC0A__ENABLED;
	EIP1 |= EIP1_PRTC0A__HIGH;

}


//#endif


