#ifndef __wifi__
#define __wifi__
#include "TCPSocket.h"
#include "nsapi_types.h"
#include "wifi-ism43362/ISM43362Interface.h"
#include <cstdio>
#include <string>


class Wifi {
public:
  WiFiInterface *wifi;
  TCPSocket socket;
    TLSSocket tls_socket;
  SocketAddress address;

  void connect_wifi() {
    wifi = WiFiInterface::get_default_instance();

    if (!wifi) {
      printf("Feil: kunne ikke koble til wifi.\n");
      return;
    }

    printf("\nKobler til  wifi %s...\n\n", MBED_CONF_APP_WIFI_SSID);

    nsapi_size_or_error_t result =
        wifi->connect(MBED_CONF_APP_WIFI_SSID, MBED_CONF_APP_WIFI_PASSWORD,
                      NSAPI_SECURITY_WPA_WPA2);

    if (result != 0) {
      printf("\nTilkoblingsfeil %d\n", result);
      return;
    }

    printf("Koblet til wifi!\n");
  }

  char *getContent(const char *host, int port, char *path, int bufferSize) {
      
    socket.open(wifi);

    nsapi_size_or_error_t result = wifi->gethostbyname(host, &address);

    while (result != 0) {
      printf("Failed to get IP address of host: %d\n", result);
      result = wifi->gethostbyname(host, &address);
      thread_sleep_for(3000);
    }

    printf("Success to get IP address of host: %s\n", host);

    address.set_port(port);
    result = socket.connect(address);

    if (result != 0) {
      printf("Failed to connect to server: %d\n", result);
      return nullptr;
    }

    char requestdata[1000];
    snprintf(requestdata, 1000,
             "GET %s HTTP/1.1\r\n"
             "Host: %s\r\n"
             "Connection: close\r\n"
             "\r\n",
             path, host);

    result = socket.send(requestdata, 1000);

    char *response = new char[bufferSize];
    // denne størrelen må være større eller lik en totale response størrelsen
    int remaining_bytes = bufferSize;
    int received_bytes = 0;

    while (remaining_bytes > 0) {

      nsapi_size_or_error_t result =
          socket.recv(response + received_bytes, 100);

      if (result == 0) {
        printf("Done receiving data, of host: %s\n", host);
        break;
      }

      if (result < 0) {
        printf("No data received\n");
        return nullptr;
      }

      received_bytes += result;
      remaining_bytes -= result;
    }

    socket.close();

    return response;
  }


char *getContentByHTTPS(const char *host, int port, char *path, int bufferSize, const char certificate[]) {
      
    tls_socket.open(wifi);

    nsapi_size_or_error_t result = wifi->gethostbyname(host, &address);

    while (result != 0) {
      printf("Failed to get IP address of host: %d\n", result);
      result = wifi->gethostbyname(host, &address);
      thread_sleep_for(3000);
    }

    printf("Success to get IP address of host: %s\n", host);

    address.set_port(port);

    result = tls_socket.set_root_ca_cert(certificate);

    result = tls_socket.connect(address);

    if (result != 0) {
      printf("Failed to connect to server: %d\n", result);
      return nullptr;
    }

    char requestdata[1000];
    snprintf(requestdata, 1000,
             "GET %s HTTP/1.1\r\n"
             "Host: %s\r\n"
             "Connection: close\r\n"
             "\r\n",
             path, host);

    

    result = tls_socket.send(requestdata, 1000);

    char *response = new char[bufferSize];
    // denne størrelen må være større eller lik en totale response størrelsen
    int remaining_bytes = bufferSize;
    int received_bytes = 0;

    while (remaining_bytes > 0) {

      nsapi_size_or_error_t result =
          tls_socket.recv(response + received_bytes, 100);

      if (result == 0) {
        printf("Done receiving data, of host: %s\n", host);
        break;
      }

      if (result < 0) {
        printf("No data received\n");
        return nullptr;
      }

      received_bytes += result;
      remaining_bytes -= result;
    }

    tls_socket.close();

    return response;
  }
};
#endif