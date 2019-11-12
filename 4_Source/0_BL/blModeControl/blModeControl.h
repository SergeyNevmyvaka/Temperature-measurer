/*
 * blModeControl.h
 */

#ifndef BL_MODE_CONTROL_H_
#define BL_MODE_CONTROL_H_

#ifdef	__cplusplus
extern "C" {
#endif

#include "../blStatusControl/blStatusControl.h"

#define BL_MODE_CONTROL_MODE_SLEEP_R        0
#define BL_MODE_CONTROL_MODE_SLEEP          1
#define BL_MODE_CONTROL_MODE_WORK           2
#define BL_MODE_CONTROL_MODE_TEST           3
#define BL_MODE_CONTROL_MODE_ERROR          4


void blModeControl_ChangeModeHandler(S_DEVICE *device);
void blModeControl_ModeHandler(S_DEVICE *device);

#ifdef	__cplusplus
}
#endif

#endif /* BL_MODE_CONTROL_H_ */

