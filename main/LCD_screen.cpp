#include "LCD_screen.h"
#include "Arduino.h"

LCDScreen::LCDScreen() {
    this->lcd.init();
    this->lcd.backlight();
    this->lcd.clear();
}

void LCDScreen::clear() {
    this->lcd.clear();
}

void LCDScreen::printLine(const char* str, int lineNumber) {
    this->lcd.setCursor(0, lineNumber - 1);
    this->lcd.print(str);
}

void LCDScreen::printLines(const char* str1, const char* str2) {
    this->clear();
    this->printLine(str1, 1);
    this->printLine(str2, 2);
}

void LCDScreen::update(enum State state, int code[], int currentDigit) {
    switch (state) {
        case ENTERING_PASSWORD:
            this->lcd.setCursor(0, 0);
            this->lcd.print("PASSWORD: ");
            this->lcd.print(_codeToString(code));
            this->lcd.setCursor(0, 1);
            this->lcd.print("CURRENT DIGIT: ");
            this->lcd.print(currentDigit);
            break;

        case IDLE:
            this->printLines(idleLine1, idleLine2)
            break;

        case WRONG_PASSWROD:
            this->printLines(wrongPasswordLine1, wrongPasswordLine2);
            break;

        case CORRECT_PASSWORD:
            this->printLines(correctPasswordLine1, correctPasswordLine2);
            break;

        case ACCESS_DENIED:
            this->printLines(accessDeniedLine1, accessDeniedLine2);
            break;

        case SAFE_OPEN:
            this->printLines(boxOpenLine1, boxOpenLine2);
            break;
    }
}

char* LCDScreen::_codeToString(const int code[]) {
    static char result[5];
    for (int i = 0; i < 4; i++) {
        result[i] = (code[i] == -1) ? 0xFE : ('0' + code[i]);
    }
    result[4] = '\0';
    return result;
}
