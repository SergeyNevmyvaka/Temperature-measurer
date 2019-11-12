#include "utlAverageTemp.h"
    

float utlAverageTemp_Calculation (float tempr[])
{
	unsigned char   i;// ������� �����������

	float  Max = tempr[0];
	float  Min = tempr[0];
	float  AverageResist   = 0;

	for (i = 0; i < AVERAGE_BUFFER_SIZE; i++)
	{
		// �������� ����� � ���������� min � max

			if (tempr[i] > Max)
				Max = tempr[i];

			if (tempr[i] < Min)
				Min = tempr[i];

		AverageResist += tempr[i];
	}

	// ��������� ����� ������������ min � max
	AverageResist = (AverageResist - Max - Min) / AVERAGE_BUFFER_DIVIDER;

	return AverageResist;
}
