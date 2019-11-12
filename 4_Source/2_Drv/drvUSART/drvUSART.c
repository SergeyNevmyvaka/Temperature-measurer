/**************************************************************************//**
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#include "drvUSART.h"
#include "../../3_HAL/halUSART/halUSART.h"
#include "../../3_HAL/halTimer/halTimer.h"



void drvUSART_InitUART(SI_VARIABLE_SEGMENT_POINTER(buffer, uint8_t, EFM8PDL_UART0_RX_BUFTYPE), uint8_t length)
{
	UART0_init(SCON0_REN__RECEIVE_ENABLED, SCON0_SMODE__8_BIT, SCON0_MCE__MULTI_DISABLED);
    UART0_readBuffer(buffer, length); // включить прием
}




