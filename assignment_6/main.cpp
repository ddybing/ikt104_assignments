#include "mbed.h"
#include "wifi-ism43362/ISM43362Interface.h"
#include "TCPSocket.h"
#include "nsapi_types.h"
#include "json.hpp"
#include <cstdio>
#include <cstring>
#include <iostream>
#include <ostream>
#include <string>


WiFiInterface *wifi;

using json = nlohmann::json;

int main()
{
    wifi = WiFiInterface::get_default_instance();

    if (!wifi) {
        printf("Feil: kunne ikke koble til wifi.\n");
        return -1;
    }

    printf("\nKobler til  wifi %s...\n\n", MBED_CONF_APP_WIFI_SSID);

    // Kobler til wifi
    nsapi_size_or_error_t result = wifi->connect(MBED_CONF_APP_WIFI_SSID, MBED_CONF_APP_WIFI_PASSWORD, NSAPI_SECURITY_WPA_WPA2);

    if (result != 0) {
        printf("\nTilkoblingsfeil %d\n", result);
        return -1;
    }

    TCPSocket socket;
    socket.open(wifi);
    SocketAddress address;

    result = wifi->gethostbyname("www.mocky.io", &address);

    if (result != 0) {
        printf("Failed to get IP address of host: %d\n", result);
        while (1);
    }

    address.set_port(80);
    result = socket.connect(address);

    if (result != 0) {
        printf("Failed to connect to server: %d\n", result);
        while (1);
    }

    result = socket.send(
        "GET /v2/5e37e64b3100004c00d37d03 HTTP/1.1\r\n"
        "Host: www.mocky.io\r\n"
        "Connection: close\r\n"
        "\r\n"
        , 1000
    );

    char response[1000];
    // denne størrelen må være større eller lik en totale response størrelsen
    int remaining_bytes = 1000;
    int received_bytes = 0;

    while (remaining_bytes > 0) {

        // bestemmer responens minneplass og hvor mye bytes som skal returners av gangen
        nsapi_size_or_error_t result = socket.recv(response + received_bytes, 100);

        if (result == 0) {
            break;
        }

        if (result < 0) {
            return result;
        }

        received_bytes += result;
        remaining_bytes -= result;

        // separerer den totale responsen hver 100. bytes
        printf("%s \n...\n ", (response + received_bytes)-100);
    }


    char *json_start_pointer = strchr(response, '{');
 
    for (int i = strlen(response); i < 0; i--) {
        if (response[i] == '{') {
            break;
        } else {
            response[i] = '\0';
        }
    }

    auto j3 = json::parse(json_start_pointer, nullptr, false);

    // Printer ut info
    std::cout << "\nFirst name: " << j3.at("first name").get<std::string>() << std::endl;
    std::cout << "Last name: " << j3.at("last name").get<std::string>() << std::endl;
    std::cout << "Age: " << j3.at("age").get<int>() << std::endl;
    
    wifi->disconnect();
}