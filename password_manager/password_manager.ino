#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

// ----- LCD and Keypad Setup -----
LiquidCrystal_I2C lcd(0x27, 16, 2);

char keys[4][4] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[4] = {9,8,7,6}, colPins[4] = {5,4,3,2};
Keypad keypad(makeKeymap(keys), rowPins, colPins, 4, 4);

// ----- States -----
enum { S_ID, S_PASS } state = S_ID;

// ----- Variables -----
String boxID, password;
String validIDs[] = {"123", "4567", "9999"};  // Example valid IDs (This is just for now until its connected with Firesbase)
const int PASS_LEN = 4;

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("Enter Box ID:");
}

void loop() {
  char k = keypad.getKey();
  if (!k) return;  // Do nothing if no key is pressed

  switch (state) {

    // ---------- ENTER BOX ID ----------
    case S_ID:
      if (k == '*') {
        // Clear Box ID
        boxID = "";
        lcd.clear();
        lcd.print("Enter Box ID:");
      } 
      else if (k == '#') {
        // Validate ID
        if (isValidID(boxID)) {
          password = "";
          state = S_PASS;
          lcd.clear();
          lcd.print("Enter 4-digit PW:");
        } else {
          // Show error briefly, then reset
          lcd.clear();
          lcd.print("Invalid Box ID!");
          delay(2000);
          boxID = "";
          lcd.clear();
          lcd.print("Enter Box ID:");
        }
      } 
      else {
        // Append character to Box ID
        boxID += k;
        lcd.setCursor(0, 1);
        lcd.print(boxID);
      }
      break;

    // ---------- ENTER 4-DIGIT PASSWORD ----------
    case S_PASS:
      if (k == '*') {
        // Clear password
        password = "";
        lcd.clear();
        lcd.print("Enter 4-digit PW:");
      } 
      else if (k == '#') {
        // Submit password only if it's exactly 4 digits
        if (password.length() == PASS_LEN) {
          lcd.clear();
          lcd.print("PW Saved:");
          lcd.setCursor(0, 1);
          lcd.print(password);
          delay(2000);

          // Reset to Box ID state
          boxID = "";
          password = "";
          state = S_ID;
          lcd.clear();
          lcd.print("Enter Box ID:");
        } else {
          // Not enough (or too many) digits
          lcd.clear();
          lcd.print("Need 4 digits!");
          delay(2000);
          lcd.clear();
          lcd.print("Enter 4-digit PW:");
        }
      }
      else if (isDigit(k) && password.length() < PASS_LEN) {
        // Append digit if under 4
        password += k;
        lcd.setCursor(0, 1);
        lcd.print(password);
      }
      // If the  password is already 4 digits long and the user enters more we just ignor it
      break;
  }
}

// ----- Check if Box ID is Valid -----
bool isValidID(const String &id) {
  for (int i = 0; i < (int)(sizeof(validIDs) / sizeof(validIDs[0])); i++) {
    if (validIDs[i] == id) return true;
  }
  return false;
}
