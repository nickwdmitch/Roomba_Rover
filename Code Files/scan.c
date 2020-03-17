/*
 * scan.c
 *
 *  Created on: Apr 18, 2019
 *  Author: Evan Christensen, Michael Ostrow, Nickolas Mitchell
 */

#include "scan.h"

struct analysis
{
    int start;
    int end;
    int distance;
    int rad_width;
    int lin_width;
    int location;
};

void scan(void)  {
        //Variable to handle UART
        char str[41];

        //Ping variables
        int done_flag, time_start, time_end, time_cycle, dist_Ping;

        //IR variables
        int buffer, temp, dist_IR;

        //Initializes the ping sensor
        ping_init(&done_flag, &time_start, &time_end);

        //Move the servo to a starting position of 0 degrees
        servo_move(0);

        //Send header
        sprintf(str, "\r\n");
        uart_sendStr(str);

        //Keeps track of the degrees
        int degrees;

        //Analysis variables
        int read = 0, object = -1, start = 0; //smallest;

        timer_waitMillis(1000);


        //PART 2
        struct analysis arr[50];

    sprintf(str, "%-20s%-20s%-20s\r\n", "degrees", "dist_IR", "dist_Ping");
    uart_sendStr(str);
        int sweeps;
        for (sweeps = 0; sweeps < 1; sweeps ++)
        {
            for (degrees = 0; degrees <= 180; degrees += 2)
            {
                //Move servo 2 degrees
                servo_move(degrees);



                //IR collects data
                buffer = 0;
                int j;
                for (j = 0; j < 16; j++)
                {
                    buffer += adc_read();
                }
                temp = buffer / 16;

                //value to distance formula
                dist_IR = 86361217037.39290 * pow(temp, -2.85756); //most recent = 86361217037.39290 * pow(temp, -2.85756);    //35581065.7721 * pow(temp, -1.9172); //15343710.81*pow(temp, -1.7996);      **CyBot #9  //289381207085.77 * pow(temp, -3.02401)  //289381207085.77*pow(temp, -3.02401);

                //Ping collects data
                ping_read();
                timer_waitMillis(100);

                //wait until both ends of ping noticed
                while (done_flag != 1)
                {
                }

                time_cycle = time_end - time_start;

                //Overflow correction
                if (time_cycle < 0)
                {
                    time_cycle = time_cycle + pow(2, 24) - 1;
                }

                //Different measurements calculations
                dist_Ping = time_cycle * 17 / 16000;

                //reset
                done_flag = 0;
                time_end = 0x0;
                time_start = 0x0;

                //Analysis
                if (dist_Ping < 80 && !read)
                {
                    read = 1;
                    object++;
                    arr[object].distance = dist_Ping;
                    arr[object].start = -1;
                    arr[object].end = -1;
                }
                //Find start value for current object
                if (read && (dist_IR < (dist_Ping + 60))
                        && !start)
                {
                    start = 1;
                    arr[object].start = degrees;

                }
                //Find end value for current object
                if (!(dist_IR < (dist_Ping + 60)) && start)
                {
                    start = 0;
                    read = 0;
                    arr[object].end = degrees;

                }
                if(read && (dist_Ping > 120) && (arr[object].start == -1)){
                    read = 0;
                    start = 0;
                    object--;
                }



                //Send all nonNoise values to Putty
                if(dist_Ping > 120)
                {
                    dist_Ping = 120;
                }
                if(dist_IR > 200)
                {
                    dist_IR = 200;
                }
                sprintf(str, "%-20d%-20d%-20d\r\n", degrees, dist_IR,
                                            dist_Ping);
                                    uart_sendStr(str);
            }
        }

        uart_sendStr("\r\n");
        sprintf(str, "%-15s%-15s%-15s%-15s\r\n", "Object", "Location", "Distance", "Width");
        uart_sendStr(str);
        //Object Calculations
        if(object == -1){return;}

        int k;
        int real = 0; //real_i = 0
        for (k = 0; k <= object; k++)
        {
            arr[k].rad_width = arr[k].end - arr[k].start;
            arr[k].location = arr[k].start + (arr[k].end - arr[k].start) / 2;

            double temporary = (arr[k].distance * arr[k].distance) + (arr[k].distance * arr[k].distance) - (2 * arr[k].distance * arr[k].distance * cos((arr[k].rad_width*3.14159)/180));

            arr[k].lin_width = pow(temporary, 0.5);
            if (arr[k].lin_width > 2)
            {
                real++;

                //uart_sendStr("\r\n");
                sprintf(str, "%-15d%-15d%-15d%-15d\r\n", real,
                        arr[k].location, arr[k].distance, arr[k].lin_width); //, arr[k].rad_width   %d
                uart_sendStr(str);
            }

        }

        servo_move(0);

        sprintf(str, "Number of Objects %d\r\n", real);
        uart_sendStr(str);
}
