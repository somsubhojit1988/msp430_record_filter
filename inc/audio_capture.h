/*
 * audio_capture.h
 *
 *  Created on: Jun 20, 2018
 *      Author: subhojit
 */

#ifndef INC_AUDIO_CAPTURE_H_
#define INC_AUDIO_CAPTURE_H_

#include <stdint.h>
#include <stdbool.h>
#include <DSPLib.h>

#define SIGNAL_LENGTH 256
#define SAMPLING_FREQ 5000

#define MIC_POWER_PORT_DIR  P6DIR
#define MIC_POWER_PORT_OUT  P6OUT
#define MIC_POWER_PIN       BIT2

#define AUDIO_PORT_SEL0     P1SEL0
#define AUDIO_PORT_SEL1     P1SEL1
#define MIC_INPUT_PIN       BIT3

#define MIC_INPUT_CHAN      ADC12INCH_3

typedef struct {
    uint16_t    bufferlen;
    uint16_t    samplingFrequency;
    _q15*       filledBuffer;
    bool        filledBuffer1;
}AudioCaptureConfig ;

extern _q15 adc_buffer1[SIGNAL_LENGTH];
extern _q15 adc_buffer2[SIGNAL_LENGTH];

void Audio_setup_recording();
void Audio_start_recording();
void Audio_stop_recording();
void Audio_shutdown_recording();

_q15* Audio_get_filled_buffer();
#endif /* INC_AUDIO_CAPTURE_H_ */
