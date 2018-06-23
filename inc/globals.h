/*
 * globals.h
 *
 *  Created on: Jun 20, 2018
 *      Author: subhojit
 */

#ifndef INC_GLOBALS_H_
#define INC_GLOBALS_H_

#include "audio_capture.h"
#include "filter.h"

#define __SYSTEM_FREQUENCY_MHZ__        8000000
#define PUSHBUTTON1_PORT                GPIO_PORT_P5
#define PUSHBUTTON1_PIN                 GPIO_PIN6
#define PUSHBUTTON2_PORT                GPIO_PORT_P5
#define PUSHBUTTON2_PIN                 GPIO_PIN5
#define NULL ( (void *) 0)


typedef enum {
    IDLE = 0,
    SETUP_RECORD,
    REC_FILTER_LOOP
} Appmode;

extern AudioCaptureConfig audConfig;
extern Appmode mode;

#endif /* INC_GLOBALS_H_ */
