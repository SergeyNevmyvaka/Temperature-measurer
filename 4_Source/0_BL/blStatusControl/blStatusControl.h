/*
 * blStatusControl.h
 */

#ifndef BL_STATUS_CONTROL_H_
#define BL_STATUS_CONTROL_H_

#ifdef	__cplusplus
extern "C" {
#endif


#define BL_SC_SHORT_STATUS_SOFTWARE_VERSION                0x010F  //1.15

#define BL_STATUS_CONTROL_SHORT_NUMBER                     40//72

typedef union {
    unsigned long  Long_Value;
    unsigned char  Byte_Value[4];
} U_4BYTE_UNION;


typedef struct{
	unsigned long ARCHIVE_DATA1_R;
	unsigned long ARCHIVE_DATA2_DATE;
	unsigned long ARCHIVE_DATA3_STATUS;
} S_ARCH_MONTH;


typedef struct{
	unsigned char    SWITCH_MODBUS;
	unsigned char    SWITCH_MEASUREMENT;
	unsigned char    SWITCH_WORK_TIME;
	unsigned char    SWITCH_CNTRL_DATE;
	unsigned char    SWITCH_CNTRL_BATTERY;
	unsigned char    SWITCH_SLEEP;
	unsigned char    SWITCH_INDICATION;

	unsigned char    DEVICE_MODE;
	unsigned char    TEMPERATURE_READY;
	unsigned char    WAKE_UP_EVENT;
	unsigned char    MENU_BUTTON_STATE;
	unsigned char    BREAK_SENSOR_STATE;
	unsigned char    APPLY_MODBUS_DATA;
	unsigned char    PARAMS_NEED_TO_SAVE;
	unsigned char    INDICATION_MODE;
	unsigned char    BATTERY_MODE;

	unsigned char    OCCUR_RTC_ALARM;
	unsigned char    ADD_TIME;
	unsigned char    UPDATE_TIME;
	unsigned char    CURRENT_MINUTE;
	unsigned char    CURRENT_HOUR;
	unsigned char    CURRENT_DAY;
	unsigned char    CURRENT_MONTH;
	unsigned char    CURRENT_YEAR;
	unsigned char    CONTROL_DAY;
	unsigned char    CONTROL_MONTH;
	unsigned char    SET_CURRENT_TIME;
	unsigned char    ERROR;
	unsigned char    UPDATE_LCD;
	unsigned char    SWITCH_WRITE_ARCHIVE;
	unsigned char    COUNTING_START_TEMPERATURE;

	unsigned long    DEVICE_ID;
	unsigned long    WORK_TIME;
	unsigned long    CHECK_READINGS_CODE;
	unsigned long    CURRENT_TIME;
	unsigned long    CONTROL_DATE;
	unsigned long    TEMPERATURE_WITH_K_LONG;

	float    TEMPERATURE_WITH_K;
	float    COUNTING_RATE;
	float    K1;
	float    K2;
	float    K3;
	float    K4;
	float    CONTROL_COUNTING_RATE;
	float    BATTERY_VDD;

} S_DEVICE;




//#define BL_SC_SHORT_STATUS_
#define BL_SC_SHORT_STATUS_DEVICE_SN_H              0x0000       //1
#define BL_SC_SHORT_STATUS_DEVICE_SN_L              0x0001       //2
#define BL_SC_SHORT_STATUS_VER_SOFT                 0x0002       //3
#define BL_SC_SHORT_STATUS_DEVICE_ERROR             0x0003       //4
#define BL_SC_SHORT_STATUS_DEV_WORK_TIME_H          0x0004       //5*
#define BL_SC_SHORT_STATUS_DEV_WORK_TIME_L          0x0005       //6*
#define BL_SC_SHORT_STATUS_CURRENT_TIME_H           0x0006       //7*
#define BL_SC_SHORT_STATUS_CURRENT_TIME_L           0x0007       //8*
#define BL_SC_SHORT_STATUS_READINGS_H               0x0008       //9
#define BL_SC_SHORT_STATUS_READINGS_L               0x0009       //10
#define BL_SC_SHORT_STATUS_MODE                     0x000A       //11
#define BL_SC_SHORT_STATUS_START_READINGS_H         0x000B       //12
#define BL_SC_SHORT_STATUS_START_READINGS_L         0x000C       //13
#define BL_SC_SHORT_STATUS_CONTROL_DATE             0x000D       //14*
#define BL_SC_SHORT_STATUS_TEMPR                    0x000E       //15
#define BL_SC_SHORT_STATUS_K1_H                     0x000F       //16*
#define BL_SC_SHORT_STATUS_K1_L                     0x0010       //17*
#define BL_SC_SHORT_STATUS_K2_H                     0x0011       //18*
#define BL_SC_SHORT_STATUS_K2_L                     0x0012       //19*
#define BL_SC_SHORT_STATUS_APPLY                    0x0013       //20
#define BL_SC_SHORT_STATUS_DELETE_ARCHIVE           0x0014       //21*
#define BL_SC_SHORT_STATUS_MEASURE_TEMP             0x0015       //22
#define BL_SC_SHORT_STATUS_K3_H                     0x0017       //23
#define BL_SC_SHORT_STATUS_K3_L                     0x0018       //24
#define BL_SC_SHORT_STATUS_K4_H                     0x0019       //25
#define BL_SC_SHORT_STATUS_K4_L                     0x001A       //26
#define BL_SC_SHORT_STATUS_CHECK_TAMPER             0x001B       //27
#define BL_SC_SHORT_STATUS_TEMPR_MODBUS             0x001C       //28

#define BL_SC_SHORT_STATUS_CURRENT_TIME_SEC         0x001D       //29
#define BL_SC_SHORT_STATUS_CURRENT_TIME_MIN         0x001E       //30
#define BL_SC_SHORT_STATUS_CURRENT_TIME_HOUR        0x001F       //31
#define BL_SC_SHORT_STATUS_CURRENT_TIME_DAY         0x0020       //32
#define BL_SC_SHORT_STATUS_CURRENT_TIME_MONTH       0x0021       //33
#define BL_SC_SHORT_STATUS_CURRENT_TIME_YEAR        0x0022       //34

#define BL_SC_SHORT_STATUS_CONTROL_TIME_DAY         0x0023       //35
#define BL_SC_SHORT_STATUS_CONTROL_TIME_MONTH       0x0024       //36
#define BL_SC_SHORT_STATUS_CONTROL_TIME_YEAR        0x0025       //37

#define BL_SC_SHORT_STATUS_BATTERY_VOLTAGE          0x0026       //38

#define BL_SC_SHORT_STATUS_CONTROL_R_OLD            0x0027       //39



#define BL_STATUS_MODE_SLEEP_R        0
#define BL_STATUS_MODE_SLEEP          1
#define BL_STATUS_MODE_WORK           2
#define BL_STATUS_MODE_TEST           3
#define BL_STATUS_MODE_ERROR          4

#define BL_STATUS_BUTTON_NOT_PRESSED    0
#define BL_STATUS_BUTTON_PRESSED_SHORT  1
#define BL_STATUS_BUTTON_PRESSED_LONG   2

#define BL_STATUS_CONTROL_NOT_UPDATED   0
#define BL_STATUS_CONTROL_UPDATED       1
#define SET         1
#define RESET       0

#define BL_STATUS_WAKE_EVENT_COMPARATOR                 0x01 //1/// Wake-up from a Comparator 0 rising edge.
#define BL_STATUS_WAKE_EVENT_PORT_MATCH                 0x02 //2/// Wake-up from a Port Match event.
#define BL_STATUS_WAKE_EVENT_RTC                        0x04 //3/// Wake-up from an RTC Alarm.
#define BL_STATUS_WAKE_EVENT_OSC_FAIL                   0x08 //4/// Wake-up from an RTC oscillator fail event.
#define BL_STATUS_WAKE_EVENT_RESET                      0x10 //5/// Wake-up from a glitch detected on RSTb.

#define BL_STATUS_SAVE_PARAMS_MONTH                     0x01
#define BL_STATUS_SAVE_PARAMS_CONTROL                   0x02
#define BL_STATUS_SAVE_PARAMS_DEVICE                    0x04
#define BL_STATUS_SAVE_PARAMS_BREAK_EVENT               0x08


#define BL_SC_STATUS_BATTERY_DISCHARGE_VOLTAGE	2.3

#define BL_STATUS_STATIC_ERROR     	0x08
#define BL_STATUS_DYNAMIC_ERROR   	0x04

#define BL_STATUS_ERROR_TAMPER_BREAK_EVENT   	(0x20 | BL_STATUS_STATIC_ERROR)  //0x28 = 40
#define BL_STATUS_ERROR_BATTERY_DISCHARGED   	(0x10 | BL_STATUS_DYNAMIC_ERROR) //0x14 = 20
#define BL_STATUS_ERROR_TERMOSENSOR			 	(0x03 | BL_STATUS_STATIC_ERROR)  //0x0B = 11
#define BL_STATUS_ERROR_TAMPER                  (0x80 | BL_STATUS_STATIC_ERROR)  //0x88 = 136


#define BL_TEMP_WITH_K_K1_DEFAULT_VALUE        1.0              //
#define BL_TEMP_WITH_K_K2_DEFAULT_VALUE        0.0              //
#define BL_TEMP_WITH_K_K3_DEFAULT_VALUE        0x3F800000       //1.00  //0x41a00000//20.00
#define BL_TEMP_WITH_K_K4_DEFAULT_VALUE        0x3F933333       //1.15



void blStatusControl_SetConvCompleteFlag(unsigned char flag);

void blStatusControl_SetADC_ConvValue(unsigned long value);

void blStatusControl_ShiftParamsToModbus(S_DEVICE *device);

void blStatusControl_Init(S_DEVICE *device);

void blStatusControl_Handler(S_DEVICE *device);

void    blStatusControl_SetStatus_short(unsigned char statusID, unsigned short status);

unsigned short  blStatusControl_GetStatus_short(unsigned char statusID);

void blPower_Handler(S_DEVICE *device);

#ifdef	__cplusplus
}
#endif


#endif /* BL_STATUS_CONTROL_H_ */


