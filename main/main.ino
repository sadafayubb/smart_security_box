/** @file main.ino */

// -----------------------------------------------------------------------------
// Module imports
// -----------------------------------------------------------------------------
#include <Arduino.h>
#include "buzzer.h"
#include "LCD_screen.h"
#include "constants.h"
#include "motor_module.h"
#include "LEDs.h"
#include <Firebase.h>
#include "FirebaseHandler.h"
#include "WiFiManager.h"
#include "potentiometer.h"

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

// -----------------------------------------------------------------------------
// Objects for LCD Screen, LEDs, and Buzzer
// -----------------------------------------------------------------------------
/** @brief LCD screen object. */
LCDScreen* lcd;

/** @brief Buzzer object for sound notifications. */
Buzzer* buzzer;

/** @brief LEDs object for controlling the LEDs. */
LEDs* leds;

// -----------------------------------------------------------------------------
// Global State
// -----------------------------------------------------------------------------
/** @brief System state enum. */
enum State state = IDLE;

// -----------------------------------------------------------------------------
// Pin Definitions
// -----------------------------------------------------------------------------
/** @brief Potentiometer input pin. */
const int potPin = A0;

/** @brief Button input pin. */
const int buttonPin = 16;

/** @brief Buzzer output pin. */
const int buzzerPin = 15;

/** @brief Green LED output pin. */
const int greenLEDPin = 14;

/** @brief Yellow LED output pin. */
const int yellowLEDPin = 12;

/** @brief Red LED output pin. */
const int redLEDPin = 13;

/** @brief Servo motor control pin. */
const int servoMotorPin = 2;

// -----------------------------------------------------------------------------
// Global Variables
// -----------------------------------------------------------------------------

/** @brief BOX ID for Firebase. */
String boxID = "004";

/** @brief Array storing user input numbers. */
int selectedNumbers[4] = { -1, -1, -1, -1 };

/** @brief Tracks which digit the user is selecting. */
int codeIndex = 0;

/** @brief Tracks the number of failed attempts. */
int attemptsUsed = 0;

/** @brief Current selected number from the potentiometer. */
int selectedNumber = 0;

/** @brief Maximum allowed password attempts before access is denied. */
const int maxAttempts = 3;

/** @brief Flag to indicate if it is the first user input reading. Is used to ensure IDLE mode upon program start */
bool firstReading = true;

/** @brief Timestamp of the last interaction. */
unsigned long lastInteractionTime = 0;

/** @brief Timeout duration for inactivity. */
const unsigned long timeoutDuration = 60000;

/** @brief Duration after which the system resets due to inactivity (and resets attempt count). */
const unsigned long resetDuration = 1800000;

/** @brief Is there to contain the correct password. */
int correctCode[4];


// -----------------------------------------------------------------------------
// Function Prototypes
// -----------------------------------------------------------------------------
void resetSystem();
void checkCode();
void updateCorrectCode();
void checkConnection();

// -----------------------------------------------------------------------------
// setup()
// -----------------------------------------------------------------------------
/**
 * @brief Main arduino setup function responsible for initializing objects, pins, and hardware components.
 */
void setup() {
    Serial.begin(115200);
    Serial.println("I AM ALIVE!");

    // Instantiate objects
    lcd    = new LCDScreen();
    buzzer = new Buzzer(buzzerPin);
    leds = new LEDs(greenLEDPin, yellowLEDPin, redLEDPin);
    wifiManager = new WiFiManager(ssid, pass);
    firebase = new FirebaseHandler(FIREBASE_HOST, FIREBASE_AUTH);

    // Connect to WiFi
    wifiManager->connect();  

    // Lock door
    setupMotor(servoMotorPin);
    setLock(false);

    // Setup pins
    pinMode(buttonPin, INPUT_PULLUP);
    pinMode(buzzerPin, OUTPUT);
    pinMode(greenLEDPin, OUTPUT);
    pinMode(yellowLEDPin, OUTPUT);
    pinMode(redLEDPin, OUTPUT);

    delay(3000);

    // Update LCD and LEDs to show IDLE state
    updateBoxLCD(lcd, state, selectedNumbers, selectedNumber);
    leds->update(state);
}

// -----------------------------------------------------------------------------
// loop()
// -----------------------------------------------------------------------------
/**
 * @brief Main arduino loop function. This runs constantly and allows for user interaction.
 */
void loop() {

    // Reset system if inactive for 1 minute
    if (state != ACCESS_DENIED && state != IDLE && state != BOX_OPEN && millis() - lastInteractionTime > timeoutDuration) {
        Serial.println("Inactivity timeout: Resetting system...");
        resetSystem();
    } else if (state == IDLE && attemptsUsed > 0 && millis() - lastInteractionTime > resetDuration) {  // if 30 minutes inactive reset attemps also
        Serial.println("Resetting attempt count...");
        attemptsUsed = 0;
    }

    // If ready for the user to make input
    if (state == IDLE || state == ENTERING_PASSWORD) {
        chooseNumber(lastInteractionTime, 
                     lcd, 
                     state, 
                     selectedNumbers, 
                     selectedNumber, 
                     codeIndex, 
                     buzzer, 
                     buttonPin, 
                     potPin, 
                     firstReading);  // Allow input
        if (codeIndex == 4) {
            checkCode();  // If four digits have been entered check the code
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

}


// -----------------------------------------------------------------------------
// Functions essential to the main functionalities
// -----------------------------------------------------------------------------

/**
 * @brief Resets the system state and clears selected numbers.
 */
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

/**
 * @brief Checks the entered code against the correct code.
 */
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
        checkConnection();
        firebase->logEntry("box" + boxID, "Access granted!");

        state = CORRECT_PASSWORD;
        updateBoxLCD(lcd, state, selectedNumbers, selectedNumber);
        leds->update(state);
        buzzer->correctPassword();
        setLock(true);
        Serial.println("Correct Code Entered! Opening door.");

        delay(3000);

        state = BOX_OPEN;
        updateBoxLCD(lcd, state, selectedNumbers, selectedNumber);
        leds->update(state);

        // Reset codeIndex/attempts for a new round
        codeIndex = 0;
        attemptsUsed = 0;
    } else {
        // Code is incorrect
        checkConnection();
        firebase->logEntry("box" + boxID, "Wrong password: " + (String)selectedNumbers[0] + (String)selectedNumbers[1] + (String)selectedNumbers[2] + (String)selectedNumbers[3]);

        state = WRONG_PASSWORD;
        updateBoxLCD(lcd, state, selectedNumbers, selectedNumber);
        leds->update(state);
        buzzer->wrongPassword();
        Serial.println("Incorrect Code!");

        // Clear the entered numbers
        for (int i = 0; i < 4; i++) {
            selectedNumbers[i] = -1;
        }

        attemptsUsed++;

        // Delay before retry
        delay(3000);

        // If max attempts reached, deny access
        if (attemptsUsed >= maxAttempts) {
            checkConnection();
            firebase->logEntry("box" + boxID, "Access denied!");

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

/**
 * @brief Updates the correct code by retrieving it from Firebase.
 */
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

