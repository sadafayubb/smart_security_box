#ifndef POTENTIOMETER_HANDLER_H
#define POTENTIOMETER_HANDLER_H

#include <Arduino.h>

class PotentiometerHandler {
private:
    uint8_t pinButton;       // Pin for the button
    uint8_t pinSensor;       // Analog pin for the potentiometer
    int digit;               // Current digit value
    int previousDigit;       // Previous digit value
    int code[4];             // Array to store entered code
    int codeIndex;           // Current index in the code array
    int password[4];         // Array holding the correct password

    void resetCode();        // Reset the code array

public:
    PotentiometerHandler(uint8_t sensorPin, uint8_t buttonPin);
    void begin();
    void handleInput();
    bool checkPassword();
};

#endif
