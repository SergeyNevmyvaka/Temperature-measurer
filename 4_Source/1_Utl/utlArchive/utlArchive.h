
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

#ifndef UTL_ARCHIVE_H
#define	UTL_ARCHIVE_H

#ifdef	__cplusplus
extern "C" {
#endif

#define ARCH_ARCHIVE_SIZE           212//132//118//110

#define ARCH_DATA_BYTE_1            0       //LOW
#define ARCH_DATA_BYTE_2            1       //MID_LOW
#define ARCH_DATA_BYTE_3            2       //MID_HIGH
#define ARCH_DATA_BYTE_4            3       //HIGH

#define ARCH_DATA_ID_R              0
#define ARCH_DATA_ID_DATE           4
#define ARCH_DATA_ID_STATUS         8

/******************************************************************************/
//------------------------------------------
#define ARCH_DATA_MONTH_1_R         0
#define ARCH_DATA_MONTH_1_DATE      4
#define ARCH_DATA_MONTH_1_STATUS    8

#define ARCH_DATA_MONTH_2_R         12
#define ARCH_DATA_MONTH_2_DATE      16
#define ARCH_DATA_MONTH_2_STATUS    20

#define ARCH_DATA_MONTH_3_R         24
#define ARCH_DATA_MONTH_3_DATE      28
#define ARCH_DATA_MONTH_3_STATUS    32

#define ARCH_DATA_MONTH_4_R         36
#define ARCH_DATA_MONTH_4_DATE      40
#define ARCH_DATA_MONTH_4_STATUS    44

#define ARCH_DATA_MONTH_5_R         48
#define ARCH_DATA_MONTH_5_DATE      52
#define ARCH_DATA_MONTH_5_STATUS    56

#define ARCH_DATA_MONTH_6_R         60
#define ARCH_DATA_MONTH_6_DATE      64
#define ARCH_DATA_MONTH_6_STATUS    68

#define ARCH_DATA_MONTH_7_R         72
#define ARCH_DATA_MONTH_7_DATE      76
#define ARCH_DATA_MONTH_7_STATUS    80

#define ARCH_DATA_MONTH_8_R         84
#define ARCH_DATA_MONTH_8_DATE      88
#define ARCH_DATA_MONTH_8_STATUS    92

#define ARCH_DATA_MONTH_9_R         96
#define ARCH_DATA_MONTH_9_DATE      100
#define ARCH_DATA_MONTH_9_STATUS    104

#define ARCH_DATA_MONTH_10_R        108
#define ARCH_DATA_MONTH_10_DATE     112
#define ARCH_DATA_MONTH_10_STATUS   116

#define ARCH_DATA_MONTH_11_R        120
#define ARCH_DATA_MONTH_11_DATE     124
#define ARCH_DATA_MONTH_11_STATUS   128

#define ARCH_DATA_MONTH_12_R        132
#define ARCH_DATA_MONTH_12_DATE     136
#define ARCH_DATA_MONTH_12_STATUS   140

//------------------------------------------
#define ARCH_DATA_ENABLES_NUMBER    144

#define ARCH_DATA_CURRENT_MODE      148

#define ARCH_DATA_SN                152

#define ARCH_DATA_CONFIG_DATE       156

#define ARCH_DATA_K1                160
#define ARCH_DATA_K2                164
#define ARCH_DATA_K3                168
#define ARCH_DATA_K4                172

#define ARCH_DATA_BREAK_R           176
#define ARCH_DATA_BREAK_DATE        180
#define ARCH_DATA_BREAK_STATUS      184

#define ARCH_DATA_CONTROL_R         188
#define ARCH_DATA_CONTROL_DATE      192
#define ARCH_DATA_CONTROL_STATUS    196

#define ARCH_DATA_CURRENT_MONTH     200

#define ARCH_DATA_DELETE_ARCHIVE_COUNTER 204

#define ARCH_DATA_TAMPER_STATE      208

//----------------------------------------
// end at 204//132


//END 106+12=118
    
/******************************************************************************/

typedef union {
    unsigned long  Long_Value;
    unsigned char  Byte_Value[4];
} U_ARCH_4BYTE_UNION;



void utlArchive_ReadArchiveArray(void);

void utlArchive_WriteArchiveArray(void);

void utlArchive_WriteDataInArray(unsigned long ByteData, unsigned char dataPlace);

unsigned long utlArchive_ReadDataFromArray(unsigned char dataPlace);



#ifdef	__cplusplus
}
#endif

#endif	/* UTL_ARCHIVE_H */

