#include "utlArchive.h"
#include "../../2_Drv/drvROM/drvROM.h"
#include "si_toolchain.h"

SI_SEGMENT_VARIABLE(ArchiveArray[ARCH_ARCHIVE_SIZE], unsigned char, SI_SEG_XDATA);


void utlArchive_ReadArchiveArray(void)
{
    drvROM_Init();
    drvROM_ReadBenchmarkData(ARCH_ARCHIVE_SIZE, ArchiveArray);

    drvROM_Deinit();
}

void utlArchive_WriteArchiveArray(void)
{
    drvROM_Init();

    drvROM_WriteBenchmarkData(ARCH_ARCHIVE_SIZE, ArchiveArray);

    drvROM_Deinit();
}

void utlArchive_WriteDataInArray(unsigned long ByteData, unsigned char dataPlace)
{
	unsigned char i;

	for (i = 0; i < 4; i++)
		ArchiveArray[dataPlace + i] = ByteData >> (i*8);
}

unsigned long utlArchive_ReadDataFromArray(unsigned char dataPlace)
{
	unsigned char i;
	unsigned long retVal;

	retVal = 0;

	for (i = 0; i < 4; i++)
		retVal |= (unsigned long)ArchiveArray[dataPlace + i] << (i*8);

	return retVal;
}






  
