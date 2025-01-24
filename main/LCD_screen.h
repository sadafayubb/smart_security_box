/** @file LCD_screen.h */


#ifndef LCD_SCREEN_H
#define LCD_SCREEN_H

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <string.h>
#include <stdio.h>
#include "constants.h"


/**
 * @brief Converts the code array into a string.
 *
 * @param code The integer array representing the code.
 * @return The code as a string.
 */
String _codeToString(const int code[]);

/**
 * @class LCDScreen
 * @brief Handles the LCD display for the Smart Security Box.
 */
class LCDScreen
{
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
    void printLine(String str, int lineNumber);

    /**
    * @brief Prints two lines to the LCD.
    *
    * @param str1 The string to be printed to line 1.
    * @param str2 The string to be printed to line 2.
    */
    void printLines(String str1, String str2);

    /**
     * @brief Clears the LCD screen.
     */
    void clear();

    /**
     * @brief Main LCD object from the LiquidCrystal_I2C module.
     */
    LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);
};

/**
 * @brief Updates the LCD screen based on the current state and inputs.
 *
 * @param lcd The LiquidCrystal_I2C object controlling the display.
 * @param state The current state of the system.
 * @param code The code being entered or displayed.
 * @param currentDigit The current digit being modified.
 */
void updateBoxLCD(LCDScreen* lcd, enum State state, int code[], int currentDigit);



#endif // LCD_SCREEN_H
