#include <stdint.h>
#include <stdbool.h>
#include "timer.h"
#include "lcd.h"
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"

void UART_init(void);
void UART_init_Interrupts(int *RX_event_addr, char *RX_char_addr);
void UART_handler(void);
void UART_Transmit(char sdata); 
char UART_Receive(void);

volatile int *RX_event_ptr;
volatile char *RX_char_ptr;

/*
* Function which initiliazes the UART
* This program utilizes interrupts to detect whether a char has been transmited or recieved.
* @author Michael Ostrow, Nickolas Mitchell
* @date 2/22/2019
*/
void UART_init(void)
{
	SYSCTL_RCGCGPIO_R |= 0x22;  //Enables clock for GPIOB and GPIOF
	SYSCTL_RCGCUART_R |= 0x02; 		//Enable clock for UART1
	
	GPIO_PORTB_AFSEL_R |= 0x03; 			//PB1 and PB0 for U1TX and U1RX
	GPIO_PORTB_PCTL_R |= 0x00000011;
	GPIO_PORTB_DEN_R |= 0x03; 				//PB1 and PB0 as digital pins
	GPIO_PORTB_DIR_R |= 0x2;
	GPIO_PORTB_DIR_R &= 0xFFFFFFFE;
	
	UART1_CTL_R &= 0xFFFFFFFE;	 	//Disable UART1
	UART1_IBRD_R |= 0x8;           //Part 1&2 (0x68) Part 3&4 (0x8)
	UART1_FBRD_R |= 0x2C;            //Part 1&2 (0xB) Part 3&4 (0x2c)
	UART1_LCRH_R |= 0x60;
	UART1_CC_R |= 0x00;
	UART1_CTL_R |= 0x1; 				//Enable UART1
}

//This function initializes the interrupts for the UART
void UART_init_Interrupts(int *RX_event_addr, char *RX_char_addr)
{
    UART1_CTL_R &= 0xFFFFFFFE;

    RX_event_ptr = RX_event_addr;
    RX_char_ptr = RX_char_addr;

    UART1_IM_R &= 0xFFFFFFEF;       //Mask RXIM
    UART1_ICR_R |= 0x10;            //Clears Interrupt handlers
    UART1_IM_R |= 0x10;             //Unmask RXIM

    NVIC_PRI1_R |= 0x00200000;
    NVIC_EN0_R |= 0x40;              //Enable NVIC

    IntRegister(INT_UART1, UART_handler);

    UART1_CTL_R |= 0x1;
}

//Handles interrupts
void UART_handler()
{
    UART1_ICR_R |= 0x10;
    *RX_event_ptr = 1;
    *RX_char_ptr = UART_Receive();
}

//Transmits characters
void UART_Transmit(char sdata)
{
	while((UART1_FR_R & 0x20) != 0);
	UART1_DR_R = sdata;
}

//Receives characters
char UART_Receive(void)
{
	uint32_t ret;
	char rdata;
	while((UART1_FR_R & 0x10) != 0);
	ret = UART1_DR_R;
	if(ret & 0xF00) 					//THIS SHOULDN'T HAPPEN 
	{
		GPIO_PORTF_DATA_R = 0xF;
	}
	else
	{
		rdata = (char)(UART1_DR_R & 0xFF);
	}
	return rdata; 
}

/**
 * @brief sends an entire string of characters over UART1 module
 * @param data pointer to the first index of the string to be sent
 */
void uart_sendStr(char *data)
{
    //until we reach a null character
    while (*data != '\0')
    {
        //send the current character
        UART_Transmit(*data);
        // increment the pointer.
        data++;
    }



}

