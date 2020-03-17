#include <stdint.h>
#include <stdbool.h>
#include "timer.h"
#include "lcd.h"
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"

#ifndef CODE_FILES_SERVO_H_
#define CODE_FILES_SERVO_H_

/*
 * servo.c
 *
 *  Created on: Apr 30, 2019
 *  Author: Nickolas Mitchell, Michael Ostrow
 */
void servo_init(void);
int servo_move(double degrees);



#endif /* CODE_FILES_SERVO_H_ */
