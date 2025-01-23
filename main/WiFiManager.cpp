/** @file WiFiManager.cpp
 *  @brief Implementation of the WiFiManager class for managing WiFi connections.
 */

#include "WiFiManager.h"

/**
 * @brief Constructs a WiFiManager object with the given network credentials.
 * @param ssid The SSID of the WiFi network.
 * @param password The password of the WiFi network.
 */
WiFiManager::WiFiManager(const char* ssid, const char* password)
  : ssid(ssid), password(password) {}

/**
 * @brief Connects to the WiFi network using the provided credentials.
 * 
 * This function attempts to connect to the specified WiFi network and blocks
 * until the connection is established. Progress is logged to the serial console.
 */
void WiFiManager::connect() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
}
