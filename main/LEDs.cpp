/** @file LEDs.cpp */

#include "Arduino.h"
#include "constants.h"
#include "LEDs.h"

LEDs::LEDs(int greenPIN, int yellowPIN, int redPIN) {

    this->greenPIN = greenPIN;
    this->yellowPIN = yellowPIN;
    this->redPIN = redPIN;

    pinMode(this->greenPIN, OUTPUT);
    pinMode(this->yellowPIN, OUTPUT);
    pinMode(this->redPIN, OUTPUT);

}

void LEDs::update(enum State state) {

  switch (state) {
            case IDLE:
            case ENTERING_PASSWORD:
                digitalWrite(this->yellowPIN, HIGH); // Yellow LED on
                digitalWrite(this->greenPIN, LOW);   // Green LED off
                digitalWrite(this->redPIN, LOW);  // Red LED off
                break;
            
            case WRONG_PASSWORD:
                digitalWrite(this->yellowPIN, LOW); // Yellow LED off
                digitalWrite(this->greenPIN, LOW);   // Green LED off
                digitalWrite(this->redPIN, HIGH);  // Red LED on
            
            case ACCESS_DENIED:
                digitalWrite(this->yellowPIN, LOW); // Yellow LED off
                digitalWrite(this->greenPIN, LOW);   // Green LED off
                digitalWrite(this->redPIN, HIGH);  // Red LED on
                break;
            
            case CORRECT_PASSWORD:
                digitalWrite(this->yellowPIN, LOW);  // turn off yellow
                digitalWrite(this->greenPIN, HIGH);  // turn on green
                digitalWrite(this->redPIN,LOW); // turn off red
                break;

            case BOX_OPEN:
                digitalWrite(this->yellowPIN, LOW);  // turn off yellow
                digitalWrite(this->greenPIN, HIGH);  // turn on green
                digitalWrite(this->redPIN,LOW); // turn off red
                break;
        }
}