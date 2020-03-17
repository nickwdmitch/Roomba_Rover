#include <stdint.h>
#include <stdbool.h>
#include "timer.h"
#include "lcd.h"
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"

/*
 * servo.c
 *
 *  Created on: Apr 30, 2019
 *  Author: Nickolas Mitchell, Michael Ostrow
 */

void servo_init()
{
    //GPIO setup
    SYSCTL_RCGCGPIO_R = SYSCTL_RCGCGPIO_R | 0b000010;
    SYSCTL_RCGCTIMER_R |= 0b000000010; // Enable Timer 1’s clock
    GPIO_PORTB_AFSEL_R |= 0b00100000; // Timer1A input is PB5
    GPIO_PORTB_PCTL_R |= 0x00700000; // Use function T1CCP0

    GPIO_PORTB_DEN_R |= 0b00100000; // Set pin 5 digital mode

    GPIO_PORTB_DIR_R |= 0b00100000; // Set up pin 5 as output

    TIMER1_CTL_R &= ~0x100;   //disable timer B
    TIMER1_CFG_R |= 0x4; // Set to 16-bit mode
    TIMER1_TBMR_R |= 0b01010; //Sets TBMR register for count down, periodic, pwm mode
    TIMER1_TBMR_R &= ~0b10101;


    //Set start value to 21.5 ms (0x04_E200)
    TIMER1_TBPR_R = 0x04;
    TIMER1_TBILR_R = 0xE200;

    //Sets the prescaler match since it doesn't change within the bounds
    TIMER1_TBPMR_R = 0x04;

    //enables clock
    TIMER1_CTL_R |= 0x100;


}

//Moves servo to the degrees equal to the parameter
int servo_move(double degrees)
{

    //disables clock
    TIMER1_CTL_R &= ~0x100;

    //calculates the match time from the inputted degrees
    int deg = 312200 - (degrees * 158);         //**Cybot #0: 312750        **Cybot #8: 312200      **Cybot #7: 312400

    //clears the prescales values (sets to 16-bits)
    deg &= 0x00FFFF;

    //sets match register
    TIMER1_TBMATCHR_R = deg;

    //enables clock
    TIMER1_CTL_R |= 0x100;

    return 0;
}



