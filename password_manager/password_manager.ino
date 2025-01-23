/** @file password_manager.ino */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "WiFiManager.h"      // Include WiFiManager class
#include <Keypad.h>
#include <Firebase.h>
#include "FirebaseHandler.h" // Include FirebaseHandler class



/** @brief LCD screen object. */
LCDScreen* lcd;

/** @brief Keypad object */
Keypad keypad(makeKeymap(keys), rowPins, colPins, 4, 4);

/** @brief System states */
enum { S_ID, S_PASS } state = S_ID;


// -----------------------------------------------------------------------------
// Firebase setup
// -----------------------------------------------------------------------------
/** @brief Wi-Fi SSID. */
const char* ssid = "Melek"; 

/** @brief Wi-Fi password. */
const char* pass = "melekdeling";

/** @brief Firebase database URL */
#define FIREBASE_HOST "https://password-entries-default-rtdb.europe-west1.firebasedatabase.app/"

/** @brief Firebase Auth key. */
#define FIREBASE_AUTH "7iTOn3J10Ob0mCVnUtG8dQY4Qmyc6kTBi926A6xa"

/** @brief WiFiManager object for handling the Wi-Fi connection. */
WiFiManager* wifiManager;

/** @brief FirebaseHandler object for Firebase operations. */
FirebaseHandler* firebase;

/** @brief Stores Box ID and entered password */
String boxID, password;
const int PASS_LEN = 4;


// -----------------------------------------------------------------------------
// Function prototypes
// -----------------------------------------------------------------------------
bool boxExists(const String &id);
void checkConnection();


// -----------------------------------------------------------------------------
// setup()
// -----------------------------------------------------------------------------
/**
 * @brief Main arduino setup function responsible for initializing objects, and hardware components.
 */
void setup() {

  Serial.begin(115200);
  lcd         = new LCDScreen();
  wifiManager = new WiFiManager(ssid, pass);
  firebase    = new FirebaseHandler(FIREBASE_HOST, FIREBASE_AUTH);

  wifiManager->connect();  // Connect to WiFi
  lcd->clear();
  lcd->printLine("Enter Box ID:", 1);

}

// -----------------------------------------------------------------------------
// loop()
// -----------------------------------------------------------------------------
/**
 * @brief Main arduino loop function. This runs constantly and allows for user interaction.
 */
void loop() {
  checkConnection();
  char k = keypad.getKey();
  if (!k) return;  // Do nothing if no key is pressed

  switch (state) {

    // ---------- ENTER BOX ID ----------
    case S_ID:
      if (k == '*') {
        // Clear Box ID
        boxID = "";
        lcd->clear();
        lcd->printLine("Enter Box ID:", 1);
      } 
      else if (k == '#') {
        // Validate ID
        if (boxExists(boxID)) {
          password = "";
          state = S_PASS;
          lcd->printLine("Enter Password:", 1);
        } else {
          // Show error briefly, then reset
          lcd->clear();
          lcd->printLine("Invalid Box ID!", 1);
          delay(2000);
          boxID = "";
          lcd->clear();
          lcd->printLine("Enter Box ID:", 1);
        }
      } else if (k == 'A') {
        // Validate ID
        if (!boxExists(boxID)) {
          password = "";
          state = S_PASS;
          lcd->clear();
          lcd->printLine("Enter 4-digit PW:", 1);
        } else {
          // Show error briefly, then reset
          lcd->printLines("Box ID already", "exists!");
          delay(2000);
          boxID = "";
          lcd->clear();
          lcd->printLine("Enter Box ID:");
        }
      } else {
        // Append character to Box ID
        boxID += k;
        lcd->printLine(boxID, 2);
      }
      break;

    // ---------- ENTER 4-DIGIT PASSWORD ----------
    case S_PASS:
      if (k == '*') {
        // Clear password
        password = "";
        lcd->clear();
        lcd->printLine("Enter 4-digit PW:", 1);
      } 
      else if (k == '#') {
        // Submit password only if it's exactly 4 digits
        if (password.length() == PASS_LEN) {
          lcd->printLines("Saving...", "Password: " + password);
          delay(2000);

          checkConnection();

          if (!firebase->savePassword("box" + boxID, password)) {  // Saving password unsuccessful
            lcd->printLines("UNABLE TO SAVE", "PASSWORD!");
            delay(3000);
          }

          // Reset to Box ID state
          boxID = "";
          password = "";
          state = S_ID;
          lcd->clear();
          lcd->printLine("Enter Box ID:", 1);
        } else {
          // Not enough (or too many) digits
          lcd->printLines("Need 4 digits!", "");
          delay(2000);
          lcd->clear();
          lcd->printLine("Enter 4-digit PW:", 1);
        }
      }
      else if (isDigit(k) && password.length() < PASS_LEN) {
        // Append digit if under 4
        password += k;
        lcd->print(password, 2);
      }
      // If the  password is already 4 digits long and the user enters more we just ignor it
      break;
  }
}

/**
 * @brief Checks If a box ID exists on the Firebase server.
 */
bool boxExists(const String &id) {
  checkConnection();
  lcd->clear();
  lcd->printLine("Loading...", 1);
  if (firebase->getPassword("box" + id) == "") return false; 
  return true;
}

/**
 * @brief Checks WiFi connection and attempts to reconnect if lost.
 */
void checkConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    wifiManager->connect(); 
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("WiFi lost; resetting...");
      ESP.restart();
    }
  }
}

