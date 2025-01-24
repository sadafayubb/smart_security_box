// @file LCD_screen.cpp

#include "LCD_screen.h"
#include "Arduino.h"


/**
 * @brief First line displayed when the box is in idle state.
 */
const char* idleLine1 = " Smart Security";

/**
 * @brief Second line displayed when the box is in idle state.
 */
const char* idleLine2 = "      Box";

/**
 * @brief First line displayed when access is denied.
 */
const char* accessDeniedLine1 = "ACCESS DENIED!";

/**
 * @brief Second line displayed when access is denied.
 */
const char* accessDeniedLine2 = "";

/**
 * @brief First line displayed when the correct password is entered.
 */
const char* correctPasswordLine1 = "ACCESS GRANTED!";

/**
 * @brief Second line displayed when the correct password is entered.
 */
const char* correctPasswordLine2 = "--> OPEN BOX <--";

/**
 * @brief First line displayed when the wrong password is entered.
 */
const char* wrongPasswordLine1 = "WRONG PASSWORD!";

/**
 * @brief Second line displayed when the wrong password is entered.
 */
const char* wrongPasswordLine2 = "";

/**
 * @brief First line displayed when the box is open.
 */
const char* boxOpenLine1 = "  BOX IS OPEN";

/**
 * @brief Second line displayed when the box is open.
 */
const char* boxOpenLine2 = "-> CLOSE DOOR <-";



/**
 * @brief Converts the code array into a string.
 *
 * @param code The integer array representing the code.
 * @return The code as a string.
 */
String _codeToString(const int code[]) {
    String result = "";  // Create an empty String object
    for (int i = 0; i < 4; i++) {
        result += (code[i] == -1) ? "_" : String(code[i]);  // Append to String
    }
    return result;
}

/**
 * @brief Updates the LCD screen based on the current state and inputs.
 *
 * @param lcd The LiquidCrystal_I2C object controlling the display.
 * @param state The current state of the system.
 * @param code The code being entered or displayed.
 * @param currentDigit The current digit being modified.
 */
void updateBoxLCD(LCDScreen* lcd, enum State state, int code[], int currentDigit) {
    switch (state) {
        case ENTERING_PASSWORD:
            lcd->clear();
            lcd->lcd.setCursor(0, 0);
            lcd->lcd.print("Password: ");
            lcd->lcd.print(_codeToString(code));
            lcd->lcd.setCursor(0, 1);
            lcd->lcd.print("Current Digit: ");
            lcd->lcd.print(currentDigit);
            break;

        case IDLE:
            lcd->printLines(idleLine1, idleLine2);
            break;

        case WRONG_PASSWORD:
            lcd->printLines(wrongPasswordLine1, wrongPasswordLine2);
            break;

        case CORRECT_PASSWORD:
            lcd->printLines(correctPasswordLine1, correctPasswordLine2);
            break;

        case ACCESS_DENIED:
            lcd->printLines(accessDeniedLine1, accessDeniedLine2);
            break;

        case BOX_OPEN:
            lcd->printLines(boxOpenLine1, boxOpenLine2);
            break;
    }
}

/**
 * @brief Constructor: Initializes the LCD screen.
 */
LCDScreen::LCDScreen() {
    this->lcd.init();
    this->lcd.backlight();
    this->lcd.clear();
}

/**
 * @brief Clears the LCD screen.
 */
void LCDScreen::clear() {
    this->lcd.clear();
}

/**
 * @brief Prints a string to a specified line on the LCD.
 *
 * @param str The string to be printed.
 * @param lineNumber The line number (1-based) where the string should be printed.
 */
void LCDScreen::printLine(String str, int lineNumber) {
    this->lcd.setCursor(0, lineNumber - 1);
    this->lcd.print(str);
}

/**
 * @brief Prints two lines to the LCD.
 *
 * @param str1 The string to be printed to line 1.
 * @param str2 The string to be printed to line 2.
 */
void LCDScreen::printLines(String str1, String str2) {
    this->clear();
    this->printLine(str1, 1);
    this->printLine(str2, 2);
}
