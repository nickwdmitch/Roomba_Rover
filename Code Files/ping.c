/*
 * ping.c
 *
 *  Created on: Mar 15, 2019
 *  Author: Nickolas Mitchell
 */
#include <stdint.h>
#include <stdbool.h>
#include "timer.h"
#include "lcd.h"
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"

#include "open_interface.h"

#ifndef CODE_FILES_PING_C_
#define CODE_FILES_PING_C_

volatile int state;
volatile int *done_flag;
volatile int *timer_start, *timer_end;

void TIMER3B_Handler() {
    // Check if an input edge-time capture interrupt occurred
    // Test the interrupt status bit
    if (TIMER3_MIS_R & 0x400) {
        // Clear the interrupt status bit by writing 1 to ICR bit
        TIMER3_ICR_R |= 0x400;
        // Capture rising edge time
        if (state == 0) {
            *timer_start = TIMER3_TBR_R;
            state = 1;
        } else {
            *timer_end = TIMER3_TBR_R;
            *done_flag = 1; //tell main done capturing 1st & 2nd wheel times
            state = 0;
        }
    }
}

void ping_init(int *event_addr, int *time_start_addr, int *time_end_addr) {
    done_flag = event_addr;
    timer_start = time_start_addr;
    timer_end = time_end_addr;
    state = 0;

    //GPIO setup
    SYSCTL_RCGCGPIO_R = SYSCTL_RCGCGPIO_R | 0b000010;
    SYSCTL_RCGCTIMER_R |= 0b00001000; // Enable Timer 3’s clock
    GPIO_PORTB_AFSEL_R |= 0b00001000; // Timer1A input is PB3
    GPIO_PORTB_PCTL_R |= 0x00007000; // Use function T1CCP0

    GPIO_PORTB_DEN_R |= 0b00001000; // Set pin 3 digital mode

    GPIO_PORTB_DIR_R &= 0b11110111; // Set up pin 3 as input

    //Setup timer T3CCP1
    TIMER3_CTL_R &= ~0x100;   //disable timer B
	// Configure Timer 1A functionality
    TIMER3_CFG_R |= 0x4; // Set to 16-bit mode
    TIMER3_TBMR_R |= 0b10111; //Count up, edge-time, capture mode
    TIMER3_CTL_R |= 0x0C00; // Detect both edges (10:11=11)
    TIMER3_TBPR_R |= 0xFF; // Use prescaler extension to 24 bits
    TIMER3_TBILR_R |= 0xFFFF;

    // Load max 24-bit value
	// B) Set up Timer 1A interrupts
    TIMER3_ICR_R |= 0x400;     //Clear capture event interrupt status
    TIMER3_IMR_R |= 0x400; // Enable capture event interrupts

    // 3. NVIC setup
    // A) Configure NVIC to allow Timer 1A interrupts (use priority=1)
    NVIC_PRI9_R |= 0x20;     //Timer1A IRQ pri=1, bits 15-13
    NVIC_EN1_R |= 0x10; //Enable Timer1A IRQ (21), bit 21

	// B) Bind Timer 1A interrupt requests to user’s interrupt handler
    IntRegister(INT_TIMER3B, TIMER3B_Handler);
    // Re-enable Timer 1A
    TIMER3_CTL_R |= 0x100;
}

void ping_read() {
    GPIO_PORTB_AFSEL_R &= 0b11110111; // Turn off alternate function
    GPIO_PORTB_DIR_R |= 0x8; // Set up pin 3 as output

    //low_high_low
    GPIO_PORTB_DATA_R &= 0xFFFFFFF7;
    timer_waitMicros(2);
    GPIO_PORTB_DATA_R |= 0x8;
    timer_waitMicros(5);
    GPIO_PORTB_DATA_R &= 0xFFFFFFF7;

    GPIO_PORTB_DIR_R &= 0b11110111; // Set up pin 3 as input
    GPIO_PORTB_AFSEL_R |= 0b00001000; // Turn on alternate function
}


#endif /* CODE_FILES_PING_C_ */
