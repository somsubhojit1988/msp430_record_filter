#include <driverlib.h>
#include "globals.h"

/**
 * main.c
 */

void initgpio();
void initclk();
void show_mode(Appmode mode);

AudioCaptureConfig audConfig;
Appmode mode = ADC_SAMPLE_DMA_LED_TOGGLE;

int main(void)
{
    WDT_A_hold(WDT_A_BASE);

    initgpio();
    initclk();

    PM5CTL0 &= ~LOCKLPM5;           // Clear lock bit

    //
    //    // Enable Switch interruptho
    GPIO_clearInterrupt(PUSHBUTTON1_PORT, PUSHBUTTON1_PIN);
    GPIO_enableInterrupt(PUSHBUTTON1_PORT, PUSHBUTTON1_PIN);
    GPIO_clearInterrupt(PUSHBUTTON2_PORT, PUSHBUTTON2_PIN);
    GPIO_enableInterrupt(PUSHBUTTON2_PORT, PUSHBUTTON2_PIN);
    _bis_SR_register(GIE);

    while(1)
    {
        switch(mode)
        {
        case IDLE:
            show_mode(mode);
            GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN1);
            _bis_SR_register(LPM4_bits + GIE);
            break;

        case ADC_SAMPLE_LED_TOGGLE:
            show_mode(mode);
            sample_and_toggle_run();
            break;

        case ADC_SAMPLE_DMA_LED_TOGGLE:
            show_mode(mode);
            sample_dma_led_toggle_run();
            break;

        case REC_FILTER_LOOP:
            show_mode(mode);
            rec_filter_loop();
            break;

        default:
            Audio_stop_recording();
            Audio_shutdown_recording();
            mode = IDLE;
            break;
        }

        GPIO_setOutputLowOnPin(TOGGLE_PORT, TOGGLE_PIN);
    }
}

void initgpio()
{
    //set push button 1 (5.6) to start record and filter loop
    P1OUT = 0x00;
    P1DIR = 0xFF;

    P2OUT = 0x00;
    P2DIR = 0xFF;

    P3OUT = 0x00;
    P3DIR = 0xFF;

    P4OUT = 0x00;
    P4DIR = 0xFF;

    P5OUT = 0x00;
    P5DIR = 0xFF;

    P6OUT = 0x00;
    P6DIR = 0xFF;

    P7OUT = 0x00;
    P7DIR = 0xFF;

    P8OUT = 0x04;
    P8DIR = 0xFF;

    PJOUT = 0x00;
    PJDIR = 0xFF;

    // Configure Push button switch with high to low transition
    GPIO_setAsInputPinWithPullUpResistor(PUSHBUTTON1_PORT,
                                         PUSHBUTTON1_PIN);

    GPIO_selectInterruptEdge(PUSHBUTTON1_PORT,
                             PUSHBUTTON1_PIN,
                             GPIO_HIGH_TO_LOW_TRANSITION);

    GPIO_setAsInputPinWithPullUpResistor(PUSHBUTTON2_PORT,
                                         PUSHBUTTON2_PIN);

    GPIO_selectInterruptEdge(PUSHBUTTON2_PORT,
                             PUSHBUTTON2_PIN,
                             GPIO_HIGH_TO_LOW_TRANSITION);
}

void initclk(){

    PJSEL0 |= BIT4 | BIT5;                  // For XT1

    // XT1 Setup
    CSCTL0_H = CSKEY >> 8;                  // Unlock CS registers
    CSCTL1 = DCOFSEL_6;                     // Set DCO to 8MHz
    CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK;
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;   // set all dividers
    CSCTL4 &= ~LFXTOFF;

    do
    {
        CSCTL5 &= ~LFXTOFFG;                // Clear XT1 fault flag
        SFRIFG1 &= ~OFIFG;
    } while (SFRIFG1 & OFIFG);              // Test oscillator fault flag
    CSCTL0_H = 0;                           // Lock CS registers

}

void show_mode(Appmode mode) {
    uint16_t ctr = 100;
    switch(mode)
    {
    case ADC_SAMPLE_LED_TOGGLE:
        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN1);
        break;
    case ADC_SAMPLE_DMA_LED_TOGGLE:
        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN1);
        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
        break;
    case REC_FILTER_LOOP:
        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN1);
        break;
    default:
        break;
    }
    while(ctr--){
        _delay_cycles(200000);
    }


    // indicate mode is about to start

    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN1);
    GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);

    for (ctr=50;ctr>0;ctr--)
    {
        GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN1);
        GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
        __delay_cycles(200000);
    }

    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN1);
}
