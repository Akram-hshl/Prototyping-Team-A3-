
# Component Assembly Log  
**Date:** May 11, 2025  
**Prepared by:** Onyesi John Abiagam  

## Overview  
This log records the physical setup and wiring of components for our robot during the prototyping stage. The focus was on connecting the motors, sensors, and power system properly before moving into software testing.

## Components Connected  

| Component         | Connection Details |
|------------------|--------------------|
| **DC Motors**     | Connected to L298N motor driver: IN1–D6, IN2–D7, IN3–D8, IN4–D9. ENA on D10, ENB on D5. |
| **Ultrasonic Sensor (HC-SR04)** | Echo → A2, Trig → A3 |
| **IR Sensors (x2)** | Left sensor to A0, Right sensor to A1 |
| **Servo Motor**    | Signal connected to D11, powered using external 5V |
| **Color Sensor (TCS3200)** | S0–S3 connected to D2–D5, OUT pin to D12 |
| **Power Supply** | 9V Li-ion battery to 5V voltage regulator, which powers the Arduino and sensors. All GNDs are linked. |

## What Was Done  
- All components were mounted on the chassis or breadboard.
- Double-checked wire connections for correct pins and voltage levels.
- Powered up the system and tested for signs of life (blinking LEDs, initial sensor response).
- Ran basic test code for IR sensors, ultrasonic sensor, servo, and motors to confirm they’re working.

## Notes  
- No visible signs of short circuits or heating.
- Servo works better when powered from the regulator, not directly from Arduino 5V.
- IR sensors responded correctly to black and white surfaces.
- Distance sensor gave expected readings (within ~2 cm error).
- Color sensor needs more lighting to give consistent results.


