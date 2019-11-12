/*
 * drvLPM.h
 */

#ifndef HAL_GPIO_H_
#define HAL_GPIO_H_

#ifdef	__cplusplus
extern "C" {
#endif

// OR'e values when use
#define XBR_UART    0x01
#define XBR_SPI     0x02
#define XBR_SMBUS   0x04
#define XBR_SYSCLK  0x08
#define XBR_COMP    0x10
#define XBR_ACOPM   0x20

#define GPIO_PORT_0 0 
#define GPIO_PORT_1 1   
#define GPIO_PORT_2 2 

#define GPIO_PIN_0 0 
#define GPIO_PIN_1 1 
#define GPIO_PIN_2 2 
#define GPIO_PIN_3 3 
#define GPIO_PIN_4 4 
#define GPIO_PIN_5 5  
#define GPIO_PIN_6 6 
#define GPIO_PIN_7 7 

#define PIN_ON      1
#define PIN_OFF     0

#define ENABLED      1
#define DISABLED     0
//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

// P_0_2
void halGPIO_SensPinUnused(void);
// P_0_0
void halGPIO_RefPinUnused(void);
// P_0_1
void halGPIO_CompPinUnused(void);

// P_0_0
void halGPIO_RefPinCharge(void);
// P_0_1
void halGPIO_CompPinAnalog(void);

// P_1_5
void halGPIO_ButtModePinAnalog(void);
// P_0_3
void halGPIO_TamperPinAnalog(void);

// P_1_5
void halGPIO_ModeButtPinIn(void);
// P_0_3
void halGPIO_TamperPinIn(void);


// P_1_5
unsigned char halGPIO_ModeButtPinGetState(void);
// P_0_3
unsigned char halGPIO_TamperPinGetState(void);


void halGPIO_EnableWeakUp(void);

void halGPIO_DisableWeakUp(void);

void halGPIO_InitPortMatchPinMode(void);

void halGPIO_InitPortMatchPinTamper(void);


#ifdef	__cplusplus
}
#endif

#endif /* HAL_GPIO_H_ */

	
