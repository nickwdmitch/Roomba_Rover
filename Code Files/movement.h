/*
 * movement.h
 *
 *  Created on: Feb 1, 2019
 *  Author: Evan Christensen
 */

#include "open_interface.h"
#ifndef HEADER_FILE        //MOVEMENT_H_
#define HEADER_FILE        //MOVEMENT_H_


//Function headers and macro definitions
void move_forward(oi_t *sensor_data, int centimeters);
void turn_right(oi_t *sensor_data, int degrees);
void turn_left(oi_t *sensor_data, int degrees);
void move_backward(oi_t *sensor_data, int centimeters);
void forward_with_collision(oi_t *sensor_data, int centimeters);
void cliff_sensor_detect(oi_t *sensor, int dist_centi);

#endif /* MOVEMENT_H_ */
