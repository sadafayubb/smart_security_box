#include "potentiometerHandler.h"

// Create a PotentiometerHandler object
PotentiometerHandler potentiometerHandler(A0, 3);

void setup() {
  Serial.begin(9600);
  potentiometerHandler.begin();
}

void loop() {
  potentiometerHandler.handleInput();
}
