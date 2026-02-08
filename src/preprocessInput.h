#ifndef PREPROCESSINPUT_H
#define PREPROCESSINPUT_H

#include <Arduino.h>	// für String
#include <WiFiS3.h>

// Funktionsdeklarationen

bool handleStateRequest(const String &req, WiFiClient &client);

void handleJoystickRequest(const String &req, WiFiClient &client);

#endif