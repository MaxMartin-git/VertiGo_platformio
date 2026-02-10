
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
#include "uart_line_rx.h"

unsigned long lastIMU = 0;

WiFiServer server(WIFI_PORT);

void setup() {
  Serial.begin(115200);
  delay(200);

#if ENABLE_UART_TEST
  Serial1.begin(UART_TEST_BAUD);
  uartLineRxInit();
#endif

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

#if ENABLE_UART_TEST
  uartLineRxPoll();
#endif

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
