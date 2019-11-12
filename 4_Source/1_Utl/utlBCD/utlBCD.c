#include "utlBCD.h"

/**
 * �������������� ���� ����� � ������� BC�, ����������� � ����� �����, � HEX ������.
 * @param BCD - ���� � ����� ������� � ������� BCD
 * @return - ��������� �������������� � ������� HEX.
 */
unsigned char BCD_toHEX8(unsigned char BCD)
{
    return (BCD >> 4)*10 + (BCD & 0x0F);
}

/**
 * �������������� ����� (HEX 1 byte) � BCD
 * @param HEX - ����� ������� ����� �������������
 * @return - ��������� ��������������
 */
unsigned char HEX8_toBCD8(unsigned char HEX)
{
        return ((HEX / 10) << 4) | (HEX % 10);
}

