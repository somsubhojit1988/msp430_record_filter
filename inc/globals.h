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
#include "SampleLEDToggle.h"
#include "SampleDMALedToggle.h"

#define __SYSTEM_FREQUENCY_MHZ__        8000000
#define PUSHBUTTON1_PORT                GPIO_PORT_P5
#define PUSHBUTTON1_PIN                 GPIO_PIN6
#define PUSHBUTTON2_PORT                GPIO_PORT_P5
#define PUSHBUTTON2_PIN                 GPIO_PIN5
#define TOGGLE_PORT                     GPIO_PORT_P4
#define TOGGLE_PIN                      GPIO_PIN2

#define NULL ( (void *) 0)



typedef enum {
    IDLE = 0,
    ADC_SAMPLE_LED_TOGGLE,
    ADC_SAMPLE_DMA_LED_TOGGLE,
    REC_FILTER_LOOP
} Appmode;

extern AudioCaptureConfig audConfig;
extern Appmode mode;

#endif /* INC_GLOBALS_H_ */
