

#ifndef __DRV_THERMO_SENSOR_H__
#define __DRV_THERMO_SENSOR_H__

#ifdef	__cplusplus
extern "C" {
#endif

#define SENSOR_COMP_PORT      0
#define SENSOR_COMP_PIN       1

#define SENSOR_REF_CAP_PORT   0
#define SENSOR_REF_CAP_PIN    0

#define SENSOR_SENS_PORT      0
#define SENSOR_SENS_PIN       2

   
unsigned long drvTermoSensor_GetDischargeTime (unsigned char DischargePinID);
 
void drvTermoSensor_MeasureModeOn(void);

void drvTermoSensor_MeasureModeOff(void);


#ifdef	__cplusplus
}
#endif

#endif //__DRV_THERMO_SENSOR_H__



