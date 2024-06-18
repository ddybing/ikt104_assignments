#include "json.hpp"
#include <DFRobot_RGBLCD.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <mbed.h>

#ifndef __rtc__
#define __rtc__

using json = nlohmann::json;

class Clock {
private:
  char *rawResp;
  int currentTime;
  int timeZone;

public:
  Clock(char *rtc) { rawResp = rtc; };

  void printEpochTime(DFRobot_RGBLCD &lcd) {

    lcd.setCursor(0, 0);
    lcd.printf("Unix epoch time:");
    lcd.setCursor(0, 1);
    lcd.printf("%i", time(NULL));
  }

  int getHour() {
    time_t dateTime = time(NULL) + timeZone * 3600;
    char buffer[2];
    strftime(buffer, 2, "%H", localtime(&dateTime));
    return std::stoi(buffer);
  }

  int getMinute() {
    time_t dateTime = time(NULL) + timeZone * 3600;
    char buffer[2];
    strftime(buffer, 2, "%M", localtime(&dateTime));
    return std::stoi(buffer);
  }

  void printDatetime(DFRobot_RGBLCD &lcd) {
    time_t dateTime = time(NULL) + timeZone * 3600;
    char buffer[16];
    strftime(buffer, 16, "%a %d %h", localtime(&dateTime));
    lcd.setCursor(0, 0);
    lcd.printf("%s %02d:%02d", buffer, getHour(), getMinute());
  }

  void extractTime() {
    char *json_end_pointer = strrchr(rawResp, '}');
    char *json_start_pointer = strchr(rawResp, '{');

    json_end_pointer[1] = 0;

    auto j3 = json::parse(json_start_pointer);

    timeZone = j3["timezone_offset_with_dst"].get<int>();
    set_time(j3["date_time_unix"].get<int>());
  }
};

#endif