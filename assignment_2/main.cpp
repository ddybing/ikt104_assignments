/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"


#define WAIT_TIME_MS 500 
DigitalIn button(D10); // The pin which the button is connected to 
AnalogIn pot(A5); // The pin which the potentiometer is connected to
float pot_value = 0.0f; // Holds the value of the potentiometer
static BufferedSerial serial_port(USBTX, USBRX); // Serial connection


PwmOut led1(D3);
PwmOut led2(D5);
PwmOut led3(D6);


int main()
{
    
    serial_port.set_baud(115200);
    serial_port.set_format(
        8,
        BufferedSerial::None,
        1
    );

    while (true)
    {
        pot_value = pot.read();
        if (button.read()==1)
        {
            // Her skal det være full brightness når knappen holdes nede
            led1.write(1.0f);
            led2.write(1.0f);
            led3.write(1.0f);
        }
        
        else
        {
            // Her skal det være potensiometeret som bestemmer
            if(pot_value < 0.01f)
            {
                led1.write(0.0f);
                led2.write(0.0f);
                led3.write(0.0f);
            }
            
            else if(pot_value < 0.33f)
            {
                led1.write(pot_value*3);
                led2.write(0.0f);
                led3.write(0.0f);
            }
            else if(pot_value < 0.66f)
            {
                led1.write(1.0f);
                led2.write((pot_value-0.33f)*3);
                led3.write(0.0f);
            }

            else
            {
                led1.write(1.0f);
                led2.write(1.0f);
                led3.write((pot_value-0.66f)*3.0f);
            }
        
        }
        thread_sleep_for(100); // Kjør loopen 10 ganger per sekund
    }
}
