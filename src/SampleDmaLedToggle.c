/*
 * SampleDmaLedToggle.c
 *
 *  Created on: Jun 26, 2018
 *      Author: subhojit
 */

#include <driverlib.h>
#include "globals.h"


void sample_dma_led_toggle_run(void){

    Audio_setup_recording();

    Audio_start_recording();

    _bis_SR_register(GIE + LPM4_bits);

    Audio_stop_recording();

    Audio_shutdown_recording();
}


