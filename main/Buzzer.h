/** @file Buzzer.h */

#ifndef BUZZER_H
#define BUZZER_H

#include "Arduino.h"
#include "constants.h"

class Buzzer {

    public:

        uint8_t pin;

        /**
        * @brief Constructor for the Buzzer class.
        * @param pin The GPIO pin connected to the buzzer.
        */
        Buzzer(uint8_t pin);

        /**
        * @brief Turns on the buzzer with a specified frequency.
        * @param frequency The frequency of the tone in Hz.
        */
        void toneOn(float frequency);

        /**
        * @brief Turns off the buzzer.
        */
        void toneOff();

        /**
        * @brief Plays a short beep when a number is selected.
        */
        void numberSelected();

        /**
        * @brief Plays an error sound for a wrong password.
        */
        void wrongPassword();

        /**
        * @brief Plays a melody indicating a correct password.
        */
        void correctPassword();

        /**
        * @brief Plays a double beep when the box is closed.
        */
        void boxClose();

        /**
        * @brief Plays a continuous alarm sound when access is denied.
        */
        void accessDenied();

};

#endif