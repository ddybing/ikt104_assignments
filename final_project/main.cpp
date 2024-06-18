#include "./alarm.h"
#include "./tempHum.h"
#include "./wifi.h"
#include "include/newsfeed.h"
#include "mbed.h"
#include "rtc.h"
#include <DFRobot_RGBLCD.h>
#include <HTS221Sensor.h>
#include <cstdio>
#include <mbed_chrono.h>
#include "ipgeolocation_cert.h"
#include "weather.h"

// define humidity and temperature sensor
DevI2C sensor_dev(PB_11, PB_10);
HTS221Sensor hum_temp(&sensor_dev);

// define lcd diplay
DFRobot_RGBLCD lcd(16, 2, D14, D15);

// alarm buzzer
PwmOut buzzer(D6);

// define page button
InterruptIn button(BUTTON1, PullUp);

InterruptIn minuteButton(A0, PullUp);
InterruptIn hourButton(A1, PullUp);
InterruptIn alarmButton(A2, PullUp);
InterruptIn snoozeButton(A3, PullUp);

Timer debounce;

int buttonState = 0;
int displayState = 1;

void knapp() {
  if (debounce.read_ms() == 0 || debounce.read_ms() > 200) {

    debounce.reset();
    debounce.start();

    displayState++;
    if (displayState > 4) {
      displayState = 1;
    }
  }
}

int main() {

  // initialize lcd display
  lcd.init();
  lcd.noCursor();
  lcd.clear();

  HumTemp humTemp;
  Alarm alarm;
  Wifi wifi;

  wifi.connect_wifi();

  Clock clock(wifi.getContentByHTTPS("api.ipgeolocation.io", 443, "/timezone?apiKey=APIKEY", 1000, ipgeolocationCert));

  clock.extractTime();

  Newsfeed newsfeed(wifi.getContent("feeds.bbci.co.uk", 80, "/news/world/rss.xml", 20000));

  newsfeed.extractHeadlines();

  Weather weather(wifi.getContent("api.openweathermap.org", 80, "/data/2.5/weather?lat=58&lon=8&appid=APPID", 1000));
    weather.extractWeather();

  // initialize humidity and temperature sensor
  hum_temp.init(NULL);
  hum_temp.enable();

  int page = 1;
  int tempDisplayState = 0;

  button.fall(&knapp);
  minuteButton.fall(callback(&alarm, &Alarm::setMinutes));
  hourButton.fall(callback(&alarm, &Alarm::setHours));
  alarmButton.fall(callback(&alarm, &Alarm::toggleAlarm));
  snoozeButton.fall(callback(&alarm, &Alarm::setSnooze));

  for (int i = 0; i < 5; i++) {
    clock.printEpochTime(lcd);
    thread_sleep_for(1000);
  }

  while (true) {
    if (tempDisplayState != displayState) {
      lcd.clear();
      tempDisplayState = displayState;
    }

    alarm.alarm(buzzer, clock);

    switch (displayState) {
    case 1: {
      clock.printDatetime(lcd);
      alarm.printAlarm(lcd);
      break;
    }
    case 2: {
      humTemp.printTemperature(hum_temp, lcd);
      humTemp.printHumidity(hum_temp, lcd);

      break;
    }
    case 3: {
      weather.printWeather(lcd);
      break;
    }
    case 4: {
      newsfeed.printNews(lcd, displayState);
      break;
    }
    default: {
      break;
    }
    }
    thread_sleep_for(200);
  }
}