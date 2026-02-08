#ifndef WALLFOLLOW_H
#define WALLFOLLOW_H

#include "ultrasonic.h"
#include "../include/commands.h"

MotorCmd wallFollowControl(const US_data &us);

#endif
