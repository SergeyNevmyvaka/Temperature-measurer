/*
 * blArchive.h
 */

#ifndef BL_ARCHIVE_H_
#define BL_ARCHIVE_H_

#ifdef	__cplusplus
extern "C" {
#endif


#include "../blStatusControl/blStatusControl.h"

#define ARCH_MONTH_STORAGE_SIZE        132

#define ARCH_MONTH_DELETE_ARCH_VALUE   4294967283

void blArchive_Init(S_DEVICE *device);

void blArchive_WriteDataInArray(S_DEVICE *device, unsigned long ByteData, unsigned char dataPlace);

void blArchive_ReadDataFromArray(S_DEVICE *device, unsigned long *ByteData, unsigned char dataPlace);

void blArchive_Handler(S_DEVICE *device);

void blArchive_RestoreData(S_DEVICE *device);


#ifdef	__cplusplus
}
#endif

#endif /* BL_ARCHIVE_H_ */

