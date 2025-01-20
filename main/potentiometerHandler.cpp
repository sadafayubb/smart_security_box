#include "PotentiometerHandler.h"

const int NUM_DIGITS = 10;  // Number of digits (0–9)
int lowerBounds[NUM_DIGITS];
int upperBounds[NUM_DIGITS];

PotentiometerHandler::PotentiometerHandler(uint8_t sensorPin, uint8_t buttonPin)
  : pinSensor(sensorPin), pinButton(buttonPin), digit(-1), previousDigit(-1), wrongAttempts(0) {
  for (int i = 0; i < 4; i++) {
    password[i] = i + 1;  // Example password: 1, 2, 3, 4
  }
}

void PotentiometerHandler::begin() {
  pinMode(pinButton, INPUT_PULLUP);

  // Precompute lower and upper bounds for each digit
  int stepSize = 1023 / NUM_DIGITS;
  int buffer = stepSize / 2;
  Serial.print("buffer: ");
  Serial.print(buffer);

  for (int i = 0; i < NUM_DIGITS; i++) {
    lowerBounds[i] = (NUM_DIGITS - i - 1) * stepSize + buffer;
    upperBounds[i] = (NUM_DIGITS - i) * stepSize - buffer;
    previousDigit = readAnalogValue();
  }
  Serial.println("Bounds initialized with larger buffer:");
  for (int i = 0; i < NUM_DIGITS; i++) {
    Serial.print("Digit ");
    Serial.print(i);
    Serial.print(": Lower = ");
    Serial.print(lowerBounds[i]);
    Serial.print(", Upper = ");
    Serial.println(upperBounds[i]);
  }
}

void PotentiometerHandler::handleInput(State& state, int (&code)[4], int& codeIndex) {
  digit = readAnalogValue();

  // Change state to ENTERING_PASSWORD if a digit change occurs and the current state is IDLE
  if (state == IDLE && digit != previousDigit) {
    state = ENTERING_PASSWORD;

    Serial.println("State changed to ENTERING_PASSWORD.");
  }

  // Check if the digit has changed
  if (digit != previousDigit) {
    Serial.print("Digit: ");
    Serial.println(digit);
    previousDigit = digit;
  }

  handleButtonPress(state, code, codeIndex);
}

int PotentiometerHandler::readAnalogValue() {
  int sensorValue = analogRead(pinSensor);

  // Determine the current digit
  int calculatedDigit = 9 - (sensorValue / (1023 / 9));

  // Fetch precomputed bounds
  int lowerBound = lowerBounds[calculatedDigit];
  int upperBound = upperBounds[calculatedDigit];

  // Verify if the sensor value is in the middle range for stability
  if (sensorValue >= lowerBound && sensorValue <= upperBound) {
    return calculatedDigit;  // Return valid digit
  }
}

void PotentiometerHandler::handleButtonPress(State& state, int (&code)[4], int& codeIndex) {
  static bool pressed = false;

  // Detect button press
  if (!digitalRead(pinButton)) {
    pressed = true;
  }

  // Handle button release
  if (pressed && digitalRead(pinButton)) {
    if (state == ENTERING_PASSWORD) {
      // Store the current digit in the code array
      code[codeIndex] = digit;
      codeIndex = (codeIndex + 1) % 4;

      // Print the entered code for debugging
      Serial.print("Code: ");
      for (int i = 0; i < 4; i++) {
        Serial.print(code[i]);
        if (i < 3) Serial.print(", ");
      }
      Serial.println();

      // Check the code if 4 digits have been entered
      if (codeIndex == 0) {
        checkPassword(state, code);
        resetCode(code, codeIndex);
      }
    }
    if (state == SAFE_OPEN || state == WRONG_PASSWORD) {
      // Change state to IDLE when the button is pressed
      state = IDLE;
      Serial.println("State changed to IDLE.");
    }

    // Reset pressed flag and add debounce delay
    pressed = false;
    delay(50);
  }
}

void PotentiometerHandler::checkPassword(State& state, const int (&code)[4]) {
  bool isCorrect = true;

  // Compare entered code with the password
  for (int i = 0; i < 4; i++) {
    if (code[i] != password[i]) {
      isCorrect = false;
      break;
    }
  }

  if (isCorrect) {
    state = CORRECT_PASSWORD;
    Serial.println("Access Granted!");
    delay(3000);
    state = SAFE_OPEN;

  } else {
    wrongAttempts++;
    if (wrongAttempts >= 3) {
      state = ACCESS_DENIED;
      Serial.println("Access Denied: Too many wrong attempts!");
    } else {
      state = WRONG_PASSWORD;
      Serial.println("Access Denied: Incorrect password!");
    }
  }
}

void PotentiometerHandler::resetCode(int (&code)[4], int& codeIndex) {
  for (int i = 0; i < 4; i++) {
    code[i] = -1;
  }
  codeIndex = 0;
  Serial.println("Code reset.");
}
