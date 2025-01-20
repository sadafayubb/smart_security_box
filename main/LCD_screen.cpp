#include "LCD_screen.h"
#include "Arduino.h"


String idleLine1 = " Smart Security";
String idleLine2 = "      Box";

String accessDeniedLine1 = "ACCESS DENIED!";
String accessDeniedLine2 = "";

String correctPasswordLine1 = "ACCESS GRANTED!";
String correctPasswordLine2 = "--> OPEN BOX <--";

String wrongPasswordLine1 = "WRONG PASSWORD!";
String wrongPasswordLine2 = "";

String boxOpenLine1 = "  BOX IS OPEN";
String boxOpenLine2 = "-> CLOSE DOOR <-";


String _codeToString(const int code[]) {
    String result = "";  // Create an empty String object
    for (int i = 0; i < 4; i++) {
        result += (code[i] == -1) ? "_" : String(code[i]);  // Append to String
    }
    return result;
}


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


LCDScreen::LCDScreen() {
    this->lcd.init();
    this->lcd.backlight();
    this->lcd.clear();
}

void LCDScreen::clear() {
    this->lcd.clear();
}

void LCDScreen::printLine(String str, int lineNumber) {
    this->lcd.setCursor(0, lineNumber - 1);
    this->lcd.print(str);
}

void LCDScreen::printLines(String str1, String str2) {
    this->clear();
    this->printLine(str1, 1);
    this->printLine(str2, 2);
}




