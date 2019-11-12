
#include "SI_EFM8SB2_Register_Enums.h"

#include "blModBus.h"
#include "../../1_Utl/utlModBus/utlModBus.h"
#include "../blStatusControl/blStatusControl.h"
#include "../../1_Utl/utl_Calendar/utlCalendar.h"
#include "../../1_Utl/utlLPM/utlLPM.h"

#define BL_MB_ARCHIVE_DATA_MEASURE_T    0x54      //

unsigned char ModBusFunction;
unsigned short ModBusRegister;
unsigned short ModBusData;

unsigned char blModbus_Handler(S_DEVICE *device)
{
    unsigned char currentStatus;// = 0;

	if ((utlModBus_GetUARTStatus() == MB_STATUS_CLEAR) && (device->SWITCH_MODBUS == SET))
	{
		// �������� ����� // ������� 0,5 ���, ���� ����� �� ����� ������
		utlModBus_SetUARTStatus(MB_STATUS_RX);
		utlModBus_StartRxTimeout();
		device->UPDATE_TIME = SET;
	}

    currentStatus = utlModBus_GetUARTStatus(); // ������� ����� ������� UART

    if (currentStatus != MB_STATUS_CLEAR)
    {

        if (currentStatus == MB_STATUS_COMPLETE_RX) // ����� ��������
        {
        	utlModBus_GetModBusData(&ModBusFunction, &ModBusRegister, &ModBusData); // �������� �� ��, ��� ������ ����� � ����� ������ �� ������

        	if ((ModBusRegister == BL_SC_SHORT_STATUS_MEASURE_TEMP)	&&      /// ������ ��������� �� ������� MODBUS
        		(ModBusData == BL_MB_ARCHIVE_DATA_MEASURE_T) &&
        		(device->DEVICE_MODE == BL_STATUS_MODE_SLEEP))
                {
					device->SWITCH_MEASUREMENT = SET;
					if (blStatusControl_GetStatus_short(BL_SC_SHORT_STATUS_TEMPR_MODBUS) == 0)
						return 1;

					ModBusData = blStatusControl_GetStatus_short(BL_SC_SHORT_STATUS_TEMPR_MODBUS);
					blStatusControl_SetStatus_short(BL_SC_SHORT_STATUS_TEMPR_MODBUS, RESET);
					device->SWITCH_MEASUREMENT = RESET;
        	    }


        		if (ModBusFunction == MB_FUNCTION_READ) // ������ ������ �� �������� MODBUS �������
        		{
        			if ((ModBusRegister < STORAGE_SIZE) && (ModBusData == 1))
        			{
        				blStatusControl_ShiftParamsToModbus(device);
        				ModBusData = blStatusControl_GetStatus_short(ModBusRegister);
        			}
        			else
        			{
        				ModBusFunction = MB_ExCODE_FUNCTION;
        				utlModBus_SetError(MB_ExCODE_ILLEGAL_DATA_VALUE);
        			}
        		}

        	utlModBus_FormatRequest(ModBusFunction, ModBusRegister, ModBusData); // ��������� ����� �� �����

        	utlModBus_Transmitt(); // ������ ��������
        	utlModBus_SetUARTStatus(MB_STATUS_TX);  // ��������� ������ �������  UART
        }

        if (currentStatus == MB_STATUS_COMPLETE_TX) // �������� ���������
        {
        	utlModBus_SetUARTStatus(MB_STATUS_CLEAR); // ��������� ��������� �������� UART-�

    	  	if (ModBusFunction == MB_FUNCTION_WRITE) // ������ ������ � ������� MODBUS �������
    	  	{
				if (ModBusRegister < STORAGE_SIZE)
				{
					blStatusControl_SetStatus_short(ModBusRegister, ModBusData);
				}
				else
				{
					ModBusFunction = MB_ExCODE_FUNCTION;
					utlModBus_SetError(MB_ExCODE_ILLEGAL_DATA_ADDRESS);
				}
    	  	}

    	  	utlModBus_Reinit();			// ���������� ������������ ������

        }
    }

    if (utlModBus_CheckRxTimeout())  //�������� �������� ������ �� UART
    {
       	utlModBus_Reinit();			// ���������� ������������ ������

       	utlModBus_SetUARTStatus(MB_STATUS_CLEAR); // ��������� ��������� �������� UART-�

        device->SWITCH_SLEEP = SET;  // ��������� � ����� ���
    }

    return 1;
}


