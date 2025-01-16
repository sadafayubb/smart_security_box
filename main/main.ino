#include "LCD_screen.h"


LCDScreen* lcd;

int code[4] = {1,2, 3, 4};
enum State state = IDLE;

void setup() {

  lcd = new LCDScreen(); // Dynamically allocate LCDScreen object
  lcd->printLines("Hello", "Idiot<3");
  lcd->update(state);

  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
