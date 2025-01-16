#ifndef FIREBASE_SERVER_H
#define FIREBASE_SERVER_H

#include <Firebase.h>
#include <ArduinoJson.h>

void logEntry(Firebase& firebase, const String& boxID, const String& entryStatus);

void savePassword(Firebase& firebase, const String& boxID, const String& password);

String getPassword(Firebase& firebase, const String& userID);

#endif // FIREBASE_SERVER_H
