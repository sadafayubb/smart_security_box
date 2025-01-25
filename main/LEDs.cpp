/** 
 * @file LEDs.cpp
 * @brief Implementation file for the LEDs class, which controls LED behavior based on system state.
 */

#include "Arduino.h"
#include "constants.h"
#include "LEDs.h"

/**
 * @brief Constructor for the LEDs class.
 * 
 * Initializes the LED pins and sets them as outputs.
 * 
 * @param greenPIN The pin number connected to the green LED.
 * @param yellowPIN The pin number connected to the yellow LED.
 * @param redPIN The pin number connected to the red LED.
 */
LEDs::LEDs(int greenPIN, int yellowPIN, int redPIN) {

    this->greenPIN = greenPIN;
    this->yellowPIN = yellowPIN;
    this->redPIN = redPIN;

    pinMode(this->greenPIN, OUTPUT);
    pinMode(this->yellowPIN, OUTPUT);
    pinMode(this->redPIN, OUTPUT);
}

/**
 * @brief Updates the LEDs based on the provided system state.
 * 
 * Sets the state of the LEDs to indicate the current system state.
 * 
 * @param state The current state of the system, which determines the LED behavior.
 */
void LEDs::update(enum State state) {

    switch (state) {
        case IDLE:
        case ENTERING_PASSWORD:
            digitalWrite(this->yellowPIN, HIGH); // Yellow LED on
            digitalWrite(this->greenPIN, LOW);   // Green LED off
            digitalWrite(this->redPIN, LOW);    // Red LED off
            break;

        case WRONG_PASSWORD:
            digitalWrite(this->yellowPIN, LOW); // Yellow LED off
            digitalWrite(this->greenPIN, LOW);   // Green LED off
            digitalWrite(this->redPIN, HIGH);  // Red LED on
            break;

        case ACCESS_DENIED:
            digitalWrite(this->yellowPIN, LOW); // Yellow LED off
            digitalWrite(this->greenPIN, LOW);   // Green LED off
            digitalWrite(this->redPIN, HIGH);  // Red LED on
            break;

        case CORRECT_PASSWORD:
            digitalWrite(this->yellowPIN, LOW);  // Yellow LED off
            digitalWrite(this->greenPIN, HIGH);  // Green LED on
            digitalWrite(this->redPIN, LOW);    // Red LED off
            break;

        case BOX_OPEN:
            digitalWrite(this->yellowPIN, LOW);  // Yellow LED off
            digitalWrite(this->greenPIN, HIGH);  // Green LED on
            digitalWrite(this->redPIN, LOW);    // Red LED off
            break;
    }
}
