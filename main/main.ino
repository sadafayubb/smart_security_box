#include <Arduino.h>
#include "buzzer.h"
#include "LCD_screen.h"
#include "constants.h"
#include "motor_module.h"
#include "LEDs.h"
#include <Firebase.h>
#include "FirebaseHandler.h" // Include FirebaseHandler class
#include "WiFiManager.h"      // Include WiFiManager class



// -----------------------------------------------------------------------------
// Firesbase setup
// -----------------------------------------------------------------------------
const char* ssid = "iPhone von Frederik"; // Replace with your Wi-Fi name
const char* pass = "abcdefg123456"; // Replace with your Wi-Fi password

#define FIREBASE_HOST "https://password-entries-default-rtdb.europe-west1.firebasedatabase.app/"  // Replace with your Firebase Realtime Database URL
#define FIREBASE_AUTH "7iTOn3J10Ob0mCVnUtG8dQY4Qmyc6kTBi926A6xa"                 // Replace with your Firebase Database Secret or API Key

// Declare WiFiManager and FirebaseHandler objects
WiFiManager* wifiManager;
FirebaseHandler* firebase;

// -----------------------------------------------------------------------------
// Objects for LCD Screen and Buzzer
// -----------------------------------------------------------------------------
LCDScreen* lcd;
Buzzer*    buzzer;
LEDs*      leds;

// -----------------------------------------------------------------------------
// Global State
// -----------------------------------------------------------------------------
enum State state = IDLE;

// -----------------------------------------------------------------------------
// Pin Definitions
// -----------------------------------------------------------------------------
const int potPin        = A0;  // Potentiometer connected to A0
const int buttonPin     = 16;  // Button connected to D13
const int buzzerPin     = 15;  
const int greenLEDPin   = 14;
const int yellowLEDPin  = 12;
const int redLEDPin     = 13;
const int servoMotorPin = 2;

// -----------------------------------------------------------------------------
// Global Variables
// -----------------------------------------------------------------------------
int lowerBounds[10];
int upperBounds[10];
String boxID            = "004";
int selectedNumbers[4]  = { -1, -1, -1, -1 }; // Stores user input numbers
int codeIndex           = 0;                  // Tracks which digit the user is selecting
int attemptsUsed        = 0;                  // Tracks number of failed attempts
int selectedNumber      = 0;                  // Current selected number from Pot
const int maxAttempts   = 3;                  // Maximum allowed attempts
bool firstReading = true;
unsigned long lastInteractionTime = 0;  // Track last user interaction
const unsigned long timeoutDuration = 60000;  // 1 minute in milliseconds
const unsigned long resetDuration = 1800000;  // 30 minutes in milliseconds
int correctCode[4];  

// -----------------------------------------------------------------------------
// Function Prototypes
// -----------------------------------------------------------------------------
void chooseNumber(unsigned long &lastInteractionTime);
void checkCode();
void resetSystem();
int readAnalogValue();

// -----------------------------------------------------------------------------
// setup()
// -----------------------------------------------------------------------------
void setup() {
    // Instantiate objects
    lcd    = new LCDScreen();
    buzzer = new Buzzer(buzzerPin);
    leds = new LEDs(greenLEDPin, yellowLEDPin, redLEDPin);
    wifiManager = new WiFiManager(ssid, pass);
    firebase = new FirebaseHandler(FIREBASE_HOST, FIREBASE_AUTH);

    wifiManager->connect();  // Connect to WiFi


    // Initialize Motor
    setupMotor(servoMotorPin);
    setLock(false);

    // Configure pins
    pinMode(buttonPin, INPUT_PULLUP);  // Internal pull-up resistor for button
    pinMode(buzzerPin, OUTPUT);
    pinMode(greenLEDPin, OUTPUT);
    pinMode(yellowLEDPin, OUTPUT);
    pinMode(redLEDPin, OUTPUT);

    digitalWrite(yellowLEDPin, HIGH);

    delay(3000);

    // Start Serial (for debugging)
    Serial.begin(115200);

    // Update LCD and LEDs to show IDLE state
    updateBoxLCD(lcd, state, selectedNumbers, selectedNumber);
    leds->update(state);
}

// -----------------------------------------------------------------------------
// loop()
// -----------------------------------------------------------------------------
void loop() {
    // Reset system if inactive for 2 minutes, unless in ACCESS_DENIED mode
    if (state != ACCESS_DENIED && state != IDLE && state != BOX_OPEN && millis() - lastInteractionTime > timeoutDuration) {
        Serial.println("Inactivity timeout: Resetting system...");
        resetSystem();
    } else if (state == IDLE && attemptsUsed > 0 && millis() - lastInteractionTime > resetDuration) {
        Serial.println("Resetting attempt count...");
        attemptsUsed = 0;
    }

    // If ready for the user to make input
    if (state == IDLE || state == ENTERING_PASSWORD) {
        chooseNumber(lastInteractionTime);
        if (codeIndex == 4) {
            checkCode();
        }
    }

    if (state == BOX_OPEN && digitalRead(buttonPin) == LOW) {
        delay(200);
        while (digitalRead(buttonPin) == LOW);
        delay(200);
        buzzer->boxClose();
        resetSystem();
        Serial.println("Box closed and system reset.");
    }

    //if (state == CORRECT_PASSWORD && digitalRead(buttonPin) == LOW) {
    //    delay(200);
    //    while (digitalRead(buttonPin) == LOW);
    //    delay(200);
    //    resetSystem();
    //    Serial.println("Access granted state interrupted and system reset.");
    //}
}

