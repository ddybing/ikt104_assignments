#include <mbed.h>
#include <HTS221Sensor.h>
#include <DFRobot_RGBLCD.h>

#ifndef __HUM_TEMP__
#define __HUM_TEMP__

class HumTemp {
public:
    void printTemperature(HTS221Sensor &hum_temp, DFRobot_RGBLCD &lcd) {
        float temp = 0.0f;

        hum_temp.get_temperature(&temp);

        lcd.setCursor(0,0);
        lcd.printf("Temp:");
        
        lcd.printf("%.1f", temp);
    }

    void printHumidity(HTS221Sensor &hum_temp, DFRobot_RGBLCD &lcd) {
        float humid = 0.0f;

        hum_temp.get_humidity(&humid);

        lcd.setCursor(0,1);
        lcd.printf("Humi:");
        
        lcd.printf("%.1f", humid);
    }
};

#endif