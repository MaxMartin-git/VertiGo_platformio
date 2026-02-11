#ifndef LINE_FOLLOWER_CONTROL_H
#define LINE_FOLLOWER_CONTROL_H

#include <Arduino.h>
#include "../include/commands.h"

// PID-Controller für sanfte Line-Following
class LineFollowerPID {
public:
    LineFollowerPID(float kp, float ki, float kd, int baseSpeed);
    
    // Berechne Motorkommandos basierend auf Offset-Fehler
    MotorCmd calculate(int error);
    
    // Reset PID-Controller (z.B. nach Neustart)
    void reset();
    
    // Parameter zur Laufzeit anpassen
    void setParameters(float kp, float ki, float kd, int baseSpeed);
    void setBaseSpeed(int speed);
    
private:
    float kp_;          // Proportional gain
    float ki_;          // Integral gain
    float kd_;          // Derivative gain
    int baseSpeed_;     // Basis-Geschwindigkeit
    
    float integral_;    // Integral term
    float lastError_;   // Letzter Fehler für Derivative
    uint32_t lastTime_; // Letzte Berechnungszeit
};

// Initialisierung
void lineFollowerInit();

// Hauptloop-Funktion - muss regelmäßig aufgerufen werden
void lineFollowerUpdate(int error);

// Motor-Kommando abrufen
MotorCmd getLineFollowerCmd();

// Parameter-Einstellungen
void setLineFollowerSpeed(int speed);
void setLineFollowerPID(float kp, float ki, float kd);

#endif
