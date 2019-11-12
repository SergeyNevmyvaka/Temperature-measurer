/******************************************************************************
 * Copyright (c) 2014 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#ifndef __DRV_USART_H__
#define __DRV_USART_H__

#ifdef	__cplusplus
extern "C" {
#endif

#include "SI_EFM8SB2_Register_Enums.h"
#include "../../3_HAL/halUSART/halUSART.h"

#define DRVUSART_RX_FLAG        0x01
#define DRVUSART_TIMEOUT_FLAG   0x10


void drvUSART_InitUART(SI_VARIABLE_SEGMENT_POINTER(buffer, uint8_t, EFM8PDL_UART0_RX_BUFTYPE), uint8_t length);


#ifdef	__cplusplus
}
#endif

#endif //__DRV_USART_H__



