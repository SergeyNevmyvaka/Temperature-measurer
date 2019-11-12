/*
 * utlModBus.h
 */

#ifndef UTL_MODBUS_H_
#define UTL_MODBUS_H_

#ifdef	__cplusplus
extern "C" {
#endif

#define USART_MAX_RX_BUFFER_SIZE    7


#define MB_DEVICE_ADDRESS   0x01

#define ARCHIVE_REG_START   0x0100
#define ARCHIVE_REG_END     0x016C

#define MB_ADDRESS_BYTE     0
#define MB_FUNCTION_BYTE    1
#define MB_ERORCODE_BYTE    2
#define MB_REG_BYTE_HIGH    2
#define MB_REG_BYTE_LOW     3
#define MB_DATA_BYTE_HIGH   4
#define MB_DATA_BYTE_LOW    5
#define MB_LCR_BYTE         6       // байт в котором находится LCR пакета-запроса


#define MB_ExCODE_ILLEGAL_FUNCTION      0x01
#define MB_ExCODE_ILLEGAL_DATA_ADDRESS  0x02
#define MB_ExCODE_ILLEGAL_DATA_VALUE    0x03
#define MB_ExCODE_ILLEGAL_LCR           0xA7
#define MB_ExCODE_FUNCTION              0x08

//#define MB_TIMEOUT_PERIOD               2500       // 500 ms // 5MHz
#define MB_TIMEOUT_PERIOD               200//35//       // 500 ms // 3MHz

#define MB_STATUS_CLEAR         0x00
#define MB_STATUS_COMPLETE_RX   0x01
#define MB_STATUS_COMPLETE_TX   0x02
#define MB_STATUS_RX   			0x03
#define MB_STATUS_TX   			0x04

#define MB_FUNCTION_READ        0x03
#define MB_FUNCTION_WRITE       0x06


void utlModBus_Init(void);

void utlModBus_Reinit(void);

void utlModBus_StartRxTimeout(void);

unsigned char utlModBus_CheckRxTimeout(void);

void utlModBus_Transmitt(void);

void UART0_receiveCompleteCb(void);

void UART0_transmitCompleteCb(void);

void utlModBus_GetModBusData(unsigned char *Function, unsigned short *Register, unsigned short *Data);

unsigned char utlModBus_FormatRequest(unsigned char ModBusFunction, unsigned short ModBusRegister, unsigned short ModBusData);

unsigned char utlModBus_GetUARTStatus(void);

void utlModBus_SetUARTStatus(unsigned char status);

void utlModBus_SetError(unsigned char error);

#ifdef	__cplusplus
}
#endif


#endif /* UTL_MODBUS_H_ */

