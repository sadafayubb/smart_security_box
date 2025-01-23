/** @file FirebaseHandler.cpp
 *  @brief Implementation of the FirebaseHandler class for interacting with Firebase.
 */

#include "FirebaseHandler.h"
#include <ArduinoJson.h>

/**
 * @brief Constructs a FirebaseHandler object.
 * @param host The Firebase host URL.
 * @param auth The Firebase authentication token.
 */
FirebaseHandler::FirebaseHandler(const String& host, const String& auth)
  : firebase(host, auth) {}

/**
 * @brief Logs an entry status to Firebase under the specified box ID.
 * @param boxID The ID of the lockbox.
 * @param entryStatus The status to log (e.g., "Unlocked", "Failed Attempt").
 */
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

/**
 * @brief Saves a password to Firebase under the specified box ID.
 * @param boxID The ID of the lockbox.
 * @param password The password to save.
 */
void FirebaseHandler::savePassword(const String& boxID, const String& password) {
  String path = "/passwords/" + boxID;
  int result = firebase.pushString(path, password);

  // Check if the operation was successful for debugging.
  if (result == 200) {
    Serial.println("Password successfully saved!");
  } else {
    Serial.print("Failed to save password. Error code: ");
    Serial.println(result);
  }
}

/**
 * @brief Retrieves the password associated with the given user ID from Firebase.
 * @param userID The ID of the user whose password is to be retrieved.
 * @return The retrieved password as a string. Returns an empty string if the operation fails.
 */
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
