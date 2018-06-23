/*
 * filter.c
 *
 *  Created on: Jun 20, 2018
 *      Author: Subhojit Som
 */
#include <stdio.h>
#include <DSPLib.h>
#include <driverlib.h>
#include "filter.h"
#include "globals.h"
#include "bpbutter4_200_800.h"


#define FILTER_LENGTH   (SIGNAL_LENGTH)
#define FILTER_STAGES   (sizeof(FILTER_COEFFS_EX3)/sizeof(msp_biquad_df1_q15_coeffs))

#define BASE_ADDR(ptr)  ptr == adc_buffer1? "BUFFER_1":"BUFFER_2"

/* Filter coefficients */
DSPLIB_DATA(filterCoeffs,4)
msp_biquad_df1_q15_coeffs filterCoeffs[FILTER_STAGES];

DSPLIB_DATA(input_data,4)
_q15 input_data[SIGNAL_LENGTH];

/* Filter result that can be used by the application*/
DSPLIB_DATA(filtered_data,4)
_q15 filtered_data[SIGNAL_LENGTH];

/* State buffer */
DSPLIB_DATA(states,4)
msp_biquad_df1_q15_states states[FILTER_STAGES];

DSPLIB_DATA(df1Params,4)
msp_biquad_cascade_df1_q15_params df1Params;

DSPLIB_DATA(fillParams,4)
msp_fill_q15_params fillParams;

/* Benchmark cycle counts */
volatile uint32_t cycleCount = 0;

msp_status status;

void filter(){

    //    _bic_SR_register(GIE);

    uint16_t ctr = 0;
    _q15* input;
    input = Audio_get_filled_buffer();

    if(input == NULL)
        return;

    for (ctr = 0; ctr < SIGNAL_LENGTH; ++ctr) {
        input_data[ctr] = input[ctr];
    }

    // history inputs outputs initialized to 0
    fillParams.length = sizeof(states)/sizeof(_q15);
    fillParams.value = 0;

    status = msp_fill_q15(&fillParams, (void *)states);

    if(status != MSP_SUCCESS){
        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
        printf("Failure msp_fill_q15 %d\n", (int)status);
    }
    msp_checkStatus(status);

    df1Params.length = FILTER_LENGTH;
    df1Params.stages = FILTER_STAGES;
    df1Params.coeffs = filterCoeffs;
    df1Params.states = states;

//    msp_benchmarkStart(MSP_BENCHMARK_BASE, 8);
    status = msp_biquad_cascade_df1_q15(&df1Params, input_data, filtered_data);
//    cycleCount = msp_benchmarkStop(MSP_BENCHMARK_BASE);

    if(status != MSP_SUCCESS){
        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
        printf("Failure msp_biquad_cascade_df1_q15 %d\n", (int)status);
    }
    msp_checkStatus(status);

    printf("%s:%d : input_ptr = %s filter o/p status %d\n",
           __FUNCTION__, __LINE__, BASE_ADDR(input), (int) status);
}

void Filter_loop()
{
    Audio_start_recording();

    while(mode == REC_FILTER_LOOP){

        _bis_SR_register(LPM4_bits + GIE);

        filter();
    }
}
