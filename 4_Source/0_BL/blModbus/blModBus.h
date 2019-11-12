
/** 
 * File:   ModBus.h
 * Author: s.aksenov
 * e-mail: Aksonov.Sergei@gmail.com
 *
 * Created on 31 ќкт€брь 2011 г., 9:07
 *
 * 
 *
 */

#ifndef MODBUS_H
#define	MODBUS_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "../blStatusControl/blStatusControl.h"

#define STORAGE_SIZE        BL_STATUS_CONTROL_SHORT_NUMBER//39

unsigned char blModbus_Handler(S_DEVICE *device);


#ifdef	__cplusplus
}
#endif

#endif	/* MODBUS_H */

