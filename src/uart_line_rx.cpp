#include "uart_line_rx.h"
#include <Arduino.h>
#include "../include/config.h"

namespace {
constexpr uint32_t kTimeoutMs = UART_TEST_TIMEOUT_MS;
constexpr uint8_t kBufSize = 16;

char g_buf[kBufSize];
uint8_t g_idx = 0;
int g_error = 0;
uint32_t g_lastRxMs = 0;
uint32_t g_lastPrintMs = 0;

void handleLine() {
  g_buf[g_idx] = '\0';
  if (g_buf[0] == 'E' && g_buf[1] == ':') {
    g_error = constrain(atoi(g_buf + 2), -100, 100);
    g_lastRxMs = millis();
  }
  g_idx = 0;
}
}

void handleError(int error) __attribute__((weak));
void handleError(int error) {
  (void)error;
}

void uartLineRxInit() {
  Serial.println("UART test receiver enabled (Serial1). Expecting E:<int>\\n.");
}

void uartLineRxPoll() {
  while (Serial1.available() > 0) {
    char c = static_cast<char>(Serial1.read());
    if (c == '\n') {
      handleLine();
    } else if (g_idx < kBufSize - 1) {
      g_buf[g_idx++] = c;
    }
  }

  if (millis() - g_lastRxMs > kTimeoutMs) {
    g_error = 0;
  }

  handleError(g_error);

  if (millis() - g_lastPrintMs >= 100) {
    g_lastPrintMs = millis();
    Serial.print("UART error=");
    Serial.println(g_error);
  }
}
