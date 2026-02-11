/*
 * LINE FOLLOWER TEST - Standalone
 * 
 * Einfaches Test-Programm für Line-Following mit Pi-Camera.
 * 
 * Funktionalität:
 * - Empfängt Offset-Werte von Raspberry Pi über UART (Serial1)
 * - Nutzt PID-Controller für sanfte Motorsteuerung
 * - Gibt Debug-Infos über Serial (USB) aus
 * 
 * Hardware:
 * - Serial1 (UART): RX von Raspberry Pi (115200 baud)
 * - Motoren über H-Bridge
 * - Optional: Buzzer auf Pin 9
 * 
 * Start-Prozedur:
 * 1. Power on
 * 2. Wartet auf UART-Verbindung (3 Piep-Töne wenn verbunden)
 * 3. Startet automatisch Line-Following
 * 
 * Stopp:
 * - Bei Timeout (300ms ohne neue Daten): Motoren stoppen
 */

#include <Arduino.h>
#include "motors.h"
#include "line_follower_control.h"

// ============================================================================
// Konfiguration
// ============================================================================
constexpr uint32_t kUartBaud = 115200;
constexpr uint32_t kTimeoutMs = 300;     // Timeout ohne UART-Daten
constexpr uint8_t kBuzzerPin = 9;
constexpr uint8_t kBufSize = 16;

// ============================================================================
// Globale Variablen
// ============================================================================
char g_buf[kBufSize];
uint8_t g_idx = 0;
int g_lineError = 0;                     // Offset von Kamera (-100 bis +100)
uint32_t g_lastRxMs = 0;
bool g_motorsEnabled = true;
bool g_connected = false;

// ============================================================================
// UART Empfang
// ============================================================================
void handleLine() {
    g_buf[g_idx] = '\0';
    
    // Erwarte Format: "E:<int>\n"
    if (g_buf[0] == 'E' && g_buf[1] == ':') {
        g_lineError = constrain(atoi(g_buf + 2), -200, 200);
        g_lastRxMs = millis();
        
        // Beim ersten Empfang: Verbindung hergestellt
        if (!g_connected) {
            g_connected = true;
            Serial.println("✓ UART connected to Raspberry Pi!");
            
            // Signal: 3x kurz piepen
            for (int i = 0; i < 3; i++) {
                tone(kBuzzerPin, 2000, 100);
                delay(150);
            }
        }
    }
    g_idx = 0;
}

void uartPoll() {
    while (Serial1.available() > 0) {
        char c = static_cast<char>(Serial1.read());
        if (c == '\n') {
            handleLine();
        } else if (g_idx < kBufSize - 1) {
            g_buf[g_idx++] = c;
        }
    }
    
    // Timeout-Handling: Bei zu langer Wartezeit stoppen
    if (millis() - g_lastRxMs > kTimeoutMs && g_connected) {
        g_lineError = 0;
        g_motorsEnabled = false;
    } else if (g_connected) {
        g_motorsEnabled = true;
    }
}

// ============================================================================
// Setup
// ============================================================================
void setup() {
    // USB Serial für Debug
    Serial.begin(115200);
    while (!Serial && millis() < 3000); // Max 3s warten
    
    Serial.println("\n\n");
    Serial.println("============================================");
    Serial.println("   LINE FOLLOWER - Standalone Test Mode    ");
    Serial.println("============================================");
    Serial.println();
    
    // UART für Pi-Kommunikation
    Serial1.begin(kUartBaud);
    Serial.print("UART (Serial1) initialized @ ");
    Serial.print(kUartBaud);
    Serial.println(" baud");
    Serial.println("Waiting for Raspberry Pi connection...");
    
    // Buzzer
    pinMode(kBuzzerPin, OUTPUT);
    tone(kBuzzerPin, 1000, 200); // Start-Signal
    
    // Motoren initialisieren
    setMotorpins();
    Serial.println("Motor pins configured");
    
    // Line Follower Controller initialisieren
    lineFollowerInit();
    
    // PID-Tuning (bei Bedarf anpassen)
    setLineFollowerPID(2.0, 0.05, 1.0);  // Kp, Ki, Kd
    setLineFollowerSpeed(80);             // Basis-Speed (0-255)
    
    Serial.println("\n✓ Setup complete. Ready for line following!");
    Serial.println("Expected UART format: E:<int>\\n");
    Serial.println();
}

// ============================================================================
// Main Loop
// ============================================================================
void loop() {
    // UART-Daten empfangen
    uartPoll();
    
    // Line Follower Controller aktualisieren
    lineFollowerUpdate(g_lineError);
    
    // Motor-Kommandos abrufen und ausführen
    MotorCmd cmd = getLineFollowerCmd();
    
    if (g_motorsEnabled) {
        driveMotors(cmd);
    } else {
        // Motoren stoppen bei Timeout
        MotorCmd stopCmd = {0, 0, 1, 1};
        driveMotors(stopCmd);
        
        // Warnung alle 2 Sekunden
        static uint32_t lastWarn = 0;
        if (millis() - lastWarn > 2000) {
            lastWarn = millis();
            Serial.println("⚠ Motors STOPPED - UART timeout or no connection");
        }
    }
    
    // Status-LED / Buzzer bei Verbindungsverlust
    static bool wasConnected = false;
    if (wasConnected && !g_motorsEnabled) {
        tone(kBuzzerPin, 500, 100); // Warnsignal
        wasConnected = false;
    } else if (!wasConnected && g_motorsEnabled) {
        wasConnected = true;
    }
    
    // Kleine Verzögerung (Control-Loop sollte ~10-20ms laufen)
    delay(10);
}
