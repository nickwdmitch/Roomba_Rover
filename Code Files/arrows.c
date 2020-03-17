/*
 * arrows.c
 *
 *  Created on: Apr 15, 2019
 *  Author: Evan Christensen
 */

#include "arrows.h"

void takeControl(oi_t *sensor_data)  {
    char str[80];
    while(1)  {
        char c = UART_Receive();
        //uart_sendChar(c);
        if (c == 'w')  {
            sprintf(str, "The bot has moved forward 10 cm\n\r");
            uart_sendStr(str);
            cliff_sensor_detect(sensor_data, 10);
        }
        else if (c == 'a')  {
            sprintf(str, "The bot has moved left 90 degrees\n\r");
            uart_sendStr(str);
            turn_left(sensor_data, 94);
        }
        else if (c == 's')  {
            sprintf(str, "The bot has moved back 10 cm\n\r");
            uart_sendStr(str);
            move_backward(sensor_data, 10);
        }
        else if (c == 'd')  {
            sprintf(str, "The bot has moved right 90 degrees\n\r");
            uart_sendStr(str);
            turn_right(sensor_data, 94);
        }
        else if (c == 'q')  {
            sprintf(str, "The bot has moved left 15 degrees\n\r");
            uart_sendStr(str);
            turn_left(sensor_data, 13);
        }
        else if (c == 'e')  {
            sprintf(str, "The bot has moved right 15 degrees\n\r");
            uart_sendStr(str);
            turn_right(sensor_data, 13);
        }
        else if (c == 'z')  {
            uart_sendStr("\n\r");
            scan2();
            uart_sendStr("\n\r");
            sprintf(str, "%-30s%-30s%-30s%-30s\r\n%-30s%-30s%-30s\r\n%-30s%-30s%", "W: Move forward 10 cm", "S: Move back 10 cm", "A: Rotate left 90 degrees", "D: Rotate right 90 degrees", "Q: Rotate left 15 degrees", "E: Rotate right 15 degrees", "Z: Scan environment", "M: Play music", "P: End simulation\n\r");
            uart_sendStr(str);
        }
        else if (c == 'm')  {
            sprintf(str, "There is a song playing\n\r");
            uart_sendStr(str);
            playSong(sensor_data);
        }
        else if (c == 'p')  {  //p for done
            sprintf(str, "Done\n\r");
            uart_sendStr(str);
            playSong(sensor_data);
            break;
        }
        timer_waitMillis(200);
    }
    return;
}


