/*
 * @file LCD_screen.h
 */

#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <string.h>
#include <stdio.h>
#include "cosntants.h"

/**
 * @brief Enum representing different system states.
 */

extern int stateDelay;
extern int code[4];  ///< Global array storing the entered code.
extern int currentDigit;  ///< Stores the currently selected digit.

extern char idleLine1[];
extern char idleLine2[];
extern char accessDeniedLine1[];
extern char accessDeniedLine2[];
extern char accessGrantedLine1[];
extern char accessGrantedLine2[];
extern char boxOpenLine1[];
extern char boxOpenLine2[];

/**
 * @brief Converts a numeric code array into a displayable string.
 *
 * @param code[] The integer array representing the code.
 * @returns A null-terminated character array.
 */
char* codeToString(const int code[]);

/**
 * @brief Prints a string to a specified line on the LCD.
 *
 * @param lcd An initialized LiquidCrystal_I2C object.
 * @param str The string to be printed.
 * @param lineNumber The line number (1-based) where the string should be printed.
 */
void LCDPrintLine(LiquidCrystal_I2C lcd, const char* str, int lineNumber);

/**
 * @brief Prints two lines on the LCD screen.
 *
 * @tparam T1 Type of first line input.
 * @tparam T2 Type of second line input.
 * @param lcd An initialized LiquidCrystal_I2C object.
 * @param line1 The first line to print.
 * @param line2 The second line to print.
 */
template <typename T1, typename T2>
void LCDPrintLines(LiquidCrystal_I2C lcd, T1 line1, T2 line2) {
    lcd.setCursor(0, 0);
    lcd.print(line1);
    lcd.setCursor(0, 1);
    lcd.print(line2);
}

/**
 * @brief Initializes and returns a LiquidCrystal_I2C object.
 *
 * @returns An initialized LCD object.
 */
LiquidCrystal_I2C LCDInitialize();

/**
 * @brief Updates the LCD display based on the system state.
 *
 * @param lcd An initialized LiquidCrystal_I2C object.
 * @param state The current system state.
 */
void LCDUpdate(LiquidCrystal_I2C lcd, enum State state);

#endif
