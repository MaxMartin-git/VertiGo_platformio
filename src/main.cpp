
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
#include "line_follower_control.h"

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
  
  // Line Follower Controller initialisieren
  lineFollowerInit();
  setLineFollowerSpeed(80);  // Basis-Geschwindigkeit für Line Following
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

  // Modus-basierte Steuerung
  if (mode == MANUAL) {
      activeCmd = manualCmd;
  } else if (mode == WALL_ALIGN) {
      US_data us = US_measure();
      activeCmd = wallFollowControl(us);
  } else if (mode == LINE_FOLLOWING) {
      // Line Following mit PID Controller
      // Nur wenn UART-Verbindung aktiv ist!
      if (isUartConnected()) {
          int lineError = getLineError();
          lineFollowerUpdate(lineError);
          activeCmd = getLineFollowerCmd();
      } else {
          // Keine Verbindung: Motoren stoppen
          activeCmd = {0, 0, 1, 1};
          static uint32_t lastWarn = 0;
          if (millis() - lastWarn > 2000) {
              lastWarn = millis();
              Serial.println("⚠ LINE_FOLLOWING: Waiting for Pi connection...");
          }
      }
  }
  
  driveMotors(activeCmd);
}
