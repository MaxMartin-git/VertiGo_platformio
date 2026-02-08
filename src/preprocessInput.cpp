#include "preprocessInput.h"
#include "../include/config.h"
#include "commands.h"


bool handleStateRequest(const String &req, WiFiClient &client) {

    //Antriebsfreigabe auslesen
    if (req.indexOf("/?motor=on") != -1) {
        enableMotors = true;
        Serial.println("enableMotors: true");
        return true;
    }

    if (req.indexOf("/?motor=off") != -1) {
        enableMotors = false;
        Serial.println("enableMotors: false");
		return true;
    }

    //Fahrmodus auslesen
    if (req.indexOf("/?steeringmode=manual") != -1) {
        mode = MANUAL;
        Serial.println("mode: MANUAL;");
		return false;
    }

    if (req.indexOf("/?steeringmode=auto") != -1) {
        mode = WALL_ALIGN;
        Serial.println("mode: WALL_ALIGN;");
		return false;
    }
    Serial.println("Zweiter Req-Durchlauf");
    return false;
}

void handleJoystickRequest(const String &req, WiFiClient &client) {

    lastCommandTime = millis();

    //Joystick-Steuerung mit gleichwertiger Skalierung von x und y
    int posX = req.indexOf("x=");
    int posY = req.indexOf("y=");

    if (posX == -1 || posY == -1) return; //zur Sicherheit wird geprüft, ob beide Werte vorliegen, um falsche Ansteuerung zu vermeiden

    float fx = req.substring(posX + 2, req.indexOf("&", posX)).toFloat();
    float fy = req.substring(posY + 2).toFloat();

    // Joystick → Motorwerte
    int velocity = round(fy * 125);
    int steering = round(fx * 125);

    int maxLeft  = velocity + steering;
    int maxRight = velocity - steering;

    int maxPWM = max(abs(maxLeft), abs(maxRight));
    if (maxPWM > 255) {
        float scale = 255.0 / maxPWM;
        velocity = round(velocity * scale);
    }

    manualCmd.leftDir  = (maxLeft  >= 0) ? 1 : -1;
    manualCmd.rightDir = (maxRight >= 0) ? 1 : -1;

    manualCmd.leftPWM  = constrain(abs(maxLeft), 0, 255);
    manualCmd.rightPWM = constrain(abs(maxRight), 0, 255);
	
	// send short HTTP-response
	/*client.println("HTTP/1.1 200 OK");
	client.println("Connection: close");
	client.println();*/
}