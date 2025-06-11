# 🤖 Line-Following Robot

This repository documents the development of a line-following robot based on an STM32 microcontroller.

The project integrates embedded control, analog sensing, and a graphical desktop interface for sensor calibration and monitoring.

![LineFollowingRobot](https://github.com/user-attachments/assets/b5fae4a5-53b8-4ba4-b195-d37fce326963)



https://github.com/user-attachments/assets/9db59973-5611-4065-9fe1-802ef03e5322


---

## 🎯 Project Objective

To implement an autonomous system capable of following a black line on a white background using reflective sensors and an embedded control algorithm. The approach includes a calibration phase assisted by a desktop interface built with Qt, which allows users to verify sensor functionality and power supply stability.

---

## ⚙️ System Components

### 🧩 STM32 Firmware (Embedded)
- **Reading reflective sensors** via ADC
- **Calibration of maximum and minimum values** for each sensor
- **Line error calculation and control logic**
- **Motor control using PWM signals**
- **Structured serial communication protocol**
- **Periodic transmission of data to the PC**

### 🖥️ Qt Application (Visualization)
- Automatic detection and connection to serial ports
- Real-time reading of sensor values
- Display of detected minimum and maximum values
- Graphical indicators of signal intensity
- Monitoring of system voltage

---

## 🔄 Communication Protocol

Communication between the microcontroller and the PC is handled through a structured packet format including delimiters, size, data, and checksum verification.

Packet structure (bytes):

```
[START][SIZE][DATA][CHECKSUM][END]
```

Types of data transmitted:
- Sensor calibration values
- Real-time sensor readings
- System voltage level
- Message type identified via control field

---

## 📈 Applications and Use Cases

- Education in mobile robotics and embedded systems
- Prototyping of basic autonomous vehicles
- Testing of control algorithms (P, PD, PID)

---

## 📎 Requirements

- STM32 microcontroller board (e.g., F401 series or similar)
- Reflective sensor module
- Motor driver and geared motors
- Stable power supply

---

## 🔧 Potential Extensions

- Integration of PID control for smoother trajectories
- Sensor session recording in CSV format
- Real-time visualization with plots or graphs
- Wireless data transmission (Bluetooth, WiFi)
- Logic for handling curves or intersections

---

## 👥 Credits

Josh Sebastián López Murcia  
Julián Humberto Lozada Silva

![Pista](https://github.com/user-attachments/assets/257ab0f4-8b12-42e9-88e3-3d6d33177fdf)


