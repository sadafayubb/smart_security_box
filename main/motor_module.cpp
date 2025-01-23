/** @file motor_module.cpp
 *  @brief Implementation of motor control functions for locking and unlocking mechanisms.
 */

#include "motor_module.h"

// Servo motor object
Servo servo; ///< Servo object to control the motor.
int angle = 0; ///< Initial angle of the servo motor.

/**
 * @brief Initializes the motor by attaching it to the specified pin and setting its initial position.
 * @param pin The GPIO pin to which the servo motor is connected.
 */
void setupMotor(int pin) {
    servo.attach(pin);
    servo.write(angle);
}

/**
 * @brief Sets the lock state of the motor based on the accessGranted status.
 * @param accessGranted A boolean value indicating whether access is granted (true to unlock, false to lock).
 */
void setLock(bool accessGranted) {
    if (accessGranted) {
        servo.write(170); ///< Unlock position (angle 170 degrees).
    } else {
        servo.write(0); ///< Lock position (angle 0 degrees).
    }
}
