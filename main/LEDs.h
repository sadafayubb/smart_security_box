/** @file LEDs.h */

#ifndef LEDS_H
#define LEDS_H

#include "constants.h"


class LEDs {

    public:

        int greenPIN, yellowPIN, redPIN;

        LEDs(int greenPIN, int yellowPIN, int redPIN);

        void update(enum State state);

};



#endif

