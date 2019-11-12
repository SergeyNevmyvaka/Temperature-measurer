//-----------------------------------------------------------------------------
// power.h
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// Driver for the  power management function.
//
// Target:         EFM8SB1
// Tool chain:     Generic
// Command Line:   None
//
// Release 1.0
//    - Initial Revision (FB)
//    - 19 MAY 2010
//

//-----------------------------------------------------------------------------
// Register and Bit Definitions
//-----------------------------------------------------------------------------
#ifndef HAL_POWER_H_
#define HAL_POWER_H_

#ifdef	__cplusplus
extern "C" {
#endif


// Power Management Bit Definitions
#define SLEEP        0x80              // Sleep Mode Select
#define SUSPEND      0x40              // Suspend Mode Select
#define CLEAR        0x20              // Wake-Up Flag Clear
#define RSTWK        0x10              // Reset Pin Falling Edge Wake-Up
#define RTCFWK       0x08              // SmaRTClock Failure Wake-Up
#define RTCAWK       0x04              // SmaRTClock Alarm Wake-Up
#define PMATWK       0x02              // Port Match Wake-Up
#define CMP0WK       0x01              // Comparator0 Wake-Up

// Friendly names for the LPM function arguments
#define PORT_MATCH   PMATWK
#define RTC          RTCAWK// + RTCFWK
#define COMPARATOR   CMP0WK

// FLSCL Bit Definition
#define BYPASS    0x40
#define NON_ZERO  0x01

//-----------------------------------------------------------------------------
// Extern Variable Definitions
//-----------------------------------------------------------------------------

extern unsigned char PMU0CF_Local;

extern unsigned char RTC_Alarm;
extern unsigned char RTC_Failure;
extern unsigned char Comparator_Wakeup;
extern unsigned char Port_Match_Wakeup;

//-----------------------------------------------------------------------------
// Extern Function PROTOTYPES
//-----------------------------------------------------------------------------

extern void LPM_Init (void);
extern void LPM(unsigned char mode);
extern void LPM_Enable_Wakeup (unsigned char wakeup);
extern void LPM_Disable_Wakeup (unsigned char wakeup);


#ifdef	__cplusplus
}
#endif

#endif /* HAL_POWER_H_ */
