/** @file WiFiManager.h
 *  @brief Declaration of the WiFiManager class for managing WiFi connections.
 */

#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <Arduino.h> ///< Arduino core library.
#include <ESP8266WiFi.h> ///< Library for managing WiFi connections on the ESP8266 module.

/**
 * @class WiFiManager
 * @brief A class to manage WiFi connections on the ESP8266.
 */
class WiFiManager {
public:
    /**
     * @brief Constructs a WiFiManager object with the given network credentials.
     * @param ssid The SSID of the WiFi network.
     * @param password The password of the WiFi network.
     */
    WiFiManager(const char* ssid, const char* password);

    /**
     * @brief Connects to the WiFi network using the provided credentials.
     * 
     * This method attempts to establish a WiFi connection by continuously checking
     * the connection status and prints the connection progress to the serial console.
     */
    void connect();

private:
    const char* ssid; ///< The SSID of the WiFi network.
    const char* password; ///< The password for the WiFi network.
};

#endif // WIFIMANAGER_H
