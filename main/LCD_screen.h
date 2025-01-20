/*
 * @file LCD_screen.h
 */

#ifndef LCD_SCREEN_H
#define LCD_SCREEN_H

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <string.h>
#include <stdio.h>
#include "constants.h"


/**
 * @class LCDScreen
 * @brief Handles the LCD display for the Smart Security Box.
 */
class LCDScreen {

public:

    /**
     * @brief Constructor: Initializes the LCD screen.
     */
    LCDScreen();

    /**
     * @brief Prints a string to a specified line on the LCD.
     *
     * @param str The string to be printed.
     * @param lineNumber The line number (1-based) where the string should be printed.
     */
    void printLine(const char* str, int lineNumber);

    /**
     * @brief Prints two lines to the LCD.
     *
     * @param str1 The string to be printed to line 1.
     * @param str2 The string to be printed to line 2.
     */
    void printLines(const char* str1, const char* str2);

    /**
     * @brief Updates the LCD display based on the system state.
     *
     * @param state The current system state.
     */
    void update(enum State state, int code[], int currentDigit);
    
    /**
     * @brief Clears the LCD screen
     *
     */
    void clear();

private:

    /// Main LCD object from the LiquidCrystal_I2C module
    LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);

    static const char* idleLine1;
    static const char* idleLine2;
    static const char* accessDeniedLine1;
    static const char* accessDeniedLine2;
    static const char* correctPasswordLine1;
    static const char* correctPasswordLine2;
    static const char* wrongPasswordLine1;
    static const char* wrongPasswordLine2;
    static const char* boxOpenLine1;
    static const char* boxOpenLine2;
    /**
     * @brief Converts a numeric code array into a displayable string.
     *
     * @param code[] The integer array representing the code.
     * @returns A null-terminated character array.
     */
    char* _codeToString(const int code[]);
};

#endif