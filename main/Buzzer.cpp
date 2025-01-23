/** @file Buzzer.cpp */

#include "Buzzer.h"

/**
 * @brief Constructor for the Buzzer class.
 * @param pin The GPIO pin connected to the buzzer.
 */
Buzzer::Buzzer(uint8_t pin) {
    this->pin = pin;
    pinMode(pin, OUTPUT);
}

/**
 * @brief Turns on the buzzer with a specified frequency.
 * @param frequency The frequency of the tone in Hz.
 */
void Buzzer::toneOn(float frequency) {
    tone(this->pin, frequency);
}

/**
 * @brief Turns off the buzzer.
 */
void Buzzer::toneOff() {
    noTone(this->pin);
}

/**
 * @brief Plays a short beep when a number is selected.
 */
void Buzzer::numberSelected() {
    tone(this->pin, 1500);
    delay(80);
    noTone(this->pin);
}

/**
 * @brief Plays an error sound for a wrong password.
 */
void Buzzer::wrongPassword() {
    tone(this->pin, 100);
    delay(100);
    noTone(this->pin);
    delay(100);
    tone(this->pin, 100);
    delay(200);
    noTone(this->pin);
}

/**
 * @brief Plays a melody indicating a correct password.
 */
void Buzzer::correctPassword() {
    tone(this->pin, 261);
    delay(100);
    tone(this->pin, 329);
    delay(100);
    tone(this->pin, 392);
    delay(100);
    tone(this->pin, 523);
    delay(150);
    noTone(this->pin);
}

/**
 * @brief Plays a double beep when the box is closed.
 */
void Buzzer::boxClose() {
    tone(this->pin, 1500);
    delay(100);
    noTone(this->pin);
    delay(50);
    tone(this->pin, 1500);
    delay(100);
    noTone(this->pin);
}

/**
 * @brief Plays a continuous alarm sound when access is denied.
 */
void Buzzer::accessDenied() {
    while (true) {
        tone(this->pin, 1000);
        delay(500);
        tone(this->pin, 500);
        delay(500);
    }
}
