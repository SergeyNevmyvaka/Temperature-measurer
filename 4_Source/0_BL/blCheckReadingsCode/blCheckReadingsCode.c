
#include "blCheckReadingsCode.h"

#include "../../0_BL/blStatusControl/blStatusControl.h"


unsigned char blCheckReadingsCode_CalculateReadings (unsigned long currReadings)
{
	unsigned char returnValue;
	unsigned int step1;
	unsigned int step2;
	unsigned int step3;
	unsigned int step4;
	unsigned int step5;
	unsigned int buff;
	unsigned int SUM;

	step1 = (currReadings >> 16) & 0x0F;

	buff = (currReadings >> 8) & 0xFF;
	step2 = buff / 2;

	step3 = (buff * 2) % 256;

	step4 = currReadings & 0xFF;
	step4 = step4 / 4;

	step5 = currReadings & 0x0F;

	SUM = step1 + step2 + step3 + step4 + step5;

	returnValue = SUM;

    return  returnValue;
}

unsigned char blCheckReadingsCode_CalculateControlReadings (unsigned long currReadings)
{
	unsigned char returnValue;
	unsigned int step1;
	unsigned int step2;
	unsigned int step3;
	unsigned int step4;
	unsigned int step5;
	unsigned int buff;
	unsigned int SUM;

	step1 = (currReadings >> 16) & 0x0F;

	buff = (currReadings >> 8) & 0xFF;
	step2 = buff / 4;

	step3 = (buff * 2) % 256;

	step4 = (currReadings & 0xFF) / 2;

	step5 = currReadings & 0x0F;

	SUM = step1 + step2 + step3 + step4 + step5;

	returnValue = SUM;

    return  returnValue;
}

unsigned char blCheckReadingsCode_CalculateDate (unsigned char currDay, unsigned char currMonth)
{
	unsigned char returnValue;
	unsigned int step1;
	unsigned int step2;
	unsigned int step3;
	unsigned int step4;
	unsigned int SUM;

	step1 = currDay;
	step1 = step1 / 4;

	step2 = currDay & 0x0F;

	step3 = currMonth;
	step3 = step3 / 2;

	step4 = currMonth & 0x0F;
	step4 = step4 * 2;

	SUM = step1 + step2 + step3 + step4;

	returnValue = SUM;

    return  returnValue;
}


void blCheckReadingsCode_Handler(S_DEVICE *device) // TODO: проверить расстановку полубайт
{
	unsigned char codeMass[6];
	unsigned char tempCode;
	unsigned char i;

	tempCode = blCheckReadingsCode_CalculateReadings((unsigned long)(device->COUNTING_RATE + 0.5));
	codeMass[5] = tempCode & 0x0F;
	codeMass[3] = tempCode >> 4;

	tempCode = blCheckReadingsCode_CalculateControlReadings((unsigned long)device->CONTROL_COUNTING_RATE);
	codeMass[2] = tempCode & 0x0F;
	codeMass[0] = tempCode >> 4;

	tempCode = blCheckReadingsCode_CalculateDate(device->CURRENT_DAY, device->CURRENT_MONTH);
	codeMass[4] = tempCode & 0x0F;
	codeMass[1] = tempCode >> 4;


	device->CHECK_READINGS_CODE = 0; //1//1150  /2//8555  /3//20.02  /=>//0x007E3A08

	for (i = 0; i < 6; i++)
		device->CHECK_READINGS_CODE |= (unsigned long)codeMass[i] << (i * 4);
}






