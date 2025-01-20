#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "WiFiManager.h"      // Include WiFiManager class
#include <Keypad.h>
#include <Firebase.h>
#include "FirebaseHandler.h" // Include FirebaseHandler class



// ----- LCD and Keypad Setup -----
LiquidCrystal_I2C lcd(0x27, 16, 2);

char keys[4][4] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// Match these to the actual GPIOs you used for the rows:
byte rowPins[4] = {2, 3, 9, 10};  // <-- Example wiring to GPIO4, GPIO5, GPIO12, GPIO13

// Match these to the actual GPIOs you used for the columns:
byte colPins[4] = {12, 13, 14, 15};  // <-- Example wiring to GPIO14, GPIO2, GPIO3, GPIO15


//byte rowPins[4] = {13,12,11,9}, 
//colPins[4] = {15,14,3,2};
Keypad keypad(makeKeymap(keys), rowPins, colPins, 4, 4);

// ----- States -----
enum { S_ID, S_PASS } state = S_ID;


// ----- Firesbase setup -----

const char* ssid = "iPhone von Frederik"; // Replace with your Wi-Fi name
const char* pass = "abcdefg123456"; // Replace with your Wi-Fi password

#define FIREBASE_HOST "https://password-entries-default-rtdb.europe-west1.firebasedatabase.app/"  // Replace with your Firebase Realtime Database URL
#define FIREBASE_AUTH "7iTOn3J10Ob0mCVnUtG8dQY4Qmyc6kTBi926A6xa"                 // Replace with your Firebase Database Secret or API Key

// Declare WiFiManager and FirebaseHandler objects
WiFiManager* wifiManager;
FirebaseHandler* firebase;

// ----- Variables -----
String boxID, password;
String validIDs[] = {"123", "4567", "9999"};  // Example valid IDs (This is just for now until its connected with Firesbase)
const int PASS_LEN = 4;


// ----- Function Prototypes -----
bool boxExists(const String &id);
void checkConnection();


void setup() {

  Serial.begin(115200);
  wifiManager = new WiFiManager(ssid, pass);
  firebase = new FirebaseHandler(FIREBASE_HOST, FIREBASE_AUTH);

  wifiManager->connect();  // Connect to WiFi

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("Enter Box ID:");

}

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
        lcd.clear();
        lcd.print("Enter Box ID:");
      } 
      else if (k == '#') {
        // Validate ID
        if (boxExists(boxID)) {
          password = "";
          state = S_PASS;
          lcd.clear();
          lcd.print("Enter Password:");
        } else {
          // Show error briefly, then reset
          lcd.clear();
          lcd.print("Invalid Box ID!");
          delay(2000);
          boxID = "";
          lcd.clear();
          lcd.print("Enter Box ID:");
        }
      } else if (k == 'A') {
        // Validate ID
        if (!boxExists(boxID)) {
          password = "";
          state = S_PASS;
          lcd.clear();
          lcd.print("Enter 4-digit PW:");
        } else {
          // Show error briefly, then reset
          lcd.clear();
          lcd.print("Box ID already");
          lcd.setCursor(0, 1);
          lcd.print("exists!");
          delay(2000);
          boxID = "";
          lcd.clear();
          lcd.print("Enter Box ID:");
        }
      } else {
        // Append character to Box ID
        boxID += k;
        lcd.setCursor(0, 1);
        lcd.print(boxID);
      }
      break;

    // ---------- ENTER 4-DIGIT PASSWORD ----------
    case S_PASS:
      if (k == '*') {
        // Clear password
        password = "";
        lcd.clear();
        lcd.print("Enter 4-digit PW:");
      } 
      else if (k == '#') {
        // Submit password only if it's exactly 4 digits
        if (password.length() == PASS_LEN) {
          lcd.clear();
          lcd.print("Saving...");
          lcd.setCursor(0, 1);
          lcd.print("Password: " + password);
          delay(2000);

          checkConnection();

          if (!firebase->savePassword("box" + boxID, password)) {  // Saving password unsuccessful
            lcd.clear();
            lcd.print("UNABLE TO SAVE");
            lcd.setCursor(0, 1);
            lcd.print("PASSWORD!");
            delay(3000);
          }

          // Reset to Box ID state
          boxID = "";
          password = "";
          state = S_ID;
          lcd.clear();
          lcd.print("Enter Box ID:");
        } else {
          // Not enough (or too many) digits
          lcd.clear();
          lcd.print("Need 4 digits!");
          delay(2000);
          lcd.clear();
          lcd.print("Enter 4-digit PW:");
        }
      }
      else if (isDigit(k) && password.length() < PASS_LEN) {
        // Append digit if under 4
        password += k;
        lcd.setCursor(0, 1);
        lcd.print(password);
      }
      // If the  password is already 4 digits long and the user enters more we just ignor it
      break;
  }
}

// ----- Check if Box ID is Valid -----
bool boxExists(const String &id) {
  checkConnection();
  lcd.clear();
  lcd.print("Loading...");
  if (firebase->getPassword("box" + id) == "") return false; 
  return true;
}

// ----- Check WiFi Conncetion -----
void checkConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    wifiManager->connect(); 
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("WiFi lost; resetting...");
      ESP.restart();
    }
  }
}

