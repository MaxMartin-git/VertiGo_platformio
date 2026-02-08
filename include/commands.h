#pragma once

// Steuermodi
enum ControlMode {
    MANUAL,
    WALL_ALIGN
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
