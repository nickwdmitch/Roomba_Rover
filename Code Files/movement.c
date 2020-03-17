/*
 * movement.c
 *
 *  Created on: Feb 1, 2019
 *  Author: Evan Christensen, Nickolas Mitchell
 */

#include "movement.h"
#include "lcd.h"
#include "test.h"

//This function moves the robot forward without checking the bumpers.
void move_forward(oi_t *sensor_data, int centimeters) {

    int sum = 0;
    oi_setWheels(250, 250);

    while (sum < centimeters * 10)  {
        oi_update(sensor_data);
        sum += sensor_data -> distance;
    }
    oi_setWheels(0, 0);
}

//This function moves the robot backwards without checking the sensors
void move_backward(oi_t *sensor_data, int centimeters)  {
    int sum = 0;
    oi_setWheels(-250, -250);

    while (sum > centimeters * -10)  {
        oi_update(sensor_data);
        sum += sensor_data -> distance;
    }
    oi_setWheels(0, 0);
}

//This function turns the robot right without checking the sensors
void turn_right(oi_t *sensor_data, int degrees) {
    int sum = 0;
    oi_setWheels(-200,200);
    degrees *= -1;
    while(sum > degrees) {
        oi_update(sensor_data);
        sum += sensor_data -> angle;
    }
    oi_setWheels(0, 0);
}

//This function turns the robot left without checking the sensors
void turn_left(oi_t *sensor_data, int degrees)  {
    int sum = 0;
    oi_setWheels(200,-200);

    while(sum < degrees){
        oi_update(sensor_data);
        sum += sensor_data -> angle;
    }
    oi_setWheels(0, 0);
}

void cliff_sensor_detect(oi_t *sensor, int dist_centi)
{
    char bumpLeft[] = "ouch on left bumper\n\r";
    char bumpRight[] = "ouch on right bumper\n\r";
    char cliffLeft[] = "cliff on left left sensor\n\r";
    char cliffFrontLeft[] = "cliff on front left sensor\n\r";
    char cliffRight[] = "cliff on right right sensor\n\r";
    char cliffFrontRight[] = "cliff on front right sensor\n\r";
    char signalLeft[] = "hit edge on left left\n\r";
    char signalFrontLeft[] = "hit edge on front left\n\r";
    char signalRight[] = "hit edge on right right\n\r";
    char signalFrontRight[] = "hit edge on front right\n\r";
    int millimeters = dist_centi * 10;
    int sum = 0;
    //int i = 0;
    oi_setWheels(150, 150);
    while (sum < millimeters)
    {
        oi_update(sensor);

        //Object or cliff
        //checks if the left bumper has been pushed, if so, moves around obstacle
        if (sensor->bumpLeft == 1)
        {
            move_backward(sensor, 10);
            sum -= 50;
            uart_sendStr(bumpLeft);
            break;
        }

        //checks if the right bumper has been pushed, if so, moves around obstacle
        if (sensor->bumpRight == 1)
        {
            move_backward(sensor, 10);
            sum -= 50;
            uart_sendStr(bumpRight);
            break;
        }

        //checks if left cliff sensor was triggered
        if (sensor->cliffLeft == 1)
        {
            move_backward(sensor, 10);
            sum -= 50;
            uart_sendStr(cliffLeft);
            break;
        }
        if (sensor->cliffFrontLeft == 1)
        {
            move_backward(sensor, 10);
            sum -= 50;
            uart_sendStr(cliffFrontLeft);
            break;
        }

        //checks if right cliff sensor was triggered
        if (sensor->cliffRight == 1)
        {
            move_backward(sensor, 10);
            sum -= 50;
            uart_sendStr(cliffRight);
            break;
        }
        if (sensor->cliffFrontRight == 1)
        {
            move_backward(sensor, 10);
            uart_sendStr(cliffFrontRight);
            break;
        }

        //Boundary
        //checks left bounds
        if (sensor->cliffLeftSignal > 2600)
        {
            move_backward(sensor, 10);
            sum -= 100;
            uart_sendStr(signalLeft);
            break;
        }
        if (sensor->cliffFrontLeftSignal > 2600)
        {
            move_backward(sensor, 10);
            sum -= 100;
            uart_sendStr(signalFrontLeft);
            break;
        }

        //checks right bounds
        if (sensor->cliffRightSignal > 2600)
        {
            move_backward(sensor, 10);
            sum -= 100;
            uart_sendStr(signalRight);
            break;
        }
        if (sensor->cliffFrontRightSignal > 2600)
        {
            move_backward(sensor, 10);
            sum -= 100;
            uart_sendStr(signalFrontRight);
            break;
        }
        //oi_setWheels(250, 250);
        sum += sensor->distance;
    }
    oi_setWheels(0, 0);
}


