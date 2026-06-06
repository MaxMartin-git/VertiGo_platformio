# VertiGo_platformio

VertiGo_platformio ist ein Arduino/PlatformIO-Demoprojekt für intelligente Fahrsteuerung.

## Projektübersicht

Dieses Projekt läuft auf dem Arduino Uno R4 WiFi und bietet folgende Funktionen:

- Misst den Neigungswinkel mit dem MPU6050 IMU.
- Erfasst die Wanddistanz mit zwei Ultraschallsensoren.
- Nutzt eine Web-Oberfläche für Mode-Umschaltung, Motor ein/aus und Joystick-Steuerung.
- Unterstützt manuelles Fahren und automatisches Wandfolgen.
- Überwacht Batteriespannung und zeigt den Batteriestatus an.
- Löst einen Summeralarm aus, wenn die Neigung einen sicheren Schwellenwert überschreitet.

## Hauptfunktionen

| Funktion           | Beschreibung |
| ------------------ | --------------------------------------------------------------------------- |
| WiFi-Zugang        | Erstellt einen AP namens `VertiGo` für die Browser-Steuerung.                |
| Manuellmodus       | Nutzt den Web-Joystick zur Echtzeitsteuerung von Motordrehzahl und -richtung. |
| Automodus          | Berechnet Steuerbefehle aus Ultraschallmessungen, um der Wand automatisch zu folgen. |
| Sicherheit         | Aktiviert eine Summerwarnung, wenn der Neigungswinkel 30° überschreitet.     |
| Batteriemonitoring | Liest die Batteriespannung von A0 und schätzt den Ladezustand.              |

## Hardwareplattform

- Arduino Uno R4 WiFi
- MPU6050 Beschleunigungs-/Gyroskopmodul
- Zwei Ultraschallsensoren (HC-SR04 oder kompatibel)
- Zwei Gleichstrom-Motorkanäle
- Summer
- Batteriespannungs-Messschaltung

## Softwareabhängigkeiten

Das Projekt verwendet PlatformIO mit dem Arduino-Framework und benötigt:

- `NewPing`
- `I2Cdevlib-Core`
- `I2Cdevlib-MPU6050`

## Kompilieren und Hochladen

1. Öffnen Sie den Projektordner `VertiGo_AIIT`.
2. Wählen Sie in PlatformIO die Umgebung `uno_r4_wifi` aus.
3. Kompilieren Sie und laden Sie auf den Arduino Uno R4 WiFi hoch.

Wenn Sie die Befehlszeile verwenden:

```bash
platformio run --environment uno_r4_wifi
platformio run --environment uno_r4_wifi --target upload
```

## Verwendung

1. Schalten Sie das Gerät ein. Es startet einen WiFi-AP mit dem Namen `VertiGo`.
2. Verbinden Sie ein Telefon oder einen Computer mit dem Hotspot und öffnen Sie die Standard-IP-Adresse, die im Serial Monitor angezeigt wird.
3. Im Web-Steuerfeld:
   - Schalten Sie `FREIGEBEN` / `SPERREN`, um die Motor-Ausgangsfreigabe ein- oder auszuschalten.
   - Schalten Sie `Manuell` / `Auto`, um den Fahrmodus auszuwählen.
   - Verwenden Sie den virtuellen Joystick zur Bewegung im manuellen Modus.
4. Im Automodus folgt das System mit Ultraschallmessungen der Wand.
5. Überschreitet die Neigung 30°, warnt der Summer vor unsicherem Zustand.

Demo Vedio: https://youtube.com/shorts/a0h48lALChM; https://youtu.be/RbbMbN5aSW8

## Projektstruktur

| Datei / Pfad | Beschreibung |
| --- | --- |
| `platformio.ini` | PlatformIO-Konfiguration. |
| `models` | 3D-STEP-Modelle. |
| `src/main.cpp` | Haupteinstieg und Loop-Logik. |
| `src/IMU.cpp` | MPU6050-Positions- und Neigungsberechnung. |
| `src/ultrasonic.cpp` | Duale Ultraschall-Entfernungsmessung. |
| `src/wallfollow.cpp` | Wandfolgesteuerungsalgorithmus. |
| `src/motors.cpp` | Motorsteuerungs-Ausgabe. |
| `src/wifi_handler.cpp` | HTTP-Anfrageverarbeitung. |
| `src/webpage.cpp` | Webseitenerstellung und Interaktion. |
| `src/batterieStatus.cpp` | Batteriespannungsablesung und SOC-Schätzung. |
| `include/config.h` | WiFi- und Steuerungskonfiguration. |
| `include/commands.h` | Befehls- und Modusdefinitionen. |

## Zukünftige Verbesserungen

- Echtes Links/Rechts-Ultraschall-Hardware-Switching hinzufügen.
- Automatikfahrt mit PID-Regelung verfeinern.
- Weitere Sicherheitsprüfungen wie Strom- oder Temperaturüberwachung hinzufügen.
- Web-UI mit Echtzeitstatus und Sensordatenhistorie verbessern.

## Beitrag und Support

Issues und Pull Requests sind willkommen.