// -----------------------------------------------------------------------------
// resetSystem()
// -----------------------------------------------------------------------------
void resetSystem() {
    codeIndex = 0;
    //attemptsUsed = 0;

    for (int i = 0; i < 4; i++) {
        selectedNumbers[i] = -1;
    }

    state = IDLE;
    setLock(false);
    updateBoxLCD(lcd, state, selectedNumbers, selectedNumber);
    leds->update(state);
    Serial.println("System reset due to inactivity.");
}


// -----------------------------------------------------------------------------
// readAnalogValue()
// -----------------------------------------------------------------------------
int readAnalogValue() {

  int sensorValue       = analogRead(potPin);
  int digit             = map(sensorValue, 24, 1023, 0, 9);

  //// Determine the current digit
  //int calculatedDigit = 9 - (sensorValue / (1023 / 9));
  //
  //// Fetch precomputed bounds
  //int lowerBound = lowerBounds[calculatedDigit];
  //int upperBound = upperBounds[calculatedDigit];

  //// Verify if the sensor value is in the middle range for stability
  //if (sensorValue >= lowerBound && sensorValue <= upperBound) {
  //  return calculatedDigit;  // Return valid digit
  //}

  return digit;

}


// -----------------------------------------------------------------------------
// chooseNumber()
// Handles number selection using potentiometer and button confirmation
// -----------------------------------------------------------------------------
void chooseNumber(unsigned long &lastInteractionTime) {

  static int lastNumber = -1;

  selectedNumber = readAnalogValue();

  if (firstReading) {
    lastNumber = selectedNumber;
    firstReading = false;
  } 

  // If potentiometer has been turned
  if (selectedNumber != lastNumber) {
      lastInteractionTime = millis();  // Reset inactivity timer
      state = ENTERING_PASSWORD;
      updateBoxLCD(lcd, state, selectedNumbers, selectedNumber);
      Serial.print("Current Selection: ");
      Serial.println(selectedNumber);
      lastNumber = selectedNumber;
  }

  if (digitalRead(buttonPin) == LOW) {
      delay(200);
      lastInteractionTime = millis();  // Reset inactivity timer

      state = ENTERING_PASSWORD;

      if (codeIndex < 4) {
          selectedNumbers[codeIndex] = selectedNumber;
          updateBoxLCD(lcd, state, selectedNumbers, selectedNumber);
          buzzer->numberSelected();
          Serial.print("Confirmed Number: ");
          Serial.println(selectedNumber);
          codeIndex++;
      }

      while (digitalRead(buttonPin) == LOW); // wait until button no longer is pressed
      delay(200);
  }


}


// -----------------------------------------------------------------------------
// updateCorrectCode()
// Downloads correct code
// -----------------------------------------------------------------------------
void updateCorrectCode() {

  lcd->clear();
  lcd->printLine("Loading...", 1);

  checkConnection();



  String codeStr = firebase->getPassword("box" + boxID);

  lcd->clear();

  // Convert String to int array
  for (int i = 0; i < codeStr.length(); i++) {
    correctCode[i] = codeStr.charAt(i) - '0';  // Convert character to integer
  }

}

// -----------------------------------------------------------------------------
// checkConnection
// Check wifi connection
// -----------------------------------------------------------------------------
void checkConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    wifiManager->connect(); 
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("WiFi lost; resetting...");
      ESP.restart();
    }
  }
}



// -----------------------------------------------------------------------------
// checkCode()
// Compares the user-entered code against the predefined 'correctCode'
// -----------------------------------------------------------------------------
void checkCode() {
    bool isCorrect = true;

    updateCorrectCode();

    // Compare selectedNumbers to correctCode
    for (int i = 0; i < 4; i++) {
        if (selectedNumbers[i] != correctCode[i]) {
            isCorrect = false;
            break;
        }
    }

    if (isCorrect) {
        // Code is correct
        state = CORRECT_PASSWORD;
        updateBoxLCD(lcd, state, selectedNumbers, selectedNumber);
        leds->update(state);
        buzzer->correctPassword();
        setLock(true);
        Serial.println("Correct Code Entered! Opening door.");

        delay(3000);

        Serial.println("Correct Code Entered! Opening door.");
        state = BOX_OPEN;
        updateBoxLCD(lcd, state, selectedNumbers, selectedNumber);
        leds->update(state);


        // Reset codeIndex/attempts for a new round
        codeIndex        = 0;
        attemptsUsed = 0;
    } else {
        // Code is wrong
        state = WRONG_PASSWORD;
        updateBoxLCD(lcd, state, selectedNumbers, selectedNumber);
        leds->update(state);
        buzzer->wrongPassword();
        Serial.println("Incorrect Code!");

        // Clear the entered numbers
        selectedNumbers[0] = -1;
        selectedNumbers[1] = -1;
        selectedNumbers[2] = -1;
        selectedNumbers[3] = -1;

        attemptsUsed++;

        // Give user a moment of RED LED
        delay(3000);

        // If max attempts reached, deny access
        if (attemptsUsed >= maxAttempts) {
            state = ACCESS_DENIED;
            leds->update(state);
            updateBoxLCD(lcd, state, selectedNumbers, selectedNumber);
            buzzer->accessDenied();
            Serial.println("Access Denied: Maximum Attempts Reached! Triggering Alarm.");
        } else {
            // Go back to entering the password
            state = ENTERING_PASSWORD;
            leds->update(state);
            updateBoxLCD(lcd, state, selectedNumbers, selectedNumber);
        }

        // Reset selection codeIndex for the next attempt
        codeIndex = 0;
    }

    // Small delay before user can start entering next input
    delay(1000);
}
