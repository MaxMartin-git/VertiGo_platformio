# VertiGo_AIIT

VertiGo_AIIT is an Arduino/PlatformIO demo project for smart drive control.  
VertiGo_AIIT 是一个基于 Arduino/PlatformIO 的智能驱动演示项目。

## Project Overview / 项目概述

This project runs on Arduino Uno R4 WiFi and includes the following features:  
该项目运行于 Arduino Uno R4 WiFi，集成以下功能：

- Read tilt angle using MPU6050 IMU.  
- 通过 MPU6050 IMU 读取倾斜角度。
- Measure wall distance with dual ultrasonic sensors.  
- 通过双超声波传感器测量墙面距离。
- Use a web interface for mode switching, motor enable/disable, and joystick control.  
- 通过网页界面进行模式切换、驱动启停与虚拟摇杆控制。
- Support manual driving and wall-following automatic driving.  
- 实现手动驾驶与墙壁跟踪自动驾驶模式。
- Monitor battery voltage and display battery status.  
- 电池电压监测与状态显示。
- Trigger a buzzer alert when tilt exceeds a safe threshold.  
- 过倾角时发出蜂鸣器警报。

## Key Features / 关键特性

- WiFi access: creates an AP named `VertiGo` for browser control.  
- `WiFi` 接入：创建 AP 名称 `VertiGo`，通过浏览器访问控制面板。
- Manual mode: use the web joystick for real-time motor control.  
- `手动模式`：网页摇杆实时控制电机转速与方向。
- Auto mode: wall-following control based on ultrasonic distance readings.  
- `自动模式`：根据超声波距离计算控制量，实现沿墙行驶。
- Safety protection: buzzer warning when tilt angle exceeds 30°.  
- `安全保护`：倾斜角度超过 30° 时触发蜂鸣器提醒。
- Battery monitoring: read voltage from A0 and estimate state of charge.  
- `电池监测`：通过 A0 模拟输入读取电压并估算电量。

## Hardware Platform / 硬件平台

- Arduino Uno R4 WiFi  
- Arduino Uno R4 WiFi
- MPU6050 accelerometer/gyroscope module  
- MPU6050 加速度计 / 陀螺仪模块
- Two ultrasonic sensors (HC-SR04 or compatible)  
- 两个超声波传感器（HC-SR04 或兼容型号）
- Two DC motor channels  
- 两个直流电机驱动通道
- Buzzer  
- 蜂鸣器
- Battery voltage sensing circuit  
- 电池电压检测电路

## Software Dependencies / 软件依赖

The project uses PlatformIO with the Arduino framework, and depends on:  
项目使用 PlatformIO 和 Arduino 框架，依赖库如下：

- `NewPing`  
- `NewPing`
- `I2Cdevlib-Core`  
- `I2Cdevlib-Core`
- `I2Cdevlib-MPU6050`  
- `I2Cdevlib-MPU6050`

## Build and Upload / 编译与上传

1. Open the `VertiGo_AIIT` project folder.  
1. 打开项目目录 `VertiGo_AIIT`。
2. Select environment `uno_r4_wifi` in PlatformIO.  
2. 在 PlatformIO 中选择环境 `uno_r4_wifi`。
3. Build and upload to the Arduino Uno R4 WiFi.  
3. 编译并上传到 `Arduino Uno R4 WiFi`。

If using the command line:  
如果使用命令行：

```bash
platformio run --environment uno_r4_wifi
platformio run --environment uno_r4_wifi --target upload
```

## Usage / 使用说明

1. Power on the device. It starts a WiFi AP named `VertiGo`.  
1. 上电后，设备启动 WiFi AP：`VertiGo`。
2. Connect a phone or computer to the hotspot and open the default IP address shown in the serial monitor.  
2. 手机或电脑连接该热点，访问默认 IP 地址（Arduino 会在串口打印）。
3. In the web control panel:  
3. 在网页控制面板中：
   - Switch `FREIGEBEN` / `SPERREN` to enable or disable motor output.  
   - 切换 `FREIGEBEN` / `SPERREN` 控制电机输出。
   - Switch `Manuell` / `Auto` to select driving mode.  
   - 切换 `Manuell` / `Auto` 驱动模式。
   - Use the virtual joystick for movement in manual mode.  
   - 在手动模式下使用页面中的虚拟摇杆控制运动。
4. In auto mode, the system follows the wall using ultrasonic measurements.  
4. 自动模式下，系统会使用超声波测距结果进行墙壁跟踪控制。
5. If tilt exceeds 30°, the buzzer alerts to warn of unsafe conditions.  
5. 若倾斜角度超过 30°，蜂鸣器会发出警报以提示危险状态。

## Project Structure / 项目结构

- `platformio.ini`: PlatformIO configuration.  
- `platformio.ini`：PlatformIO 配置。
- `src/main.cpp`: main entry and loop logic.  
- `src/main.cpp`：项目入口，主循环逻辑。
- `src/IMU.cpp`: MPU6050 position and tilt calculation.  
- `src/IMU.cpp`：MPU6050 传感器读取与倾斜角度计算。
- `src/ultrasonic.cpp`: dual ultrasonic distance measurement.  
- `src/ultrasonic.cpp`：双超声波传感器测距。
- `src/wallfollow.cpp`: wall-following control algorithm.  
- `src/wallfollow.cpp`：墙壁跟踪控制算法。
- `src/motors.cpp`: motor driver output.  
- `src/motors.cpp`：电机驱动输出。
- `src/wifi_handler.cpp`: HTTP request handling.  
- `src/wifi_handler.cpp`：HTTP 请求处理。
- `src/webpage.cpp`: web page generation and interaction.  
- `src/webpage.cpp`：网页界面生成与交互。
- `src/batterieStatus.cpp`: battery voltage read and SOC estimate.  
- `src/batterieStatus.cpp`：电池电压读取与 SOC 估算。
- `include/config.h`: WiFi and control configuration.  
- `include/config.h`：WiFi 与控制参数配置。
- `include/commands.h`: command and mode definitions.  
- `include/commands.h`：控制命令与模式定义。

## Future Improvements / 未来改进建议

- Add real left/right ultrasonic hardware switching.  
- 增加真正的左右超声波传感器硬件切换。
- Refine automatic driving with PID control.  
- 细化自动驾驶 PID 控制策略。
- Add more safety checks like current or temperature monitoring.  
- 添加更多安全检测，如电流/温度监控。
- Improve web UI with real-time status and sensor history charts.  
- 提升网页显示：实时状态、传感器历史曲线。

## Contribution and Support / 贡献与支持

Issues and pull requests are welcome.  
欢迎提交 issue 或 pull request。
