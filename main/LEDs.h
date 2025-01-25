/** 
 * @file LEDs.h
 * @brief Header file for the LEDs class, which controls LED pins based on the system state.
 */

#ifndef LEDS_H
#define LEDS_H

#include "constants.h"

/**
 * @class LEDs
 * @brief A class to manage and update the state of LEDs based on the given state.
 */
class LEDs {

    public:

        /**
         * @brief The pin number for the green LED.
         */
        int greenPIN;

        /**
         * @brief The pin number for the yellow LED.
         */
        int yellowPIN;

        /**
         * @brief The pin number for the red LED.
         */
        int redPIN;

        /**
         * @brief Constructor for the LEDs class.
         * @param greenPIN The pin number connected to the green LED.
         * @param yellowPIN The pin number connected to the yellow LED.
         * @param redPIN The pin number connected to the red LED.
         */
        LEDs(int greenPIN, int yellowPIN, int redPIN);

        /**
         * @brief Updates the LEDs based on the provided state.
         * @param state The current state of the system, determining the LED configuration.
         */
        void update(enum State state);

};

#endif
