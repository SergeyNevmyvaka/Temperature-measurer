#include "drvROM.h"
#include "../../3_HAL/halFlash/halFlash.h"


void drvROM_Init(void)
{
    halFlash_InitFlash();
}

void drvROM_Deinit(void)
{
    halFlash_DeinitFlash();
}


////////////////////////////////////////////////////////////////////////////////
void drvROM_WriteBenchmarkData(unsigned short BufferSize, unsigned char *ptrBuffer){
       
    halFlash_Erase_page();
    halFlash_Write(FLASH_BASE_ADDR, ptrBuffer, BufferSize);
}


void drvROM_ReadBenchmarkData(unsigned short BufferSize, unsigned char *ptrBuffer){
    
    halFlash_Read(FLASH_BASE_ADDR, ptrBuffer, BufferSize);
}

////////////////////////////////////////////////////////////////////////////////



