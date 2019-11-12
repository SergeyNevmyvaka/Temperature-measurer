
#include "utlModBus.h"
#include "../../2_Drv/drvUSART/drvUSART.h"
#include "../../2_Drv/drvSysClock/drvSysClock.h"


static unsigned char MB_Status = MB_STATUS_CLEAR;
static unsigned char MB_PacketSize;// = 0;
static unsigned char MB_ErrorCode = 0;
static unsigned long TicksTimeoutStart;// = 0;

SI_SEGMENT_VARIABLE(MB_Buffer[USART_MAX_RX_BUFFER_SIZE], uint8_t, SI_SEG_XDATA);
SI_SEGMENT_VARIABLE(UART_Buffer[USART_MAX_RX_BUFFER_SIZE], uint8_t, SI_SEG_XDATA);


unsigned char ModBus_getLRC(unsigned char *ptrPackage, unsigned char size);

/*******************************************************************************************************************/

unsigned char ModBus_getLRC(unsigned char *ptrPackage, unsigned char size)
{
    unsigned char LRC = 0;
    unsigned short i;// = 0;

    for (i = 0; i < size-1; i++)
    {
        LRC += ptrPackage[i];
    }

    return ((unsigned char)(-((LRC))));
}


void utlModBus_StartRxTimeout(void)
{
	TicksTimeoutStart = drvSysClock_getTime();
}

unsigned char utlModBus_CheckRxTimeout(void)
{
	if (drvSysClock_getPastTime(TicksTimeoutStart/*, SYSCLOCK_GET_TIME_MKS_100*/) > MB_TIMEOUT_PERIOD)
	{
		return 1;
	}
	return 0;
}


void utlModBus_Init(void)
{
    unsigned char i;
    
    for (i = 0; i < USART_MAX_RX_BUFFER_SIZE; i++)
    {
        MB_Buffer[i] = 0;
        UART_Buffer[i] = 0;
    }
    utlModBus_SetUARTStatus(MB_STATUS_CLEAR);

    drvUSART_InitUART(UART_Buffer, USART_MAX_RX_BUFFER_SIZE);
}

void utlModBus_Reinit(void)
{
    unsigned char i;

    for (i = 0; i < USART_MAX_RX_BUFFER_SIZE;i++)
    {
    	UART_Buffer[i] = 0;
    }
    UART0_readBuffer(UART_Buffer, USART_MAX_RX_BUFFER_SIZE); // включить прием
}


void UART0_receiveCompleteCb(void)
{
    unsigned char i;

    if ((UART_Buffer[MB_LCR_BYTE] == ModBus_getLRC(UART_Buffer, USART_MAX_RX_BUFFER_SIZE)) && (UART_Buffer[MB_ADDRESS_BYTE] == MB_DEVICE_ADDRESS))
    {
        for (i = 0; i < USART_MAX_RX_BUFFER_SIZE; i++)
        {
            MB_Buffer[i] = UART_Buffer[i];
        }
        utlModBus_SetUARTStatus(MB_STATUS_COMPLETE_RX);
        utlModBus_Reinit();
    }
    else
    {
    	utlModBus_Reinit();
    }
}


void UART0_transmitCompleteCb(void)
{
    utlModBus_SetUARTStatus(MB_STATUS_COMPLETE_TX);
    utlModBus_Reinit();
}

void utlModBus_Transmitt(void)
{
    unsigned char i;

	for (i = 0; i < USART_MAX_RX_BUFFER_SIZE; i++)
	{
		UART_Buffer[i] = MB_Buffer[i];
	}
    UART0_writeBuffer(UART_Buffer, MB_PacketSize);
}

void utlModBus_SetUARTStatus(unsigned char status)
{
    MB_Status = MB_STATUS_CLEAR;
    MB_Status = status;
}

unsigned char utlModBus_GetUARTStatus(void)
{
    return MB_Status;
}

void utlModBus_GetModBusData(unsigned char *Function, unsigned short *Register, unsigned short *Data)
{
	if ((MB_Buffer[MB_FUNCTION_BYTE] == MB_FUNCTION_READ) || (MB_Buffer[MB_FUNCTION_BYTE] == MB_FUNCTION_WRITE))
    {
		*Function = MB_Buffer[MB_FUNCTION_BYTE];

		*Register  = (unsigned short)(MB_Buffer[MB_REG_BYTE_HIGH]) << 8;
		*Register |= (unsigned short)(MB_Buffer[MB_REG_BYTE_LOW]);

		*Data  = (unsigned short)(MB_Buffer[MB_DATA_BYTE_HIGH]) << 8;
		*Data |= (unsigned short)(MB_Buffer[MB_DATA_BYTE_LOW]);
    }
    else
    {
    	*Function = MB_ExCODE_FUNCTION;
    	MB_ErrorCode = MB_ExCODE_ILLEGAL_LCR;
    }

}

unsigned char utlModBus_FormatRequest(unsigned char ModBusFunction, unsigned short ModBusRegister, unsigned short ModBusData)
{
    
    switch(ModBusFunction)
    {
    	case MB_FUNCTION_READ :
            MB_PacketSize = 6;
            MB_Buffer[0] = 0x01;
            MB_Buffer[1] = ModBusFunction;
            MB_Buffer[2] = 0x02;
            MB_Buffer[3] = ModBusData >> 8;
            MB_Buffer[4] = ModBusData;
            MB_Buffer[5] = ModBus_getLRC(&MB_Buffer, MB_PacketSize);
            break;
        case MB_FUNCTION_WRITE :
            MB_PacketSize = 7;
            MB_Buffer[0] = 0x01;
            MB_Buffer[1] = ModBusFunction;
            MB_Buffer[2] = ModBusRegister >> 8;
            MB_Buffer[3] = ModBusRegister;
            MB_Buffer[4] = ModBusData >> 8;
            MB_Buffer[5] = ModBusData;
            MB_Buffer[6] = ModBus_getLRC(&MB_Buffer, MB_PacketSize);
            break;
        default:
        	 MB_PacketSize = 4;
        	MB_Buffer[MB_ADDRESS_BYTE]    = MB_DEVICE_ADDRESS;
        	MB_Buffer[MB_FUNCTION_BYTE]   |= MB_ExCODE_FUNCTION;
        	MB_Buffer[MB_ERORCODE_BYTE]   = MB_ErrorCode;
        	MB_Buffer[MB_ERORCODE_BYTE+1] = ModBus_getLRC(&MB_Buffer, MB_PacketSize);
            //*size = 4;
            break;
    }
    return 1;
}

void utlModBus_SetError(unsigned char error)
{
	MB_ErrorCode = error;
}


