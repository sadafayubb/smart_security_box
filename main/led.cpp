//
#include "constants.h"


void setupLED() {
}
void lightCheck::update(enum State state) {

  switch (state) {
            case IDLE:
                digitalWrite(10, HIGH); // Yellow LED on
                digitalWrite(9, LOW);   // Green LED off
                digitalWrite(11, LOW);  // Red LED off
                break;
            
            case ENTERING_PASSWORD:
                 digitalWrite(10, HIGH); // Yellow LED on
                digitalWrite(9, LOW);   // Green LED off
                digitalWrite(11, LOW);  // Red LED off
                break;
            
            case WRONG_PASSWORD:
            // If attempt is incorrect once
                digitalWrite(10, LOW);       // turn off yellow
                digitalWrite(11, HIGH);      // red blinks
                delay(1000);
                digitalWrite(11, LOW);
                digitalWrite(10, HIGH);  // turn yellow back on
            
            case ACCESS_DENIED:
                digitalWrite(10, LOW);   // turn off yellow
                digitalWrite(11, HIGH);  // red blinks
                delay(500);
                digitalWrite(11, LOW);
                digitalWrite(11, HIGH);  // red blinks
                delay(500);
                digitalWrite(11, LOW);
                digitalWrite(11, HIGH);  // red blinks
                delay(500);
                digitalWrite(11, LOW);

                digitalWrite(10, HIGH);  // turn yellow back on
                break;
            
            case SAFE_OPEN:
                digitalWrite(10, LOW);  // turn off yellow
                digitalWrite(9, HIGH);  // turn on green
                digitalWrite(11,LOW); // turn off red
                break;
        }


 /* pseudocode for understanding
  void ledController() {
    while (idle == true) {  // Keep yellow on when idle
      Yellow LED on

      if (incorrect once == true) {  // If attempt is incorrect once
        Yellow LED off  
        Red LED blinks once
        Yellow LED back on
      } else if
         (incorrect thrice == true) {
            Yellow LED off  
            Red LED blinks three times
            Yellow LED back on
        }
      else if (correct attempt == true) {
          Yellow LED off
          Keep green on until Idle state
        }
    }
  */
  }

