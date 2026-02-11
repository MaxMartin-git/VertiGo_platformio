# LINE FOLLOWER - Arduino Integration

## Übersicht

Dieses Modul ermöglicht Vision-basiertes Line-Following mit:
- **Raspberry Pi** mit Pi-Camera (sendet Offset-Werte via UART)
- **Arduino** mit Motorsteuerung (empfängt Werte und steuert Motoren)

## Dateien

### Arduino (PlatformIO)
- `line_follower_control.h/cpp` - PID-Controller für sanfte Motorsteuerung
- `line_follower_test.cpp` - Standalone Test-Programm (nur Line Following)
- `motors.h/cpp` - Motorsteuerung (bereits vorhanden)

### Raspberry Pi
- `line_follower.py` - Vision-Skript mit Pi-Camera und UART-Kommunikation

## Hardware-Verbindung

```
Raspberry Pi GPIO    →    Arduino
─────────────────────────────────────
TX (GPIO 14)         →    RX1 (Pin 0 auf Mega, Pin 0 auf Uno*)
GND                  →    GND

* Bei Arduino Uno: Serial1 nicht verfügbar, nutze SoftwareSerial
```

**UART-Parameter:**
- Baudrate: 115200
- Format: 8N1 (8 data bits, no parity, 1 stop bit)
- Protokoll: `E:<int>\n` (z.B. `E:-25\n`)

## Verwendung

### 1. Test-Modus (nur Line Following)

**PlatformIO kompilieren:**
```bash
# In platformio.ini:
[env:line_follower_test]
platform = atmelavr
board = megaatmega2560  # oder dein Board
framework = arduino
build_src_filter = 
    +<motors.cpp>
    +<line_follower_control.cpp>
    +<line_follower_test.cpp>
    -<*.cpp>
    -<*.c>

# Kompilieren und hochladen
pio run -e line_follower_test --target upload
```

**Raspberry Pi starten:**
```bash
# Service aktivieren (siehe INSTALL_AUTOSTART.md)
sudo systemctl start line_follower.service

# Oder manuell
python3 line_follower.py
```

**Was passiert:**
1. Arduino startet, wartet auf UART-Verbindung
2. Raspberry Pi sendet kontinuierlich Offset-Werte
3. Arduino piept 3x wenn Verbindung hergestellt
4. Motoren starten automatisch, folgen der Linie
5. Bei UART-Timeout (300ms): Motoren stoppen

### 2. Integration in bestehendes Projekt

**In main.cpp einfügen:**
```cpp
#include "line_follower_control.h"
#include "uart_line_rx.h"  // Falls vorhanden

void setup() {
    // ... andere Initialisierungen
    lineFollowerInit();
}

void loop() {
    // 1. Fehler von UART oder einer anderen Quelle abrufen
    int error = g_error;  // z.B. aus uart_line_rx.cpp
    
    // 2. Line Follower Controller aktualisieren
    lineFollowerUpdate(error);
    
    // 3. Motor-Kommando abrufen
    MotorCmd cmd = getLineFollowerCmd();
    
    // 4. Motoren ansteuern
    driveMotors(cmd);
}
```

## PID-Tuning

Die PID-Parameter können zur Laufzeit angepasst werden:

```cpp
// In setup() oder zur Laufzeit:
setLineFollowerPID(2.0, 0.05, 1.0);  // Kp, Ki, Kd
setLineFollowerSpeed(80);             // Basis-Geschwindigkeit (0-255)
```

**Parameter-Bedeutung:**
- **Kp (Proportional)**: Stärke der sofortigen Reaktion auf Fehler
  - Zu hoch: Oszillation, zu niedrig: träge Reaktion
  - Empfohlen: 1.5 - 3.0
  
- **Ki (Integral)**: Korrektur von dauerhaften Abweichungen
  - Zu hoch: Überschwingen, zu niedrig: Drift
  - Empfohlen: 0.01 - 0.1
  
- **Kd (Derivative)**: Dämpfung von schnellen Änderungen
  - Zu hoch: zu viel Dämpfung, zu niedrig: Oszillation
  - Empfohlen: 0.5 - 2.0

**Tuning-Prozess:**
1. Start mit Kp=2.0, Ki=0, Kd=0
2. Erhöhe Kp bis leichte Oszillation auftritt
3. Reduziere Kp um 20%
4. Füge Ki hinzu (klein anfangen: 0.01)
5. Füge Kd hinzu für Dämpfung

## Geschwindigkeit anpassen

```cpp
setLineFollowerSpeed(80);  // Langsam (stabiler)
setLineFollowerSpeed(120); // Mittel
setLineFollowerSpeed(160); // Schnell (weniger stabil)
```

Je höher die Geschwindigkeit, desto kritischer wird das PID-Tuning!

## Debugging

**Arduino Serial Monitor:**
```
Line Follower Control initialized
UART (Serial1) initialized @ 115200 baud
✓ UART connected to Raspberry Pi!
LineFollower: error=-25 | Left PWM=105 | Right PWM=55
LineFollower: error=10 | Left PWM=70 | Right PWM=90
...
```

**Wichtige Werte:**
- `error`: Offset von der Bildmitte (negativ=links, positiv=rechts)
- `Left/Right PWM`: Motorgeschwindigkeiten (0-255)

**Bei Problemen:**
- Keine UART-Verbindung → Verkabelung prüfen, Baudrate checken
- Motoren stoppen → UART-Timeout, Raspberry Pi prüfen
- Oszillation → Kp reduzieren, Kd erhöhen
- Zu langsame Reaktion → Kp erhöhen
- Drift zur Seite → Ki leicht erhöhen

## Raspberry Pi Logs überwachen

```bash
# Live-Logs anschauen
sudo journalctl -u line_follower.service -f

# Letzten 100 Zeilen
sudo journalctl -u line_follower.service -n 100

# Log-Datei direkt
tail -f /tmp/line_follower.log
```

## Advanced: Multiple Control Modes

Falls du mehrere Modi brauchst (Manual, Line Following, Wall Following):

```cpp
enum ControlMode {
    MANUAL,
    LINE_FOLLOWING,
    WALL_ALIGN
};

ControlMode currentMode = LINE_FOLLOWING;

void loop() {
    switch(currentMode) {
        case LINE_FOLLOWING:
            lineFollowerUpdate(g_error);
            driveMotors(getLineFollowerCmd());
            break;
        case MANUAL:
            driveMotors(manualCmd);
            break;
        // ... andere Modi
    }
}
```

## Siehe auch

- [INSTALL_AUTOSTART.md](../../vertigoCV/INSTALL_AUTOSTART.md) - Pi automatisch starten
- `uart_line_rx.cpp` - UART Empfangs-Implementierung
- `motors.cpp` - Motor-Ansteuerung
