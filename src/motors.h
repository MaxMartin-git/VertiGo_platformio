#ifndef MOTORS_H
#define MOTORS_H

#include <Arduino.h>
#include "../include/commands.h"

void setMotorpins();
void driveMotors(const MotorCmd &cmd);

#endif
