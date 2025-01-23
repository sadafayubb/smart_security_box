/** @file motor_module.h */

#ifndef MOTOR_MODULE_H
#define MOTOR_MODULE_H

#include <Servo.h>

// Function declarations
void setupMotor(int pin);
void setLock(bool accessGranted);

#endif // MOTOR_MODULE_H
