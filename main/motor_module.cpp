/** @file motor_module.cpp */

#include "motor_module.h"

// Servo motor object
Servo servo;
int angle = 0;

// Initialize the motor
void setupMotor(int pin) {
    servo.attach(pin);
    servo.write(angle);
}

// Lock or unlock based on the accessGranted status
void setLock(bool accessGranted) {
    if (accessGranted) {
        servo.write(170); // Unlock position
    } else {
        servo.write(0);  // Lock position
    }
}
