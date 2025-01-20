#ifndef FIREBASEHANDLER_H
#define FIREBASEHANDLER_H

#include <Firebase.h>  // Include Firebase library

class FirebaseHandler {
public:
    FirebaseHandler(const String& host, const String& auth);
    void logEntry(const String& boxID, const String& entryStatus);
    bool savePassword(const String& boxID, const String& password);
    String getPassword(const String& userID);

private:
    Firebase firebase;
};

#endif // FIREBASEHANDLER_H
