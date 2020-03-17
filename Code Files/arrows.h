/*
 * arrows.h
 *
 *  Created on: Apr 15, 2019
 *  Author: Evan Christensen
 */

#ifndef ARROWS_H_
#define ARROWS_H_

#include <stdint.h>
#include <stdbool.h>
#include "timer.h"
#include "lcd.h"
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"
#include "movement.h"
#include "test.h"
#include "open_interface.h"
#include "scan.h"
#include "scan2.h"
#include "song.h"

void takeControl(oi_t *sensor_data);

#endif /* ARROWS_H_ */
