#include "handleSafetyValues.h"
#include "commands.h"
#include "../include/config.h"

void handleSafetyValues(float tilt) {
    if (tilt > 30.0) {
        enableMotors = false;
        tone(BUZZER, 1000);
        return;                   
    }
    
    noTone(BUZZER);
    digitalWrite(BUZZER, HIGH); // wird mit HIGH ausgeschaltet
}
