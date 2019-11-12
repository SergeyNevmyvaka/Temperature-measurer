
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8SB2_Register_Enums.h>                  // SFR declarations

#include "0_BL/blStatusControl/blStatusControl.h"
#include "0_BL/blModeControl/blModeControl.h"
#include "0_BL/blModbus/blModBus.h"
#include "0_BL/blVisualizer/blVisualizer.h"
#include "0_BL/blCountingRate/blCountingRate.h"
#include "0_BL/blTempWithK/blTempWithK.h"
#include "0_BL/blArchive/blArchive.h"

SI_LOCATED_VARIABLE_NO_INIT (reserved, uint8_t, SI_SEG_XDATA, 0x0000);

S_DEVICE Device;

void Init(S_DEVICE *device);


// $[Generated Includes]
// [Generated Includes]$

//-----------------------------------------------------------------------------
// main() Routine
// ----------------------------------------------------------------------------
int main (void)
{
	// Call hardware initialization routine
	Init(&Device);

	while (1) 
   {
		blModeControl_ModeHandler(&Device);

		blStatusControl_Handler(&Device);

		blArchive_Handler(&Device);

		blModbus_Handler(&Device);

		blTempWithK_Handler(&Device);

		blCountingRate_Handler(&Device);

		blVisualizer_Handler(&Device);

		blPower_Handler(&Device);

		// $[Generated Run-time code]
		// [Generated Run-time code]$
   }                             
}


void Init(S_DEVICE *device)
{
	blStatusControl_Init(device);

	blArchive_RestoreData(device);
}


SI_INTERRUPT (RTC0ALARM_ISR, RTC0ALARM_IRQn)
{
	Device.OCCUR_RTC_ALARM = SET;
	Device.WAKE_UP_EVENT = BL_STATUS_WAKE_EVENT_RTC;
}



