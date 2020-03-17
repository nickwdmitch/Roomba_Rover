/*
 * scan2.c
 *
 *  Created on: Apr 30, 2019
 *      Author:  Michael Ostrow, Nickolas Mitchell, Jacob Puetz, and Evan Christensen
 */

#include "scan2.h"

struct object{
    int distance;
    int location;
    float width;
    int startVal;
    int endVal;
};


//function to
void analyzeData(int IRData[], double PingData[])  {
    struct object objects[30];
    int numObj = 0;
    int found = 0;
    int i = 0;
    char str[41];

    for (i = 0; i <= 90; i++)  {
        if ((IRData[i] < 100) && (!found))  {
            objects[numObj].startVal = i * 2;
            objects[numObj].distance = PingData[i];
            found = 1;
        }
        else if (found && (IRData[i] > 100))  {
            objects[numObj].endVal = (i - 1) * 2;
            found = 0;
            //objects[numObj].distance = PingData[i];
            numObj = numObj + 1;
        }
    }

    int real = 0; //real_i = 0
    sprintf(str, "\n\r%-15s%-15s%-15s%-15s\r\n", "Object", "Location", "Distance", "Width");
    uart_sendStr(str);
    if (numObj != 0)
    {
        for (i = 0; i <= numObj; i++)
        {
            int radialWidth = objects[i].endVal - objects[i].startVal;
            objects[i].location = objects[i].startVal
                    + (objects[i].endVal - objects[i].startVal) / 2;

            double temporary = (objects[i].distance * objects[i].distance)
                    + (objects[i].distance * objects[i].distance)
                    - (2 * objects[i].distance * objects[i].distance
                            * cos((radialWidth * 3.14159) / 180));

            objects[i].width = (pow(temporary, 0.5));
            if (objects[i].width > 3 && objects[i].location < 1000)
            {
                real++;
                sprintf(str, "%-15d%-15d%-15d%-15.2f\r\n", real,
                        objects[i].location, objects[i].distance,
                        objects[i].width);
                uart_sendStr(str);
            }

        }
    }
}

void scan2(void)
{
    //Ping variables
    int done_flag, time_start, time_end, time_cycle, dist_Ping;
    //Initializes the ping sensor
    ping_init(&done_flag, &time_start, &time_end);
    servo_move(0);
    int angle = 0;
    int IRData[91];
    double PingData[91];
    //gets everything ready to send via uart
    char begin[] = "Degrees     IR Distance (cm)    SonarDistance (cm)\n\r";
    char output[50];
    //wait for the servo to get to 0
    timer_waitMillis(100);
    //send the first line to uart
    uart_sendStr(begin);
    //move servo 180 degrees and send data to putty via uart, saves data to arrays for analysis
    while (angle <= 180)  {
        servo_move(angle);
        timer_waitMillis(10);
        int w;
        int sum = 0;
        for (w = 0; w < 15; w++)  {
            sum += adc_read();
        }
        sum = sum / 15;
        IRData[angle/2] = 35581065.7721 * pow(sum, -1.9172);    //Cybot: #8: 35581065.7721 * pow(sum, -1.9172);        **Cybot #7: 4000000*pow(sum,-1.635)

        //Ping collects data
        ping_read();
        timer_waitMillis(50);

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

        PingData[angle/2] = dist_Ping;
        sprintf(output, "%3d         %10d          %10.0f \r\n", angle, IRData[angle/2], PingData[angle/2]);
        uart_sendStr(output);
        angle += 2;
    }
    servo_move(0);
    analyzeData(IRData, PingData);
}



