/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"


#define MAX_BUFF_SIZE 32

static BufferedSerial serial_port(USBTX, USBRX);

#define WAIT_TIME_MS 500
DigitalOut led1(LED1);

int main()
{
    // This code sets up the serial connection
    serial_port.set_baud(115200);
    serial_port.set_format(
        8,
        BufferedSerial::None,
        1
    );

    char serialBuffer[1] = {0};
    DigitalOut led1(D6);


    while (true)
    {
        serial_port.read(serialBuffer,1);
        if (*serialBuffer=='0')
        {
            led1.write(0);
        }

        if (*serialBuffer=='1')
        {
            led1.write(1);
        }
    }
}
