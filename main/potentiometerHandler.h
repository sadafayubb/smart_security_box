#ifndef POTENTIOMETER_HANDLER_H
#define POTENTIOMETER_HANDLER_H

#include <Arduino.h>
#include "constants.h"

class PotentiometerHandler {
private:
  uint8_t pinButton;  // Pin for the button
  uint8_t pinSensor;  // Analog pin for the potentiometer

  int previousDigit;  // Previous digit value

  int password[4];    // Array holding the correct password
  int wrongAttempts;  // Tracks incorrect password attempts
  int stepSize = 1023 / 10;
  int buffer = stepSize / 4;

  void resetCode();  // Reset the code array
  int readAnalogValue();


public:
  int digit;  // Current digit value
  PotentiometerHandler(uint8_t sensorPin, uint8_t buttonPin);
  void begin();
  void handleInput(State& state, int (&code)[4], int& codeIndex);  // Pass the state by reference
  void handleButtonPress(State& state, int (&code)[4], int& codeIndex);
  void resetCode(int (&code)[4], int& codeIndex);
  void checkPassword(State& state, const int (&code)[4]);
};

#endif
