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

const char* idleLine1 = " Smart Security";
const char* idleLine2 = "      Box";

const char* accessDeniedLine1 = "ACCESS DENIED!";
const char* accessDeniedLine2 = "";

const char* correctPasswordLine1 = "ACCESS GRANTED!";
const char* correctPasswordLine2 = "--> OPEN BOX <--";

const char* wrongPasswordLine1 = "WRONG PASSWORD!";
const char* wrongPasswordLine2 = "";

const char* boxOpenLine1 = "  BOX IS OPEN";
const char* boxOpenLine2 = "-> CLOSE DOOR <-";

char* _codeToString(const int code[])
void updateBoxLCD(LiquidCrystal_I2C lcd, enum State state, int code[], int currentDigit);
void updatePasswordManagerLCD(LiquidCrystal_I2C lcd, ...);

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

    /**
     * @brief Converts a numeric code array into a displayable string.
     *
     * @param code[] The integer array representing the code.
     * @returns A null-terminated character array.
     */
    char* _codeToString(const int code[]);
};

#endif

