#include "LCD_screen.h"

LCDScreen::LCDScreen() {
    lcd.init();
    lcd.backlight();
    lcd.clear();
}

void LCDScreen::printLine(const char* str, int lineNumber) {
    lcd.clear();
    lcd.setCursor(0, lineNumber - 1);
    lcd.print(str);
}

void LCDScreen::update(enum State state) {
    switch (state) {
        case IDLE:
            printLine("IDLE", 1);
            printLine("", 2);
            break;

        case ENTERING_PASSWORD:
            lcd.setCursor(0, 0);
            lcd.print("PASSWORD: ");
            lcd.print(_codeToString(code));
            lcd.setCursor(0, 1);
            lcd.print("CURRENT DIGIT: ");
            lcd.print(currentDigit);
            break;

        case ACCESS_DENIED:
            printLine("Access Denied!", 1);
            printLine("", 2);
            break;

        case SAFE_OPEN:
            printLine("--->Box Open<---", 1);
            printLine("Please Close Box", 2);
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
