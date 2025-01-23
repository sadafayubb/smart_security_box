/** @file potentiometer.h */

#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

#include "Arduino.h"
#include "LCD_screen.h"
#include "constants.h"
#include "Buzzer.h"

/**
 * @brief Reads the analog value from the potentiometer pin (A0) and maps it to a digit.
 * @param potPin The analog pin connected to the potentiometer (A0).
 * @return The mapped digit (0-9) based on the potentiometer reading.
 */
int readAnalogValue(int potPin);

/**
 * @brief Handles number selection using the potentiometer and the button.
 * @param lastInteractionTime Reference to the last interaction time for inactivity tracking.
 * @param lcd The LCD screen object for display updates.
 * @param state The current state of the Box system.
 * @param selectedNumbers The selected numbers (so far).
 * @param selectedNumber The number currently selected with the potentiometer.
 * @param codeIndex The current index in the selected numbers array.
 * @param buzzer The buzzer object for making sound.
 * @param buttonPin The pin connected to the button.
 * @param potPin The analog pin connected to the potentiometer.
 * @param firstReading Boolean flag to handle initial reading correction.
 */
void chooseNumber(unsigned long &lastInteractionTime, LCDScreen lcd, enum State state, int selectedNumbers[], int selectedNumber, int codeIndex, Buzzer buzzer, int buttonPin, int potPin, bool firstReading);

#endif // POTENTIOMETER_H
