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


char line


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

    char idleLine1[] = "IDLE";
    char idleLine2[] = "";
    char accessDeniedLine1[] = "ACCESS DENIED!";
    char accessDeniedLine2[] = "";
    char correctPasswordLine1[] = "ACCESS GRANTED!";
    char correctPasswordLine2[] = "--->OPEN BOX<---";
    char wrongPasswordLine1[] = "WRONG PASSWORD!";
    char wrongPasswordLine2[] = "";
    char boxOpenLine1[] = "--->BOX OPEN<---";
    char boxOpenLine2[] = "PLEASE CLOSE BOX";

    /// Main LCD object from the LiquidCrystal_I2C module
    LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);

    /**
     * @brief Converts a numeric code array into a displayable string.
     *
     * @param code[] The integer array representing the code.
     * @returns A null-terminated character array.
     */
    char* _codeToString(const int code[]);
};

#endif

