/** @file motor_module.h
 *  @brief Declaration of functions for controlling the servo motor used in the locking mechanism.
 */

#ifndef MOTOR_MODULE_H
#define MOTOR_MODULE_H

#include <Servo.h> ///< Library for controlling servo motors.

/**
 * @brief Initializes the servo motor by attaching it to the specified pin.
 * @param pin The GPIO pin to which the servo motor is connected.
 */
void setupMotor(int pin);

/**
 * @brief Controls the lock state of the motor based on access status.
 * @param accessGranted A boolean value indicating whether access is granted (true to unlock, false to lock).
 */
void setLock(bool accessGranted);

#endif // MOTOR_MODULE_H
