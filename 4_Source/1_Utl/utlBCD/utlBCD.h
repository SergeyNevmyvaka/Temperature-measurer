
/* 
 * File:   BCD.h
 * Author: s.aksenov
 * e-mail: Aksonov.Sergei@gmail.com
 *
 * Created on 7 ������� 2011 �., 8:27
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
     * �������������� ���� ����� � ������� BC�, ����������� � ����� �����, � HEX ������.
     * @param BCD - ���� � ����� ������� � ������� BCD
     * @return - ��������� �������������� � ������� HEX.
     */
    unsigned char BCD_toHEX8(unsigned char BCD);
    
    /**
     * �������������� ����� (HEX 1 byte) � BCD
     * @param HEX - ����� ������� ����� �������������
     * @return - ��������� ��������������
     */
    unsigned char HEX8_toBCD8(unsigned char HEX);
    

#ifdef	__cplusplus
}
#endif

#endif	/* BCD_H */

