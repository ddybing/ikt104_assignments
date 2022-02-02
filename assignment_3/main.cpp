// Assignment 3 - Gruppe 9 - IKT104
#include <mbed.h>
#include <HTS221Sensor.h>
#include <DFRobot_RGBLCD.h>
#include <math.h>


InterruptIn button(D1, PullUp); // The pin which the button is connected to
#define WAIT_TIME_MS 500
DigitalOut led1(LED1);


I2C2 devsens(PB10, PB11);

HTS221Sensor hum_tempsensor();

DFRobot_RGBLCD lcd(16, 2, D14, D15);

static BufferedSerial serial_port(USBTX, USBRX);

// Funksjon som interrupt skal kjøre
void knapp() {
  led1 = !led1;
}
void lcdBackLightHumidity(int humidity) {
    
}
void lcdBackLightTemp(int temp){
    if(temp < 20) {
        lcd.setRGB(0, 0, 255);
    }
    if(temp >= 20 || temp <= 24) { 
        lcd.setRGB(240, 120, 0);
    }
    if(temp > 24) {
      lcd.setRGB(255, 0, 0);
    }

}
  
  
int main() {
  lcd.init();
  lcd.noCursor();
  lcd.printstr("hmmm");
  lcd.setRGB(255,255,0);
  button.fall(&knapp); // Hvilken funksjon som skal kjøre når man trykker på knappen/interrupter.
  // put your setup code here, to run once:
    
  while(1) {
    thread_sleep_for(1000); // Kjør loopen 10 ganger per sekund
  }
}