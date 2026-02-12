
#include "../include/config.h"
#include <WiFiS3.h>
#include "../include/commands.h"
#include "ultrasonic.h"
#include "wallfollow.h"
#include "IMU.h"
#include "batterieStatus.h"
#include "wifi_handler.h"
#include "handleSafetyValues.h"
#include "motors.h"


unsigned long lastIMU = 0;

WiFiServer server(WIFI_PORT);

void setup() {
  Serial.begin(115200);
  delay(200);

  WiFi.beginAP(WIFI_SSID, WIFI_PASS);
  Serial.print("AP gestartet, IP: ");
  Serial.println(WiFi.localIP());

  server.begin();

  IMU_setup();
  setMotorpins();
}

void loop() {
  static float tilt = 0;
  MotorCmd activeCmd{};

  if (millis() - lastIMU >= 10) {
      lastIMU = millis();
      tilt = readIMU();
  }

  batterieStatus();
  handleWiFi(server);
  handleSafetyValues(tilt);

  if (mode == MANUAL) {
      activeCmd = manualCmd;
  } else {
      US_data us = US_measure();
      activeCmd = wallFollowControl(us);
  }
  
  driveMotors(activeCmd);
}
