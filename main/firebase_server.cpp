#include "firebase_server.h"

// Function to log an entry to Firebase
void logEntry(Firebase& firebase, const String& boxID, const String& entryStatus) {
  // Construct the path for the entry log
  String path = "/entryLog/" + boxID;

  // Push the entryStatus to Firebase under the constructed path
  int result = firebase.pushString(path, entryStatus);

  // Check if the operation was successful for debugging DELETE LATER!!!!!!!!
  if (result == 200) { // HTTP Status Code 200
    Serial.println("Entry successfully logged!");
  } else {
    Serial.print("Failed to log entry. Error code: ");
    Serial.println(result);
  }
}

// Function to save password to Firebase
void savePassword(Firebase& firebase, const String& boxID, const String& password) {
  // Construct the path for the password
  String path = "/passwords/" + boxID;

  // Push the password to Firebase under the constructed path
  int result = firebase.pushString(path, password);

  // Check if the operation was successful for debugging DELETE LATER!!!!!!!! maybe?
  if (result == 200) { // HTTP Status Code 200
    Serial.println("Password successfully saved!");
  } else {
    Serial.print("Failed to save password. Error code: ");
    Serial.println(result);
  }
}

// Function to get the latest password entry from Firebase
String getPassword(Firebase& firebase, const String& userID) {
  // Construct the path for the given userID
  String path = "/passwords/" + userID;

  // Retrieve all entries for the specified user
  String json = firebase.getJson(path);
  if (json.length() == 0) {
    Serial.println("Failed to fetch JSON from Firebase.");
    return ""; // Return an empty string to indicate failure
  }

  // Parse the JSON using ArduinoJson
  StaticJsonDocument<1024> doc;
  DeserializationError error = deserializeJson(doc, json);
  if (error) {
    Serial.print("Failed to parse JSON: ");
    Serial.println(error.c_str());
    return ""; // Return an empty string on parsing error
  }

  // Iterate through all key-value pairs to find the latest entry
  String latestValue;
  for (JsonPair kv : doc.as<JsonObject>()) {
    latestValue = kv.value().as<String>(); // Update to the current value
  }

  return latestValue; // Return the latest value found
}

