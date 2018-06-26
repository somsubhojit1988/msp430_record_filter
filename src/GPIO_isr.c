/*
 * GPIO_isr.c
 *
 *  Created on: Jun 20, 2018
 *      Author: subhojit
 */


//******************************************************************************
// Port 5 interrupt service routine
//      This ISR handles the push button to switch the application
//      mode.
//******************************************************************************
#include "globals.h"
#pragma vector=PORT5_VECTOR
__interrupt void port5IsrHandler(void)
{
    switch (__even_in_range(P5IV, P5IV_P5IFG7))
    {
    case P5IV_NONE: break;
    case P5IV_P5IFG0: break;
    case P5IV_P5IFG1: break;
    case P5IV_P5IFG2: break;
    case P5IV_P5IFG3: break;
    case P5IV_P5IFG4: break;
    case P5IV_P5IFG5:
        break;
    case P5IV_P5IFG6:
        // Toggle record mode
        //exit LPM mode on exit
        mode++;
        audConfig.filledBuffer = NULL;

        __bic_SR_register_on_exit(LPM4_bits);
        break;
    case P5IV_P5IFG7: break;
    default: break;
    }
}
