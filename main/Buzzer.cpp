#include "Buzzer.h"

Buzzer::Buzzer(uint8_t pin) {
    this->pin = pin;
    pinMode(pin, OUTPUT);
}

void Buzzer::toneOn(float frequency) {
    tone(this->pin, frequency);
}

void Buzzer::toneOff() {
    noTone(this->pin);
}

void Buzzer::numberSelected() {
    tone(this->pin, 1500);
    delay(80);
    noTone(this->pin);

}

void wrongPassword() {
  tone(this->pin, 100);
  delay(100);
  noTone(this->pin);
  delay(100);
  tone(this->pin, 100);
  delay(200);
  noTone(this->pin);
}


void Buzzer::correctPassword() {
  tone(this->pin, 261);
  delay(100);
  tone(this->pin, 329);
  delay(100);
  tone(this->pin, 392);
  delay(100);
  tone(this->pin, 523);
  delay(150);
  noTone(this->pin);

}

void Buzzer::boxClose() {
  tone(this->pin, 1500);
  delay(100);
  noTone(this->pin);
  delay(50);
  tone(this->pin, 1500);
  delay(100);
  noTone(this->pin);
}

void Buzzer::accessDenied() {
  while (true) {
    tone(this->pin, 1000);
    delay(500);
    tone(this->pin, 500);
    delay(500);
  }


}
