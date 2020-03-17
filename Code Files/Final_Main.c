#include "lcd.h"
#include "test.h"
#include "string.h"
#include "movement.h"
#include "open_interface.h"
#include "button.h"
#include "adc.h"
#include "timer.h"
#include "ping.h"
#include "servo.h"
#include <math.h>
#include "arrows.h"
#include "scan.h"
#include "scan2.h"
#include "song.h"

/**
 * Final_Main.c
 * CprE 288 final project
 * Based upon an autonomous tractor designed for farmers, our program uses sensors such as the IR and Ping attached to the roomba
 * in addition to the cliff and bump sensors which are built into the roomba to avoid cliffs, boundaries, and other tall objects.
 * In combination with the object avoidance, user control allows the roomba to be navigated safely to the goal.
 * @author Michael Ostrow, Nickolas Mitchell, Jacob Puetz, and Evan Christensen
 * @date 3/29/2019
 */

int main(void) {
    //Button variables
    int button_event=0, button_num = 0;

    //Variable to handle UART
    char str[80];

    //Ping variables
    int done_flag, time_start, time_end;

    //Initializes the ping sensor
    ping_init(&done_flag, &time_start, &time_end);

    //Initializes the servo
    servo_init();

    //Initializes the ADC (Ping sensor)
    adc_init();

    //Initializes the UART
    UART_init();

    //Initializes LCD
    lcd_init();

    //Initialize buttons and button interrupts
    button_init();
    init_button_interrupts(&button_event, &button_num);

    //Move the servo to a starting position of 90 degrees
    servo_move(0);

    //Send header
    uart_sendStr("\r\n\r\n\r\n\r\n\r\n\r\n");
    sprintf(str, "%-20s\r\n",
            "---------------------------------------------------CprE Final Project---------------------------------------------------");
    uart_sendStr(str);
    sprintf(str, "%-20s\r\n",
            "-------------------------------------------------------AV Tractor-------------------------------------------------------");
    uart_sendStr(str);
    uart_sendStr("\r\n");
    lcd_printf("Please wait for further instructions");

    //Initialize Cybot sensors
    oi_t *sensor_data = oi_alloc();
    //oi_free(sensor_data);
    oi_init(sensor_data);

    //Personal secondhand serenade
    playSong2(sensor_data);

    //Give sensors enough time to properly initialize
    timer_waitMillis(1000);

    sprintf(str, "The tractor is now returning to the shed\r\n");
    uart_sendStr(str);
    uart_sendStr("\r\n");
    sprintf(str, "%-30s%-30s%-30s%-30s\r\n%-30s%-30s%-30s\r\n%-30s%-30s%", "W: Move forward 10 cm", "S: Move back 10 cm", "A: Rotate left 90 degrees", "D: Rotate right 90 degrees", "Q: Rotate left 15 degrees", "E: Rotate right 15 degrees", "Z: Scan environment", "M: Play music", "P: End simulation");
    uart_sendStr(str);
    uart_sendStr("\r\n");

    //Robot is running
    lcd_printf("Tractor is running");
    takeControl(sensor_data);

    //Clears sensor data
    oi_free(sensor_data);

    //Final remarks
    lcd_printf("Thank you for using Cpre288 for your AV tractor needs!");
    sprintf(str, "Thank you for using Cpr2288 for your AV tractor needs!\r\n");
    uart_sendStr(str);
    uart_sendStr("\r\n");
}
