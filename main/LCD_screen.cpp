#include "LCD_screen.h"
#include "Arduino.h"

LCDScreen::LCDScreen() {

    const char* idleLine1 = "IDLE";
    const char* idleLine2 = "";
    const char* accessDeniedLine1 = "ACCESS DENIED!";
    const char* accessDeniedLine2 = "";
    const char* correctPasswordLine1 = "ACCESS GRANTED!";
    const char* correctPasswordLine2 = "--->OPEN BOX<---";
    const char* wrongPasswordLine1 = "WRONG PASSWORD!";
    const char* wrongPasswordLine2 = "";
    const char* boxOpenLine1 = "--->BOX OPEN<---";
    const char* boxOpenLine2 = "PLEASE CLOSE BOX";

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
            this->printLines(this->idleLine1, this->idleLine2);
            break;

        case WRONG_PASSWORD:
            this->printLines(this->wrongPasswordLine1, this->wrongPasswordLine2);
            break;

        case CORRECT_PASSWORD:
            this->printLines(this->correctPasswordLine1, this->correctPasswordLine2);
            break;

        case ACCESS_DENIED:
            this->printLines(this->accessDeniedLine1, this->accessDeniedLine2);
            break;

        case SAFE_OPEN:
            this->printLines(this->boxOpenLine1, this->boxOpenLine2);
            break;
    }
}

char* LCDScreen::_codeToString(const int code[]) {
    static char result[5];
    for (int i = 0; i < 4; i++) {
        result[i] = (code[i] == -1) ? "_" : ('0' + code[i]);  // 0xFE
    }
    result[4] = '\0';
    return result;
}
