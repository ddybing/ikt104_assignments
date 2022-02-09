// IKT104 - Assignment 4 - Gruppe 9

#include "mbed.h"
#include <DFRobot_RGBLCD.h>

#define WAIT_TIME_MS 100 
DigitalOut led1(LED1);
InterruptIn button1(D1,PullUp); // Button 1, black button on the breadboard .
InterruptIn button2(D8, PullUp); // Button 2, the blue button on the breadboard.

DFRobot_RGBLCD lcd(16, 2, D14, D15); // Set up display

Timer tid; // Set up timer

bool auto_kick = 0;

void toggle_timer()
{
    if (auto_kick == 0)
    {
        tid.stop(); // Stop timer
        
    }

    else 
    {
        tid.start(); // Start timer
    }

    auto_kick = !auto_kick; // Toogle auto_kick 
}

void spark_bikkja()
{
    Watchdog::get_instance().kick();
}

int main()
{
    lcd.init(); // Initialise display
    lcd.noCursor();

    Watchdog &watchdog = Watchdog::get_instance(); // Set up watchdog
    watchdog.start(10000); // Start watchdog

    button1.fall(&toggle_timer);
    button2.fall(&spark_bikkja);

    
    tid.start(); // Start the timer
    while (true)
    {
        if (auto_kick == 1)
        {
            watchdog.kick();
        }

        // Updates display only if timer is running. 
        else {        
        printf("%.2f\n", tid.elapsed_time().count()/(float)1000000);
        lcd.clear();
        lcd.printf("%.2f", tid.elapsed_time().count()/(float)1000000);

        }
        thread_sleep_for(WAIT_TIME_MS);
    }
}
