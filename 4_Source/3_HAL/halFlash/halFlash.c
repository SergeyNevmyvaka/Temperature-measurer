/**
 * @file flash.c
 * @brief Flash read/write/erase interface of EEPROM emulation.
 *
 * @date 2 Sep 2013
 * @version 1.0
 * @author Mark Ding
 *
 ******************************************************************************
 * @section License
 * <b>Copyright (c) 2013 by Silicon Laboratories. http://www.silabs.com</b>
 ******************************************************************************
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Silicon Laboratories End User
 * License Agreement which accompanies this distribution, and is available at
 * http://developer.silabs.com/legal/version/v10/License_Agreement_v10.htm
 * Original content and implementation provided by Silicon Laboratories.
 */
//#include "eeprom_config.h"
#include "halFlash.h"
#include "SI_EFM8SB2_Register_Enums.h"

#define RSTSRC_VAL          0x04

#define ENABLE_VDDMON()     VDM0CN |= VDM0CN_VDMEN__ENABLED;\
							NOP();\
							NOP();\
							NOP();\
                            RSTSRC = (RSTSRC_VAL | 0x02);

#define DISABLE_VDDMON()     VDM0CN |= VDM0CN_VDMEN__DISABLED;

#define SFRPAGE_SWITCH()    unsigned char pageSave = SFRPAGE; SFRPAGE = 0x00;

#define SFRPAGE_RESTORE()   SFRPAGE = pageSave;
#define FLASH_SAFE_ADDR    0xFFFF

#define FLASH_KEY_1    0xA5
#define FLASH_KEY_2    0xF1

/* Global variables*/

SI_SEGMENT_VARIABLE(flashAddress, static unsigned short, SI_SEG_DATA) = FLASH_SAFE_ADDR;

#define ENABLE_FL_MOD()    FLKEY  = FLASH_KEY_1; FLKEY  = FLASH_KEY_2;
#define DISABLE_FL_MOD()   


/* Internal Constants */
#define FL_WRITE        0x01        // PSCTL mask for Flash Writes
#define FL_ERASE        0x03        // PSCTL mask for Flash Erase


unsigned char EA_save;


void halFlash_InitFlash(void)
{
	// Save current interrupt state and disable interrupts
	EA_save = IE_EA;
	IE_EA = 0;
	IE_EA = 0;

    PSCTL |= 0x04;
}

void halFlash_DeinitFlash(void)
{
	PSCTL &= ~0x04;
    // Restore Interrupt State
	IE_EA = EA_save;
}


/**
 * @fn static void flash_write_erase(U16 address, U8 byte, U8 write_erase)
 * @brief This routine writes a byte or erases a page of Flash.
 *
 * @param address 16-bit address in code space to write/erase
 * @param byte data byte to write (value is don't care on erase)
 * @param write_erase 0x01 for writes, 0x03 to erase page
 *
 * @return none
 */
static void halFlash_Write_Erase(unsigned short startAddress, unsigned char ptrBuffer, unsigned char write_Erase)
{
    
    SI_SEGMENT_VARIABLE_SEGMENT_POINTER(pwrite, static unsigned char, SI_SEG_XDATA, SI_SEG_DATA);
    SFRPAGE_SWITCH()


    if ((startAddress > FLASH_TOP_ADDR) || (startAddress < FLASH_BASE_ADDR))
    {
        SFRPAGE_RESTORE()
        return;
    }
    ENABLE_VDDMON()

	flashAddress = startAddress;

    ENABLE_FL_MOD()
    /* setup PSEE, PSWE */

    pwrite = (unsigned char SI_SEG_XDATA *) flashAddress;

    PSCTL |= (write_Erase & 0x03);

    *pwrite = ptrBuffer;
    
    PSCTL &= ~0x03;

    DISABLE_FL_MOD()
    DISABLE_VDDMON()
    SFRPAGE_RESTORE()
}

void halFlash_Erase_page(void)
{
    halFlash_Write_Erase(1, 1, FL_ERASE);
}

void halFlash_Write(unsigned short address, unsigned char *ptrBuffer, unsigned char bufferSize)
{
	unsigned short i;

	for (i = 0; i < bufferSize; i++)
	{
	    halFlash_Write_Erase(address, *ptrBuffer, FL_WRITE);
	    ptrBuffer++;
	    address++;
	}
}

unsigned char halFlash_ReadByte(unsigned short address)
{
	unsigned char dat;
	dat = *((unsigned char SI_SEG_CODE *) address);
	return dat;
}

void halFlash_Read(unsigned short address, unsigned char *ptrBuffer, unsigned char bufferSize)
{
    unsigned short i;
    
    for (i = 0; i < bufferSize; i++)
    {
        ptrBuffer[i] = halFlash_ReadByte(address);
        address++;
    }
}
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
