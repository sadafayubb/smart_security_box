/** @file WiFiManager.cpp */

#include "WiFiManager.h"

WiFiManager::WiFiManager(const char* ssid, const char* password)
  : ssid(ssid), password(password) {}

void WiFiManager::connect() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
}

