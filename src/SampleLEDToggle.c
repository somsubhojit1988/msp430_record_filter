/*
 * SampleLEDToggle.c
 *
 *  Created on: Jun 25, 2018
 *      Author: subhojit
 */
#include <driverlib.h>

#include "globals.h"


void init_sample_toggle(AudioCaptureConfig *audCapCon) {
    Timer_A_initUpModeParam upConfig =
       {
        TIMER_A_CLOCKSOURCE_SMCLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_1,
        (__SYSTEM_FREQUENCY_MHZ__ / audCapCon->samplingFrequency) - 1,
        TIMER_A_TAIE_INTERRUPT_DISABLE,
        TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE,
        TIMER_A_DO_CLEAR,
        false
       };

       Timer_A_initCompareModeParam compareConfig =
       {
        TIMER_A_CAPTURECOMPARE_REGISTER_1,
        TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE,
        TIMER_A_OUTPUTMODE_TOGGLE_RESET,
        ((__SYSTEM_FREQUENCY_MHZ__ / audCapCon->samplingFrequency) / 2) - 1
       };

       // Turn on mic power full drive strength and enable mic input pin to ADC
       MIC_POWER_PORT_OUT |= MIC_POWER_PIN; // P6OUT |=  BIT2 (0x0004)
       MIC_POWER_PORT_DIR |= MIC_POWER_PIN; // P6DIR |=  BIT2 (0x0004)


       AUDIO_PORT_SEL0 |= MIC_INPUT_PIN;    // P1SEL0 |= BIT3 (0x0008)
       AUDIO_PORT_SEL1 |= MIC_INPUT_PIN;    // P1SEL1 |= BIT3 (0x0008)


        // Configure ADC
        ADC12CTL0 &= ~ADC12ENC;           // Disable conversions to configure ADC12
        // Turn on ADC, sample 32 clock cycles =~ 2us
        ADC12CTL0 = ADC12ON + ADC12SHT0_3;

        // Use sample timer, rpt single chan 0, use MODOSC, TA0 timer channel 1
        ADC12CTL1 = ADC12SHP + ADC12CONSEQ_2 + ADC12SHS_1;

        // Mic input to ADC, (AVCC/AVSS ref), sequence end bit set
        ADC12MCTL0 = MIC_INPUT_CHAN | ADC12EOS;

        //ADC12IER0 = ADC12IER0|ADC12IE0_1; //enabling interrupt for mem0

        ADC12IER0 = ADC12IE0;

        // Initialize Timer_A channel 1 to be used as ADC12 trigger
        // Initialize TACCR0 (period register) 8MHz/20kHz = 400
        // Simple counter with no interrupt. 0...400 = 400 counts/sample
        Timer_A_initUpMode(TIMER_A0_BASE, &upConfig);

        // Initialize TA0CCR1 to generate trigger clock output, reset/set mode
        Timer_A_initCompareMode(TIMER_A0_BASE, &compareConfig);

        // Enable ADC to convert when a TA0 edge is generated
        ADC12CTL0 |= ADC12ENC;

        // Delay for the microphone to settle
        __delay_cycles(10000);
}

inline void tear_down_sample_toggle(void) {
    Timer_A_stop(TIMER_A0_BASE);
    ADC12_B_disableConversions(ADC12_B_BASE, ADC12_B_COMPLETECONVERSION);

    // Turn off preamp power
    MIC_POWER_PORT_OUT &= ~MIC_POWER_PIN;

    // Disable the ADC
    ADC12_B_disable(ADC12_B_BASE);
}


void sample_and_toggle_setup(void){
    audConfig.bufferlen = SIGNAL_LENGTH;
    audConfig.samplingFrequency = SAMPLING_FREQ;

    init_sample_toggle(&audConfig);
}

inline void sample_and_toggle_start(void){
    // Start TA0 timer to begin audio data collection
    Timer_A_clear(TIMER_A0_BASE);
    Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);
}

void sample_and_toggle_run(void){

    sample_and_toggle_setup();

    sample_and_toggle_start();


    _bis_SR_register(GIE + LPM4_bits);

//    while(mode == ADC_SAMPLE_LED_TOGGLE){
////        _bis_SR_register(GIE + LPM4_bits);
//
//        __no_operation();
//    }


    tear_down_sample_toggle();
}
