# VertiGo_platformio

VertiGo_platformio 是一个基于 Arduino/PlatformIO 的智能驱动演示项目。

## 项目概述

该项目运行于 Arduino Uno R4 WiFi，集成以下功能：

- 通过 MPU6050 IMU 读取倾斜角度。
- 通过双超声波传感器测量墙面距离。
- 通过网页界面进行模式切换、驱动启停与虚拟摇杆控制。
- 实现手动驾驶与墙壁跟踪自动驾驶模式。
- 电池电压监测与状态显示。
- 过倾角时发出蜂鸣器警报。

演示视频：https://youtube.com/shorts/a0h48lALChM; https://youtu.be/RbbMbN5aSW8

## 关键特性

| 功能             | 描述                                                                 |
| ---------------- | -------------------------------------------------------------------- |
| WiFi 访问        | 创建 AP 名称 `VertiGo`，通过浏览器访问控制面板。                         |
| 手动模式         | 通过网页摇杆实时控制电机转速与方向。                                  |
| 自动模式         | 根据超声波距离计算控制量，实现沿墙自动行驶。                             |
| 安全保护         | 倾斜角度超过 30° 时触发蜂鸣器提醒。                                     |
| 电池监测         | 通过 A0 模拟输入读取电压并估算电池电量。                               |

## 硬件平台

- Arduino Uno R4 WiFi
- MPU6050 加速度计 / 陀螺仪模块
- 两个超声波传感器（HC-SR04 或兼容型号）
- 两个直流电机驱动通道
- 蜂鸣器
- 电池电压检测电路

## 软件依赖

本项目使用 PlatformIO 和 Arduino 框架，依赖以下库：

- `NewPing`
- `I2Cdevlib-Core`
- `I2Cdevlib-MPU6050`

## 编译与上传

1. 打开项目目录 `VertiGo_AIIT`。
2. 在 PlatformIO 中选择环境 `uno_r4_wifi`。
3. 编译并上传到 Arduino Uno R4 WiFi。

如果使用命令行：

```bash
platformio run --environment uno_r4_wifi
platformio run --environment uno_r4_wifi --target upload
```

## 使用说明

1. 上电后，设备启动 WiFi AP：`VertiGo`。
2. 手机或电脑连接该热点，访问串口监视器中显示的默认 IP 地址。
3. 在网页控制面板中：
   - 切换 `FREIGEBEN` / `SPERREN` 控制电机输出。
   - 切换 `Manuell` / `Auto` 驱动模式。
   - 在手动模式下使用页面中的虚拟摇杆控制运动。
4. 自动模式下，系统会使用超声波测距结果进行墙壁跟踪控制。
5. 若倾斜角度超过 30°，蜂鸣器会发出警报以提示危险状态。

## 项目结构

| 文件 / 路径 | 描述 |
| --- | --- |
| `platformio.ini` | PlatformIO 配置。 |
| `models` | 3D STEP 模型。 |
| `src/main.cpp` | 项目入口，主循环逻辑。 |
| `src/IMU.cpp` | MPU6050 传感器读取与倾斜角度计算。 |
| `src/ultrasonic.cpp` | 双超声波传感器测距。 |
| `src/wallfollow.cpp` | 墙壁跟踪控制算法。 |
| `src/motors.cpp` | 电机驱动输出。 |
| `src/wifi_handler.cpp` | HTTP 请求处理。 |
| `src/webpage.cpp` | 网页界面生成与交互。 |
| `src/batterieStatus.cpp` | 电池电压读取与 SOC 估算。 |
| `include/config.h` | WiFi 与控制参数配置。 |
| `include/commands.h` | 控制命令与模式定义。 |

## 未来改进建议

- 增加真正的左右超声波传感器硬件切换。
- 细化自动驾驶 PID 控制策略。
- 添加更多安全检测，如电流/温度监控。
- 提升网页显示：实时状态、传感器历史曲线。

## 贡献与支持

欢迎提交 issue 或 pull request。
