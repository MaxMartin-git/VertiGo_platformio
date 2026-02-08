#include "motors.h"
#include "../include/config.h"
#include "../include/commands.h"

// Pin-Zuweisungen
#define leftBackward  1
#define leftForward   2
#define leftVelocity  3
#define rightBackward 4
#define rightForward  5
#define rightVelocity 6
extern bool enableMotors;

void setMotorpins() {
    pinMode(leftForward, OUTPUT);
    pinMode(leftBackward, OUTPUT);
    pinMode(leftVelocity, OUTPUT);
    pinMode(rightForward, OUTPUT);
    pinMode(rightBackward, OUTPUT);
    pinMode(rightVelocity, OUTPUT);
}

void driveMotors(const MotorCmd &cmd) {
    if (!enableMotors) {
        digitalWrite(leftForward, LOW);
    #include "../include/commands.h"
        digitalWrite(leftBackward, LOW);
        digitalWrite(rightForward, LOW);
        digitalWrite(rightBackward, LOW);
        return;
    }
    if (cmd.leftDir >= 0) {
        digitalWrite(leftForward, HIGH);
        digitalWrite(leftBackward, LOW);
    extern bool enableMotors;
    } else {
        digitalWrite(leftForward, LOW);
        digitalWrite(leftBackward, HIGH);
    }
    analogWrite(leftVelocity, cmd.leftPWM);

    if (cmd.rightDir >= 0) {
        digitalWrite(rightForward, HIGH);
        digitalWrite(rightBackward, LOW);
    } else {
        digitalWrite(rightForward, LOW);
        digitalWrite(rightBackward, HIGH);
    }
    analogWrite(rightVelocity, cmd.rightPWM);
}
