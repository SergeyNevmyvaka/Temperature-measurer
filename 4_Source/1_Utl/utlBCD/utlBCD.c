#include "utlBCD.h"

/**
 * Преобразование двух чисел в формате BCВ, размещенных в одном байте, в HEX формат.
 * @param BCD - байт с двумя числами в формате BCD
 * @return - результат преобразования в формате HEX.
 */
unsigned char BCD_toHEX8(unsigned char BCD)
{
    return (BCD >> 4)*10 + (BCD & 0x0F);
}

/**
 * Преобразование числа (HEX 1 byte) в BCD
 * @param HEX - число которое нужно преобразовать
 * @return - результат преобразования
 */
unsigned char HEX8_toBCD8(unsigned char HEX)
{
        return ((HEX / 10) << 4) | (HEX % 10);
}

