/** @file FirebaseHandler.h
 *  @brief Declaration of the FirebaseHandler class for interacting with Firebase.
 */

#ifndef FIREBASEHANDLER_H
#define FIREBASEHANDLER_H

#include <Firebase.h>  // Include Firebase library

/**
 * @class FirebaseHandler
 * @brief A class to handle Firebase operations such as logging entries and managing passwords.
 */
class FirebaseHandler {
public:
    /**
     * @brief Constructs a FirebaseHandler object.
     * @param host The Firebase host URL.
     * @param auth The Firebase authentication token.
     */
    FirebaseHandler(const String& host, const String& auth);

    /**
     * @brief Logs an entry status to Firebase under the specified box ID.
     * @param boxID The ID of the lockbox.
     * @param entryStatus The status to log (e.g., "Unlocked", "Failed Attempt").
     */
    void logEntry(const String& boxID, const String& entryStatus);

    /**
     * @brief Saves a password to Firebase under the specified box ID.
     * @param boxID The ID of the lockbox.
     * @param password The password to save.
     */
    void savePassword(const String& boxID, const String& password);

    /**
     * @brief Retrieves the password associated with the given user ID from Firebase.
     * @param userID The ID of the user whose password is to be retrieved.
     * @return The retrieved password as a string. Returns an empty string if the operation fails.
     */
    String getPassword(const String& userID);

private:
    Firebase firebase; ///< An instance of the Firebase class used to interact with Firebase.
};

#endif // FIREBASEHANDLER_H
