/*
 * adc.c
 *
 *  Created on: Mar 8, 2019
 *  Author: Nickolas Mitchell
 */


#include "adc.h"
#include "open_interface.h"

void adc_init()
{
     // Turn on clock for GPIO Port B and ADC0
     SYSCTL_RCGCGPIO_R = SYSCTL_RCGCGPIO_R | 0b000010; // 0x10
     SYSCTL_RCGCADC_R |= 0b1;
     // Enable alternate function
     GPIO_PORTB_AFSEL_R |= 0b00010000; //ADC channels 10 on pin 4

     // Configure pins for input and analog mode
     GPIO_PORTB_DIR_R &= 0b11101111; //Pin 4 set to input
     GPIO_PORTB_DEN_R &= 0b11101111; //Disable digital mode pin 4
     GPIO_PORTB_AMSEL_R |= 0b00010000; //Enable analog mode pin 4
    // Use default sample trigger source
    // By default, the processor (your program) is selected as the
    // trigger source and is initiated by the sample sequencer (SS)
     GPIO_PORTB_ADCCTL_R = 0x10; //Trigger in software

     // Disable ADC sample sequencer SS0 while configuring
     ADC0_ACTSS_R &= 0xFFFE; //Disable ADC0 SS0 by clearing bit 0

     // Set ADC SS0 functionality
     ADC0_EMUX_R &= 0xFFF0; //Set SS0 to trigger using ADCPSSI reg.
     ADC0_SSMUX0_R |= 0x0000000A; // Sample channel 10,

     // Configure which sample is end of sequence (bit 1 of 4-bit
     // field), after which sample the RIS flag is asserted (bit 2)
     // Use second sample (channel 1 is first, channel 9 is second)
     ADC0_SSCTL0_R = 0x00000000; // First clear register to 0
     ADC0_SSCTL0_R |= 0x00000060; // Set bits 1 and 2 for second sample

     // Re-enable ADC SS0
     ADC0_ACTSS_R |= 0x1; //Re-enable ADC0 SS0 by setting bit 0
}

int adc_read()
{
    // Initiate SS0 conversion sequence
    ADC0_PSSI_R = 0x1;

    // Wait for SS0 ADC conversions to complete
    while((ADC0_RIS_R & 0x1) == 0) // or (~ADC0_RIS_R & 0x1)
    {
        //wait
    }

    // Clear raw interrupt status flag for SS0
    ADC0_ISC_R = 0x01;

    //return converted results
    return  ADC0_SSFIFO0_R;
}

