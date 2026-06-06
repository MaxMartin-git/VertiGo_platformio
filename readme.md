# VertiGo_platformio

VertiGo_platformio is an Arduino/PlatformIO demo project for smart drive control.

## Project Overview

This project runs on Arduino Uno R4 WiFi and includes the following features:

- Read tilt angle using MPU6050 IMU.
- Measure wall distance with dual ultrasonic sensors.
- Use a web interface for mode switching, motor enable/disable, and joystick control.
- Support manual driving and wall-following automatic driving.
- Monitor battery voltage and display battery status.
- Trigger a buzzer alert when tilt exceeds a safe threshold.

## Key Features

| Function           | Description                                                                 |
| ------------------ | --------------------------------------------------------------------------- |
| WiFi Access        | Creates an AP named `VertiGo` for browser-based control.                    |
| Manual Mode        | Uses the web joystick for real-time motor speed and direction control.      |
| Auto Mode          | Calculates control commands from ultrasonic distance measurements to follow a wall automatically. |
| Safety Protection  | Activates a buzzer warning when the tilt angle exceeds 30°.                 |
| Battery Monitoring | Reads battery voltage from A0 and estimates the state of charge.            |

## Hardware Platform

- Arduino Uno R4 WiFi
- MPU6050 accelerometer/gyroscope module
- Two ultrasonic sensors (HC-SR04 or compatible)
- Two DC motor channels
- Buzzer
- Battery voltage sensing circuit

## Software Dependencies

The project uses PlatformIO with the Arduino framework, and depends on:

- `NewPing`
- `I2Cdevlib-Core`
- `I2Cdevlib-MPU6050`

## Build and Upload

1. Open the `VertiGo_AIIT` project folder.
2. Select environment `uno_r4_wifi` in PlatformIO.
3. Build and upload to the Arduino Uno R4 WiFi.

If using the command line:

```bash
platformio run --environment uno_r4_wifi
platformio run --environment uno_r4_wifi --target upload
```

## Usage

1. Power on the device. It starts a WiFi AP named `VertiGo`.
2. Connect a phone or computer to the hotspot and open the default IP address shown in the serial monitor.
3. In the web control panel:
   - Switch `FREIGEBEN` / `SPERREN` to enable or disable motor output.
   - Switch `Manuell` / `Auto` to select driving mode.
   - Use the virtual joystick for movement in manual mode.
4. In auto mode, the system follows the wall using ultrasonic measurements.
5. If tilt exceeds 30°, the buzzer alerts to warn of unsafe conditions.

## Project Structure

| File / Path | Description |
| --- | --- |
| `platformio.ini` | PlatformIO configuration. |
| `models` | 3D step models. |
| `src/main.cpp` | main entry and loop logic. |
| `src/IMU.cpp` | MPU6050 position and tilt calculation. |
| `src/ultrasonic.cpp` | dual ultrasonic distance measurement. |
| `src/wallfollow.cpp` | wall-following control algorithm. |
| `src/motors.cpp` | motor driver output. |
| `src/wifi_handler.cpp` | HTTP request handling. |
| `src/webpage.cpp` | web page generation and interaction. |
| `src/batterieStatus.cpp` | battery voltage read and SOC estimate. |
| `include/config.h` | WiFi and control configuration. |
| `include/commands.h` | command and mode definitions. |

## Future Improvements

- Add real left/right ultrasonic hardware switching.
- Refine automatic driving with PID control.
- Add more safety checks like current or temperature monitoring.
- Improve web UI with real-time status and sensor history charts.

## Contribution and Support

Issues and pull requests are welcome.
