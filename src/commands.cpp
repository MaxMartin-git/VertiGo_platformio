#include "../include/commands.h"

// Aktueller Modus - Standard: LINE_FOLLOWING
ControlMode mode = LINE_FOLLOWING;

// Structinstanzen für Motorbefehle
MotorCmd manualCmd;
MotorCmd autoCmd;

// PWM-Werte für die Motoren
int leftPWM = 0;
int rightPWM = 0;

// Richtung der Motoren
int leftDir = 1;
int rightDir = 1;

// Motoren aktiviert oder deaktiviert
bool enableMotors = true;  // Aktiviert für Line Following

//timestamp des letzten Kommandos
unsigned long lastCommandTime = 0;