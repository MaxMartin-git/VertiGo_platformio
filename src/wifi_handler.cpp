#include "wifi_handler.h"
#include "preprocessInput.h"
#include "webpage.h"
#include <Arduino.h>
#include <WiFiS3.h>

void handleWiFi(WiFiServer &server) {
    WiFiClient client = server.available();
    if (!client) return;

    unsigned long startTime = millis();
    while (!client.available()) {
        if (millis() - startTime > 2000) {
            client.stop();
            Serial.println("Client Timeout");
            return;
        }
        delay(0);
    }

    String req = client.readStringUntil('\r');
    client.flush();

    Serial.print("Request: ");
    Serial.println(req);

    if (req.indexOf("GET /joy?") != -1) {
        handleJoystickRequest(req, client);
        client.println("HTTP/1.1 200 OK");
        client.println("Connection: close");
        client.println();
        client.stop();
        return;
    }
    else if (req.indexOf("GET /favicon.ico") >= 0) {
        client.println("HTTP/1.1 204 No Content");
        client.println("Connection: close");
        client.println();
        client.stop();
        return;
    }

    if (handleStateRequest(req, client)) {
        client.println("HTTP/1.1 303 See Other");
        client.println("Location: /");
        client.println("Connection: close");
        client.println();
        client.stop();
        Serial.println("Erster Req-Durchlauf");
        return;
    }

    sendWebpage(client);
    client.stop();
}
