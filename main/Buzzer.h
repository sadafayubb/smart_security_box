#ifndef BUZZER_H
#define BUZZER_H

#include "Arduino.h"
#include "constants.h"

class Buzzer {

    public:

        uint8_t pin;

        Buzzer(uint8_t pin);

        void toneOn(float frequency);

        void toneOff();

        void numberSelected();

        void wrongPassword();

        void correctPassword();

        void boxClose();

        void accessDenied();

        void update(enum State state);

};

#endif