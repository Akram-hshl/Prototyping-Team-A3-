# Component Assembly Report
**Date:** June 22, 2025  
**Author:** [Your Name]  

## ✅ Overview
This document summarizes the component integration completed during the hardware prototyping phase.

## 🔌 Components Assembled & Wiring Summary

| Component         | Connection Details                    |
|------------------|----------------------------------------|
| **DC Motors**     | L298N motor driver: IN1–D6, IN2–D7, IN3–D8, IN4–D9, ENA–D10, ENB–D5 |
| **Ultrasonic Sensor (HC-SR04)** | Echo – A2, Trig – A3                        |
| **IR Sensors (x2)** | IR1 – A0, IR2 – A1                           |
| **Servo Motor**    | Signal – D11, powered via 5V regulator       |
| **Color Sensor (TCS3200)** | Pins D2–D5 (S0, S1, S2, S3); OUT – D12        |
| **Power**         | 7.4V Li-ion battery → 5V Regulator → Arduino + Components |

All GNDs connected together for stable reference.

## 🛠️ Notes:
- Confirmed power delivery to each component.
- All sensors tested for basic power-up response (LEDs blinking, initial readings).
- No overheating or noise during operation.
- Servo runs smoothly under external 5V.

## 📸 Images
> Place wiring photos under `images/` directory.

![Wiring Overview](../images/wiring_overview.jpg)
![Assembled Components](../images/assembled_setup.jpg)

## 🔬 Next Steps
- Start integrating sensor readings into logic.
- Begin testing motor + sensor coordination.
- Log sensor outputs in serial monitor.
