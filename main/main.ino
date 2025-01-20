#include "constants.h"
#include "potentiometerHandler.h"
// Prerequesite: Install LiquidCrystal I2C@1.1.2
// Create a PotentiometerHandler object
State currentState = IDLE;
PotentiometerHandler potentiometerHandler(A0, 3);

int code[4] = {-1, -1, -1, -1};  // Global code array
int codeIndex = 0;      // Global code index

void setup() {
  Serial.begin(9600);
  potentiometerHandler.begin();
}

void loop() {
  potentiometerHandler.handleInput(currentState, code, codeIndex);
/*
  // Print the current state for debugging
  switch (currentState) {
    case IDLE:
      Serial.println("State: IDLE");
      break;
    case ENTERING_PASSWORD:
      Serial.println("State: ENTERING_PASSWORD");
      break;
    case WRONG_PASSWORD:
      Serial.println("State: WRONG_PASSWORD");
      break;
    case ACCESS_DENIED:
      Serial.println("State: ACCESS_DENIED");
      break;
    case SAFE_OPEN:
      Serial.println("State: SAFE_OPEN");
      break;
  }
  if (currentState == WRONG_PASSWORD) {
    currentState = ENTERING_PASSWORD;
  }
  delay(300);
  */
}
