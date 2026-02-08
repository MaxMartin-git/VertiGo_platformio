
// PWM-Werte für die Motoren (immer positiv, 0..255)
extern int leftPWM;
extern int rightPWM;

// Richtung für die Motoren: 1 = vorwärts, -1 = rückwärts
extern int leftDir;
extern int rightDir;

// Motors aktivieren/deaktivieren
extern bool enableMotors;

//timestamp des letzten Kommandos
extern unsigned long lastCommandTime;