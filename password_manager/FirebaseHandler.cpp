#include "FirebaseHandler.h"
#include <ArduinoJson.h>

FirebaseHandler::FirebaseHandler(const String& host, const String& auth)
  : firebase(host, auth) {}

void FirebaseHandler::logEntry(const String& boxID, const String& entryStatus) {
  String path = "/entryLog/" + boxID;
  int result = firebase.pushString(path, entryStatus);

  // Check if the operation was successful for debugging DELETE LATER!!!!!!!!
  if (result == 200) {
    Serial.println("Entry successfully logged!");
  } else {
    Serial.print("Failed to log entry. Error code: ");
    Serial.println(result);
  }
}

bool FirebaseHandler::savePassword(const String& boxID, const String& password) {
  String path = "/passwords/" + boxID;
  int result = firebase.pushString(path, password);

  // Check if the operation was successful for debugging DELETE LATER!!!!!!!!
  if (result == 200) {
    Serial.println("Password successfully saved!");
    return true;
  } else {
    Serial.print("Failed to save password. Error code: ");
    Serial.println(result);
    return false;
  }
}

String FirebaseHandler::getPassword(const String& userID) {
  String path = "/passwords/" + userID;
  String json = firebase.getJson(path);

  if (json.length() == 0) {
    Serial.println("Failed to fetch JSON from Firebase.");
    return ""; 
  }

  StaticJsonDocument<1024> doc;
  DeserializationError error = deserializeJson(doc, json);

  if (error) {
    Serial.print("Failed to parse JSON: ");
    Serial.println(error.c_str());
    return ""; 
  }

  String latestValue;
  for (JsonPair kv : doc.as<JsonObject>()) {
    latestValue = kv.value().as<String>(); // Get the last value
  }

  return latestValue;
}
