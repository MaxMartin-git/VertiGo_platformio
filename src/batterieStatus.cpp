#include "batterieStatus.h"
#include <Arduino.h>

float batteryVoltage = 0.0;
int batterySoC = 0;

void batterieStatus() {
  static unsigned long lastBattStatus = 0; // nur beim ersten Aufruf 0
  if (millis() - lastBattStatus < 500) return;
  lastBattStatus = millis();

  batteryVoltage = analogRead(A0) * (25.0 / 1023.0);

  if      (batteryVoltage >= 12.60) batterySoC = 100;
  else if (batteryVoltage >= 12.30) batterySoC = 90;
  else if (batteryVoltage >= 12.00) batterySoC = 80;
  else if (batteryVoltage >= 11.85) batterySoC = 70;
  else if (batteryVoltage >= 11.70) batterySoC = 60;
  else if (batteryVoltage >= 11.55) batterySoC = 50;
  else if (batteryVoltage >= 11.40) batterySoC = 40;
  else if (batteryVoltage >= 11.25) batterySoC = 30;
  else if (batteryVoltage >= 11.10) batterySoC = 20;
  else if (batteryVoltage >= 10.90) batterySoC = 10;
  else batterySoC = 0;
}
