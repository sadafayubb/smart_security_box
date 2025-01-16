#include "LCD_screen.h"

int stateDelay = 4000;
int code[4] = {1, 2, 3, 4}; ///< Example default code.
int currentDigit = 0; ///< Current digit selection.

char idleLine1[] = "IDLE";
char idleLine2[] = "";
char accessDeniedLine1[] = "Access Denied!";
char accessDeniedLine2[] = "";
char accessGrantedLine1[] = "Access Granted!";
char accessGrantedLine2[] = "--->Open Box<---";
char boxOpenLine1[] = "--->Box Open<---";
char boxOpenLine2[] = "Please Close Box";


class LCDScreen {

public:

    /**
     * @brief Prints a string to a specified line on the LCD.
     *
     * @param lcd An initialized LiquidCrystal_I2C object.
     * @param str The string to be printed.
     * @param lineNumber The line number (1-based) where the string should be printed.
     */
    void printLine(const char* str, int lineNumber) {
        lcd.clear();
        lcd.setCursor(0, lineNumber - 1);
        lcd.print(str);
    }

    /**
     * @brief Updates the LCD display based on the system state.
     *
     * @param lcd An initialized LiquidCrystal_I2C object.
     * @param state The current system state.
     */
    void update(enum State state) {
        switch (state) {
            case IDLE:
                LCDPrintLines(lcd, idleLine1, idleLine2);
                break;
            
            case ENTERING_PASSWORD:
                lcd.setCursor(0, 0);
                lcd.print("PASSWORD: ");
                lcd.print(_codeToString(code));
                lcd.setCursor(0, 1);
                lcd.print("CURRENT DIGIT: ");
                lcd.print(currentDigit); // Fixed missing semicolon
                break;
            
            case ACCESS_DENIED:
                LCDPrintLines(lcd, accessDeniedLine1, accessDeniedLine2);
                break;
            
            case SAFE_OPEN:
                LCDPrintLines(lcd, boxOpenLine1, boxOpenLine2);
                break;
        }
    }

private:

    /**
     * @brief Initializes and returns a LiquidCrystal_I2C object.
     *
     * @returns An initialized LCD object.
     */

    LiquidCrystal_I2C lcd(0x27, 16, 2);

    
    LCDScreen() {
        this->lcd.init();
        this->lcd.backlight();
        this->lcd.clear();
    }

    /**
     * @brief Converts a numeric code array into a displayable string.
     *
     * @param code[] The integer array representing the code.
     * @returns A null-terminated character array.
     */
    char* _codeToString(const int code[]) {
        static char result[5]; // Fixed length 5 (4 digits + '\0')
        for (int i = 0; i < 4; i++) {
            result[i] = (code[i] == -1) ? 0xFE : ('0' + code[i]);
        }
        result[4] = '\0';
        return result;
    }


}:



