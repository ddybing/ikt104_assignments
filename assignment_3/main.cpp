// Assignment 3 - Gruppe 9 - IKT104
#include <mbed.h>
#include <HTS221Sensor.h>
#include <DFRobot_RGBLCD.h>
#include <math.h>

InterruptIn button(D1, PullUp); // The pin which the button is connected to
#define WAIT_TIME_MS 500
DigitalOut led1(LED1);

DevI2C sensor_dev(PB_11, PB_10);
HTS221Sensor hum_temp(&sensor_dev); 

DFRobot_RGBLCD lcd(16, 2, D14, D15);

static BufferedSerial serial_port(USBTX, USBRX);
int buttonState = 0;
bool displayState = 0;
Timer debounce; // Timer for å måle tiden siden siste trykk på knappen.


// Funksjon som interrupt skal kjøre
void knapp() {

    // De to første if-statementene her er for debouncing (fjerne at knappen bytter state flere ganger ved et enkelt trykk)
    if (buttonState == 0)
    {
        debounce.start();
        buttonState = 1;
    }

    if (debounce.read_ms() > 200) 
    {
        debounce.stop();
        buttonState = 0;
        displayState = !displayState;
        printf("Knapp trykket");
    } 

    debounce.reset();

}
void lcdBackLightHumidity(float humidity) {

    int red = 255 - (255*humidity/100);
    int green = 255 - (255*humidity/100);

    lcd.setRGB(red,green,255);
}

void lcdBackLightTemp(float temp){
    if(temp < 20) {
        lcd.setRGB(0, 0, 255);
    }
    else if (temp <= 20 || temp <= 24) { 
        lcd.setRGB(240, 140, 0);
    }
    else if(temp > 24) {
      lcd.setRGB(255, 0, 0);
    }

}


int main() {
  float temp = 0.0f;
  float humid = 0.0f;
  lcd.init();
  lcd.noCursor();
  lcd.printstr("test");
  lcd.setRGB(255,255,0);
  hum_temp.init(NULL);
  hum_temp.enable();


  button.fall(&knapp); // Hvilken funksjon som skal kjøre når man trykker på knappen/interrupter.

  while(1) {
    hum_temp.get_temperature(&temp);
    hum_temp.get_humidity(&humid);
    printf("Temp: %.1f Humidity: %.1f\n", temp, humid); // Printer ut på skjermen til PCen ("serial").
    if (!displayState)
    {
        // Vis temperatur på displayet.
        lcd.clear(); // Fjern innhold på display
        lcd.printf("Temperature:");
        lcd.setCursor(0,1);
        lcd.printf("%.1f", temp);
        lcdBackLightTemp(temp);
    }
    
    else
    {
        // Vis luftfuktighet på displayet.
        lcd.clear(); // Fjern innhold på display
        lcd.printf("Humidity:");
        lcd.setCursor(0,1);
        lcd.printf("%.1f", humid);
        lcdBackLightHumidity(humid);
    }

    thread_sleep_for(1000); // Kjør loopen 1 gang per sekund
  }
}