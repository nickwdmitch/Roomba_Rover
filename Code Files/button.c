/*
 *  button.c
 *  @author: Eric Middleton, Zhao Zhang, Chad Nelson, & Zachary Glanz.
 *  @date: Jul 18, 2016
 *
 *  @edit: Lindsey Sleeth and Sam Stifter on 02/04/2019
 *  @edit: Michael Ostrow, Nickolas Mitchell
 */

#include "button.h"

volatile int *button_event_ptr;
volatile int *button_num_ptr;

// The buttons are on PORTE 0:5
// GPIO_PORTE_DATA_R -- the register that must be checked on port E to determine which buttons are being pressed

#define BIT0		0x01
#define BIT1		0x02
#define BIT2		0x04
#define BIT3		0x08
#define BIT4		0x10
#define BIT5		0x20
#define BIT6		0x40
#define BIT7		0x80

uint8_t prev_Button; //must be set yourself in button_getButton()
uint8_t button; //current button being pressed, must be set yourself in button_getButton()

/**
 * Initialize PORTE and configure bits 0-5 to be used as inputs for the buttons.
 */
void button_init() {
	static uint8_t initialized = 0;

	//Check if already initialized
	if(initialized){
		return;
	}

	//Initialize buttons
	SYSCTL_RCGCGPIO_R |= 0b010000;
	GPIO_PORTE_DIR_R &= 0xC0;
	GPIO_PORTE_PUR_R |= 0x3F;
	GPIO_PORTE_DEN_R = 0x3F;

	//Set the buttons to inputs and enable
	initialized = 1;
}

/**
 * Interrupt handler -- executes when a hardware event occurs (a button is pressed)
 */
void init_button_interrupts(int *button_event_addr, int *button_num_addr) {
    button_event_ptr = button_event_addr;
    button_num_ptr = button_num_addr;

    // Mask the bits for pins 0-5
    GPIO_PORTE_IM_R &= 0xFFFFFFC0;

    // Set pins 0-5 to use edge sensing
    GPIO_PORTE_IS_R &= 0xFFFFFFC0;

    // Set pins 0-5 to use both edges. We want to update the LCD
    // when a button is pressed, and when the button is released.
    GPIO_PORTE_IBE_R |= 0x3F;

    // Clear the interrupts
    GPIO_PORTE_ICR_R = 0x3F;

    // Unmask the bits for pins 0-5
    GPIO_PORTE_IM_R |= 0x3F;

    // Enable GPIO port E interrupt
    NVIC_EN0_R |= 0x10;

    // Bind the interrupt to the handler.
    IntRegister(INT_GPIOE, gpioe_handler);
}

/**
 * Handles a hardware interrupt that occurs from a button being pressed
 */
void gpioe_handler() {
    // Clear interrupt status register
    GPIO_PORTE_ICR_R = 0x3F;
    *button_event_ptr = 1;
    *button_num_ptr = button_getButton();
}

/**
 * returns a 6-bit bit field, representing the push buttons. A 1 means the corresponding button is pressed.
 */
uint8_t button_checkButtons() {
    return (~GPIO_PORTE_DATA_R) & (BIT6-1); //Return the button status
}

/**
 * Returns the position of the leftmost button being pushed.
 * @return the position of the leftmost button being pushed. A 6 is the leftmost button, 1 is the rightmost button. Return 0 if no button is being pressed.
 */
uint8_t button_getButton() {
    //Checks if buttons are pressed, with priority to higher value buttons, then
    //returns the correct value, or 0 if no buttons are pressed
    if ((~GPIO_PORTE_DATA_R) & (0b100000))
    {
        return 6;
    }
    if ((~GPIO_PORTE_DATA_R) & (0b010000))
    {
        return 5;
    }
    if ((~GPIO_PORTE_DATA_R) & (0b001000))
    {
        return 4;
    }
    if ((~GPIO_PORTE_DATA_R) & (0b000100))
    {
        return 3;
    }
    if ((~GPIO_PORTE_DATA_R) & (0b000010))
    {
        return 2;
    }
    if ((~GPIO_PORTE_DATA_R) & (0b000001))
    {
        return 1;
    }
    return 0;
}


