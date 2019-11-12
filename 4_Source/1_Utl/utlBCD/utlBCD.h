
/* 
 * File:   BCD.h
 * Author: s.aksenov
 * e-mail: Aksonov.Sergei@gmail.com
 *
 * Created on 7 Декабрь 2011 г., 8:27
 *
 * 
 *
 */

#ifndef UTL_BCD_H
#define	UTL_BCD_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    /**
     * Преобразование двух чисел в формате BCВ, размещенных в одном байте, в HEX формат.
     * @param BCD - байт с двумя числами в формате BCD
     * @return - результат преобразования в формате HEX.
     */
    unsigned char BCD_toHEX8(unsigned char BCD);
    
    /**
     * Преобразование числа (HEX 1 byte) в BCD
     * @param HEX - число которое нужно преобразовать
     * @return - результат преобразования
     */
    unsigned char HEX8_toBCD8(unsigned char HEX);
    

#ifdef	__cplusplus
}
#endif

#endif	/* BCD_H */

