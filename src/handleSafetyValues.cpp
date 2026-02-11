#include "handleSafetyValues.h"
#include "../include/commands.h"
#include "../include/config.h"

void handleSafetyValues(float tilt) {
    // Buzzer und Safety-Check für Line Following deaktiviert
    // (Kann später wieder aktiviert werden wenn nötig)
    
    // if (tilt > 30.0) {
    //     enableMotors = false;
    //     tone(BUZZER, 1000);
    //     return;                   
    // }
    
    noTone(BUZZER);
    digitalWrite(BUZZER, HIGH); // wird mit HIGH ausgeschaltet
}
