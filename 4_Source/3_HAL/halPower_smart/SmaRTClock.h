
//-----------------------------------------------------------------------------
// SmaRTClock.h
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
// Release 1.0
//    - Initial Revision (FB)
//    - 19 MAY 2010
//

//-----------------------------------------------------------------------------
// RTC Configuration Options
//-----------------------------------------------------------------------------

#ifndef __HAL_SMART_CLOCK_H__
#define __HAL_SMART_CLOCK_H__

#ifdef	__cplusplus
extern "C" {
#endif

#define SELFOSC 0
#define CRYSTAL 1


// SmaRTClock Internal Registers
#define CAPTURE0  0x00                 // RTC address of CAPTURE0 register
#define CAPTURE1  0x01                 // RTC address of CAPTURE1 register
#define CAPTURE2  0x02                 // RTC address of CAPTURE2 register
#define CAPTURE3  0x03                 // RTC address of CAPTURE3 register
#define RTC0CN0   0x04                 // RTC address of RTC0CN0 register
#define RTC0XCN   0x05                 // RTC address of RTC0XCN register
#define RTC0XCF   0x06                 // RTC address of RTC0XCF register
#define RTC0PIN   0x07                 // RTC address of RTC0PIN register
#define ALARM0    0x08                 // RTC address of ALARM0 register
#define ALARM1    0x09                 // RTC address of ALARM1 register
#define ALARM2    0x0A                 // RTC address of ALARM2 register
#define ALARM3    0x0B                 // RTC address of ALARM3 register

// SmaRTClock Bit Definitions
#define RTC0CAP   0x01
#define RTC0SET   0x02
#define ALRM      0x04
#define RTC0AEN   0x08
#define RTC0TR    0x10
#define OSCFAIL   0x20
#define MCLKEN    0x40
#define RTC0EN    0x80

#define RTC_SLEEP_BITS		(RTC0TR+RTC0AEN+ALRM)

//-----------------------------------------------------------------------------
// Variables
//-----------------------------------------------------------------------------
extern unsigned char RTC0CN0_Local;                 // Holds the desired RTC0CN0 settings
 
//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------
extern unsigned char   RTC_Read (unsigned char reg);
extern void RTC_Write (unsigned char reg, unsigned char value);
extern void RTC0CN0_SetBits(unsigned char bits);
extern void RTC0CN0_ClearBits(unsigned char bits);
extern void halRTC_InitSleepRTC(void);


#ifdef	__cplusplus
}
#endif

#endif   /*__HAL_SMART_CLOCK_H__*/





