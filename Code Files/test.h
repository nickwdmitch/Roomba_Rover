/*
 * test.h
 *
 *  Created on: Feb 22, 2019
 *  Author: Nickolas Mitchell
 */



#ifndef CODE_FILES_TEST_H_
#define CODE_FILES_TEST_H_

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
void uart_sendStr(char *data);

#endif /* CODE_FILES_TEST_H_ */

