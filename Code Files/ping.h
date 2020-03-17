/**
 * ping.h
 *
 * Created on: Mar 15, 2019
 * @author Nickolas Mitchell
 */
#include <stdint.h>
#include <stdbool.h>
#include "timer.h"
#include "lcd.h"
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"

#ifndef CODE_FILES_PING_H_
#define CODE_FILES_PING_H_

void ping_init(int *event_addr, int *time_start_addr, int *time_end_addr);
void TIMER3B_Handler(void);
void ping_read(void);



#endif /* CODE_FILES_PING_H_ */
