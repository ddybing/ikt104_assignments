#include "json.hpp"
#include <DFRobot_RGBLCD.h>
#include <mbed.h>


using json = nlohmann::json;

#ifndef __Weather__
#define __Weather__

class Weather {
private:
  char *rawResp;
  string weather;
  float temp;

public:
  Weather(char *weather) { rawResp = weather; };

  void printWeather(DFRobot_RGBLCD &lcd) {
      lcd.setCursor(0, 0);
      lcd.printf("%s", weather.c_str());
      lcd.setCursor(0, 1);
      lcd.printf("%.0f degrees", temp);
    }

  void extractWeather() {
    char *json_end_pointer = strrchr(rawResp, '}');
    char *json_start_pointer = strchr(rawResp, '{');

    json_end_pointer[1] = 0;

    auto j3 = json::parse(json_start_pointer);
    weather = j3["weather"][0]["description"].get<string>();
    temp = j3["main"]["temp"].get<float>() - 273.15;
  }
};

#endif