#include "potentiometerHandler.h"

PotentiometerHandler::PotentiometerHandler(uint8_t sensorPin, uint8_t buttonPin)
    : pinSensor(sensorPin), pinButton(buttonPin), digit(0), previousDigit(-1), codeIndex(0) {
    for (int i = 0; i < 4; i++) {
        code[i] = -1;
        password[i] = i + 1; // Example password: 1, 2, 3, 4
    }
}

void PotentiometerHandler::begin() {
    pinMode(pinButton, INPUT_PULLUP);
    
}

void PotentiometerHandler::handleInput() {
    int sensorValue = analogRead(pinSensor);

    int stepSize = 1023 / 10;
    int buffer = stepSize / 4;

    int calculatedDigit = 9 - (sensorValue / stepSize);
    int lowerBound = (9 - calculatedDigit) * stepSize + buffer;
    int upperBound = (9 - calculatedDigit + 1) * stepSize - buffer;

    if (sensorValue >= lowerBound && sensorValue <= upperBound) {
        digit = calculatedDigit;
    }

    if (digit != previousDigit) {
        Serial.print("Digit: ");
        Serial.println(digit);
        previousDigit = digit;
    }

    static bool pressed = false;
    if (!digitalRead(pinButton)) {
        pressed = true;
    }

    if (pressed && digitalRead(pinButton)) {
        code[codeIndex] = digit;
        codeIndex = (codeIndex + 1) % 4;

        Serial.print("Code: ");
        for (int i = 0; i < 4; i++) {
            Serial.print(code[i]);
            if (i < 3) Serial.print(", ");
        }
        Serial.println();

        if (codeIndex == 0) {
            if (checkPassword()) {
                Serial.println("Access Granted!");
            } else {
                Serial.println("Access Denied!");
            }
            resetCode();
        }
        pressed = false;
        delay(100); // Debounce
    }
}

bool PotentiometerHandler::checkPassword() {
    for (int i = 0; i < 4; i++) {
        if (code[i] != password[i]) {
            return false;
        }
    }
    return true;
}

void PotentiometerHandler::resetCode() {
    for (int i = 0; i < 4; i++) {
        code[i] = -1;
    }
    codeIndex = 0;
    Serial.println("Code reset.");
}
