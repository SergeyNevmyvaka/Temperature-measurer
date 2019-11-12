/*
 * blVisualizer.h
 */

#ifndef BL_VISUALIZER_H_
#define BL_VISUALIZER_H_

#ifdef	__cplusplus
extern "C" {
#endif

#include "../blStatusControl/blStatusControl.h"

#define BL_VISUALIZER_BUTTON_NOT_PRESSED 	0
#define BL_VISUALIZER_BUTTON_PRESSED_SHORT 	1
#define BL_VISUALIZER_BUTTON_PRESSED_LONG 	2

#define BL_VISUALIZER_LEVELS_NUMBER 2
#define BL_VISUALIZER_LEVEL_0_ITEMS_NUMBER 7
#define BL_VISUALIZER_LEVEL_1_ITEMS_NUMBER 4


#define BL_VISUALIZER_STATE_SLEEP
#define BL_VISUALIZER_STATE_WORK
#define BL_VISUALIZER_STATE_TEST
#define BL_VISUALIZER_STATE_ERROR


void blVisualizer_Init(S_DEVICE *device);

void blVisualizer_Handler(S_DEVICE *device);

#ifdef	__cplusplus
}
#endif


#endif /* BL_VISUALIZER_H_ */

