#include "TCPSocket.h"
#include "json.hpp"
#include "mbed.h"
#include "nsapi_types.h"
#include "wifi-ism43362/ISM43362Interface.h"
#include <HTS221Sensor.h>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>

WiFiInterface *wifi;
DevI2C sensor_dev(PB_11, PB_10);
HTS221Sensor hum_temp(&sensor_dev);

using json = nlohmann::json;

int main() {
  hum_temp.init(NULL);
  hum_temp.enable();
  float temp = 0.0f;
  float humid = 0.0f;

  wifi = WiFiInterface::get_default_instance();

  if (!wifi) {
    printf("Feil: kunne ikke koble til wifi.\n");
    return -1;
  }

  printf("\nKobler til  wifi %s...\n", MBED_CONF_APP_WIFI_SSID);

  // Kobler til wifi
  nsapi_size_or_error_t result =
      wifi->connect(MBED_CONF_APP_WIFI_SSID, MBED_CONF_APP_WIFI_PASSWORD,
                    NSAPI_SECURITY_WPA_WPA2);

  if (result != 0) {
    printf("\nTilkoblingsfeil %d\n", result);
    return -1;
  }

  TCPSocket socket;

  while (1) {
    socket.open(wifi);
    SocketAddress address;
    address.set_ip_address("172.16.42.248");
    address.set_port(9090);
    result = socket.connect(address);

    if (result != 0) {
      printf("Failed to connect to server: %d\n", result);
    }

    hum_temp.get_temperature(&temp);
    hum_temp.get_humidity(&humid);
    printf("Temp: %.1f Humidity: %.1f\n", temp, humid);
    char json[100];
    snprintf(json, 100, "{\"humidity\": %f,\"temperature\": %f}", humid, temp);
    char requestdata[1000];
    snprintf(requestdata, 1000,
             "POST /api/v1/hK7hYfbj0JPMh2GjCh9w/telemetry HTTP/1.1\r\n"
             "Host: 172.16.42.248\r\n"
             "Content-Type: application/json\r\n"
             "Content-Length: %u\r\n"
             "\r\n%s",
             strlen(json), json);

    result = socket.send(requestdata, 1000);

    printf("sendt og %i\n", result);
    //std::cout << std::endl;

    socket.close();
    thread_sleep_for(1000);
  }
  wifi->disconnect();
}