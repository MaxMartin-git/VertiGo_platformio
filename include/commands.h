#pragma once

// Steuermodi
enum ControlMode {
    MANUAL,
    WALL_ALIGN,
    LINE_FOLLOWING
};

// Motorbefehl
struct MotorCmd {
    int leftPWM;
    int rightPWM;
    int leftDir;
    int rightDir;
};

extern ControlMode mode;
extern MotorCmd manualCmd;
extern MotorCmd autoCmd;

extern bool enableMotors;
extern unsigned long lastCommandTime;
