#include "wallfollow.h"
#include "config.h"
#include <Arduino.h>

MotorCmd wallFollowControl(const US_data &us) {
    MotorCmd cmd;

    int errorAtoB = us.dist_B - us.dist_A;
    int errorDist = TARGET_MM - (us.dist_A + us.dist_B)/2;
    int steering = K_AtoB * errorAtoB + K_DIST * errorDist;

    int tmpLeft  = BASE_SPEED - steering;
    int tmpRight = BASE_SPEED + steering;

    cmd.leftDir  = (tmpLeft  >= 0) ? 1 : -1;
    cmd.rightDir = (tmpRight >= 0) ? 1 : -1;

    cmd.leftPWM  = constrain(abs(tmpLeft), 0, 150);
    cmd.rightPWM = constrain(abs(tmpRight), 0, 150);//max: 255

    return cmd;
}