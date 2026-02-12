#ifndef CONFIG_H
#define CONFIG_H

// --------------------------------------------------------
//  WLAN-KONFIGURATION
#define WIFI_SSID  "VertiGo"
#define WIFI_PASS  "12345678"
#define WIFI_PORT     80

// --------------------------------------------------------
// UART-TEST (Pi -> Arduino, one-way)
// Set to 0 to disable test receiver in main.
#define UART_TEST_BAUD 115200
#define UART_TEST_TIMEOUT_MS 300

#define BUZZER 9

//  Konstanten
//#define DEFAULT_VELOCITY  120
//#define MAX_VELOCITY      255   // Max PWM
//#define JOYSTICK_REQUEST_TIME 200
//#define DESIRED_US_DURATION  300

//für Wandfolger
#define BASE_SPEED 200
#define TARGET_MM  150
#define K_AtoB     0.5
#define K_DIST     0.5

#endif
