#include "batterieStatus.h"
#include <Arduino.h>

float batteryVoltage = 0.0;
int batterySoC = 0;

void batterieStatus() {
  static unsigned long lastBattStatus = 0; // nur beim ersten Aufruf 0
  if (millis() - lastBattStatus < 500) return;
  lastBattStatus = millis();

  batteryVoltage = analogRead(A0) * (25.0 / 1023.0);

  if      (batteryVoltage >= 8.4) batterySoC = 100;
  else if (batteryVoltage >= 8.2) batterySoC = 90;
  else if (batteryVoltage >= 8.0) batterySoC = 80;
  else if (batteryVoltage >= 7.9) batterySoC = 70;
  else if (batteryVoltage >= 7.8) batterySoC = 60;
  else if (batteryVoltage >= 7.7) batterySoC = 50;
  else if (batteryVoltage >= 7.6) batterySoC = 40;
  else if (batteryVoltage >= 7.5) batterySoC = 30;
  else if (batteryVoltage >= 7.4) batterySoC = 20;
  else if (batteryVoltage >= 7.2) batterySoC = 10;
  else batterySoC = 0;
}
