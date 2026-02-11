#include "line_follower_control.h"
#include <Arduino.h>

namespace {
// Standardwerte
constexpr float kDefaultKp = 2.0f;      // Proportional: stärker reagieren auf Fehler
constexpr float kDefaultKi = 0.05f;     // Integral: langsame Korrektur von Dauerfehlern
constexpr float kDefaultKd = 1.0f;      // Derivative: Dämpfung von schnellen Änderungen
constexpr int kDefaultBaseSpeed = 80;   // Basis-Geschwindigkeit (80/255 = ~30%)
constexpr int kMaxSpeed = 200;          // Maximale Geschwindigkeit
constexpr int kMinSpeed = 40;           // Minimale Geschwindigkeit

LineFollowerPID* g_pid = nullptr;
MotorCmd g_motorCmd = {0, 0, 1, 1};
}

// ============================================================================
// LineFollowerPID Implementation
// ============================================================================

LineFollowerPID::LineFollowerPID(float kp, float ki, float kd, int baseSpeed)
    : kp_(kp), ki_(ki), kd_(kd), baseSpeed_(baseSpeed),
      integral_(0.0f), lastError_(0.0f), lastTime_(millis()) {
}

void LineFollowerPID::reset() {
    integral_ = 0.0f;
    lastError_ = 0.0f;
    lastTime_ = millis();
}

void LineFollowerPID::setParameters(float kp, float ki, float kd, int baseSpeed) {
    kp_ = kp;
    ki_ = ki;
    kd_ = kd;
    baseSpeed_ = baseSpeed;
}

void LineFollowerPID::setBaseSpeed(int speed) {
    baseSpeed_ = constrain(speed, kMinSpeed, kMaxSpeed);
}

MotorCmd LineFollowerPID::calculate(int error) {
    uint32_t now = millis();
    float dt = (now - lastTime_) / 1000.0f; // Sekunden
    
    if (dt <= 0.0f) {
        dt = 0.01f; // Mindestens 10ms
    }
    
    lastTime_ = now;
    
    // PID-Berechnung
    // error > 0: Linie ist rechts von der Bildmitte → nach rechts lenken
    // error < 0: Linie ist links von der Bildmitte → nach links lenken
    
    // Proportional
    float p = kp_ * error;
    
    // Integral (mit Anti-Windup)
    integral_ += error * dt;
    integral_ = constrain(integral_, -100.0f, 100.0f);
    float i = ki_ * integral_;
    
    // Derivative
    float derivative = (error - lastError_) / dt;
    float d = kd_ * derivative;
    
    lastError_ = error;
    
    // Gesamt-Korrektur
    float correction = p + i + d;
    
    // Motor-Geschwindigkeiten berechnen
    // Positive Korrektur: rechts Motor schneller (Rechtskurve)
    // Negative Korrektur: links Motor schneller (Linkskurve)
    
    int leftSpeed = baseSpeed_ - correction;
    int rightSpeed = baseSpeed_ + correction;
    
    // Auf erlaubte Bereiche begrenzen
    leftSpeed = constrain(leftSpeed, kMinSpeed, kMaxSpeed);
    rightSpeed = constrain(rightSpeed, kMinSpeed, kMaxSpeed);
    
    // MotorCmd erstellen
    MotorCmd cmd;
    cmd.leftPWM = abs(leftSpeed);
    cmd.rightPWM = abs(rightSpeed);
    cmd.leftDir = (leftSpeed >= 0) ? 1 : -1;
    cmd.rightDir = (rightSpeed >= 0) ? 1 : -1;
    
    return cmd;
}

// ============================================================================
// Public Functions
// ============================================================================

void lineFollowerInit() {
    if (g_pid == nullptr) {
        g_pid = new LineFollowerPID(kDefaultKp, kDefaultKi, kDefaultKd, kDefaultBaseSpeed);
    }
    g_motorCmd = {0, 0, 1, 1};
    Serial.println("Line Follower Control initialized");
}

void lineFollowerUpdate(int error) {
    if (g_pid == nullptr) {
        lineFollowerInit();
    }
    
    // Berechne neue Motor-Kommandos
    g_motorCmd = g_pid->calculate(error);
    
    // Debug-Ausgabe (nur alle 200ms)
    static uint32_t lastPrint = 0;
    if (millis() - lastPrint >= 200) {
        lastPrint = millis();
        Serial.print("LineFollower: error=");
        Serial.print(error);
        Serial.print(" | Left PWM=");
        Serial.print(g_motorCmd.leftPWM);
        Serial.print(" | Right PWM=");
        Serial.println(g_motorCmd.rightPWM);
    }
}

MotorCmd getLineFollowerCmd() {
    return g_motorCmd;
}

void setLineFollowerSpeed(int speed) {
    if (g_pid == nullptr) {
        lineFollowerInit();
    }
    g_pid->setBaseSpeed(speed);
    Serial.print("Line Follower base speed set to: ");
    Serial.println(speed);
}

void setLineFollowerPID(float kp, float ki, float kd) {
    if (g_pid == nullptr) {
        lineFollowerInit();
    }
    g_pid->setParameters(kp, ki, kd, kDefaultBaseSpeed);
    Serial.print("PID parameters updated: Kp=");
    Serial.print(kp);
    Serial.print(", Ki=");
    Serial.print(ki);
    Serial.print(", Kd=");
    Serial.println(kd);
}
