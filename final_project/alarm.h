#include <DFRobot_RGBLCD.h>
#include <cstdio>
#include <iostream>
#include <mbed.h>
#include "rtc.h"

#ifndef __ALARM__
#define __ALARM__

class Alarm {
private:
  bool isOn = false;
  bool isBeeping = false;
  int hours = 0;
  int minutes = 0;

public:
  Timer debounce;
 
  void setHours() {
    if (debounce.read_ms() == 0 || debounce.read_ms() > 200) {

      debounce.reset();
      debounce.start();

      hours++;
      if (hours == 24) {
        hours = 0;
      }
    }
  }

  void setMinutes() {
    if (debounce.read_ms() == 0 || debounce.read_ms() > 200) {

      debounce.reset();
      debounce.start();

      minutes++;
      if (minutes >= 60) {
        minutes = 0;
      }
    }
  }

  void setSnooze() {
    if (debounce.read_ms() == 0 || debounce.read_ms() > 200) {

      debounce.reset();
      debounce.start();
      if (isBeeping && isOn) {
        isBeeping = false;
        for (int i = 0; i < 5; i++) {
          setMinutes();
          if (minutes == 59) {
            setHours();
        }
        }
      }
    }
  }

  void toggleAlarm() {
    if (debounce.read_ms() == 0 || debounce.read_ms() > 200) {

      debounce.reset();
      debounce.start();
      isOn = !isOn;

     if (isBeeping) {
        isBeeping = false;
        }
    }
  }

  void alarm(PwmOut &buzzer, Clock clock) {
    if (isOn && minutes == clock.getMinute() && hours == clock.getHour()) {
      isBeeping = true;
    }
    if (isBeeping) {
        buzzer.write(0.5);
    } else {
        buzzer.write(0.0);
    }
  }

  void printAlarm(DFRobot_RGBLCD &lcd) {
    lcd.setCursor(0, 1);
    lcd.printf("Alarm ");
    isOn ? lcd.printf("ON  ") : lcd.printf("OFF ");
    lcd.printf("%02d:%02d", hours, minutes);
  }
};

#endif