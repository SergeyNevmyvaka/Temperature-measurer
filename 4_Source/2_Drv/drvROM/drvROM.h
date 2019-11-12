
/** 
 * File:   Archive.h
 * Author: s.aksenov
 * e-mail: Aksonov.Sergei@gmail.com
 *
 * Created on 16 Ќо¤брь 2011 г., 8:18
 *
 * 
 *
 */

#ifndef DRV_ROM_H
#define	DRV_ROM_H

#ifdef	__cplusplus
extern "C" {
#endif


void drvROM_Init(void);

void drvROM_Deinit(void);

void drvROM_WriteBenchmarkData(unsigned short BufferSize, unsigned char *ptrBuffer);


void drvROM_ReadBenchmarkData(unsigned short BufferSize, unsigned char *ptrBuffer);


#ifdef	__cplusplus
}
#endif

#endif	/* DRV_ROM_H */

