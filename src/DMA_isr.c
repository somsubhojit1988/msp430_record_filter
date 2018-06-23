/*
 * DMA_isr.c
 *
 *  Created on: Jun 20, 2018
 *      Author: subhojit
 */
#include "globals.h"
#include <driverlib.h>

//******************************************************************************
// DMA interrupt service routine
//******************************************************************************
#pragma vector=DMA_VECTOR
__interrupt void dmaIsrHandler(void)
{
    switch (__even_in_range(DMAIV, DMAIV_DMA2IFG))
    {
    case DMAIV_DMA0IFG: break;

    case DMAIV_DMA1IFG:
        // Disable the dma transfer
        DMA_disableTransfers(DMA_CHANNEL_1);

        // Disable DMA channel 1 interrupt
        DMA_disableInterrupt(DMA_CHANNEL_1);

        audConfig.filledBuffer = adc_buffer1;
        audConfig.filledBuffer1 = true;

        // Enable DMA channel 2 interrupt
        DMA_enableInterrupt(DMA_CHANNEL_2);

        // Enable DMA channel 2 transfers
        DMA_enableTransfers(DMA_CHANNEL_2);

        // Start Cpu on exit
        __bic_SR_register_on_exit(LPM4_bits);
        break;

    case DMAIV_DMA2IFG:
        // Disable the dma transfer
        DMA_disableTransfers(DMA_CHANNEL_2);

        // Disable DMA channel 2 interrupt
        DMA_disableInterrupt(DMA_CHANNEL_2);

        audConfig.filledBuffer = adc_buffer2;

        // Enable DMA channel 1 interrupt
        DMA_enableInterrupt(DMA_CHANNEL_1);

        // Enable DMA channel 1 transfers
        DMA_enableTransfers(DMA_CHANNEL_1);

        // Start Cpu on exit
        __bic_SR_register_on_exit(LPM4_bits);
        break;

    default: break;
    }
}
