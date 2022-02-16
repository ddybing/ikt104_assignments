/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "/include/DFRobot_RGBLCD/DFRobot_RGBLCD.h"
#include <utility>

#define WAIT_TIME_MS 150

// Knapp fra venstre til høyre
InterruptIn button1(D10, PullUp);
InterruptIn button2(D11, PullUp);
InterruptIn button3(D13, PullUp);
InterruptIn button4(D12, PullUp);

// Buzzer
PwmOut buzzer(D6);

// global variables
bool muteSpeaker = true;
bool running = false;
float countdown = 60.0f;
float elapsedTime = 0.00f;
Timer timer;

// knapp fungsjonene
void toggleTimer() {
    if (running)
        timer.stop();
    else
        timer.start();
    running = !running;
}
void reset() {
    countdown = 60.0f;
    timer.stop();
    timer.reset();
    running = false;
    muteSpeaker = true;
}
void tilbake() {
    if (countdown < 0)
        countdown = 0;
    else
        countdown += 5.00f;
}
void frem() {
    countdown -= 5.00f;
}


int main() {

    // initializers
    DFRobot_RGBLCD lcd(16, 2, D14, D15);
    lcd.init();
    button1.fall(&toggleTimer);
    button2.fall(&reset);
    button3.fall(&tilbake);
    button4.fall(&frem);

    // buzzer settings
    float sound = 0.5f;
    int hz = 1000;
    float cycleTime = 1/(float)hz;
    buzzer.period(cycleTime);

    while (true)
    {
        thread_sleep_for(WAIT_TIME_MS);

        elapsedTime = countdown - ((float)timer.elapsed_time().count() / 1000000);

        if (elapsedTime <= 0.0f){
            timer.stop();
            muteSpeaker = false;
            elapsedTime = 0.00f;
        }

        // for debugging
        printf("Elapsed time: %.2fs\n", elapsedTime);

        // write to LCD
        lcd.clear();
        lcd.printf("Timer: %.2fs", elapsedTime);

        // toggle mute sound
        sound = (muteSpeaker) ? 0.00f : 0.5f; 
        buzzer.write(sound);
    }
}
