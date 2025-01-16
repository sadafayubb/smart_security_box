#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <ESP8266WiFi.h>  // Include Wi-Fi library

class WiFiManager {
public:
    WiFiManager(const char* ssid, const char* password);
    void connect(); // Connect to Wi-Fi

private:
    const char* ssid;
    const char* password;
};

#endif // WIFIMANAGER_H
