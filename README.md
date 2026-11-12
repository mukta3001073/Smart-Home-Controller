# ESP32 MQTT-Based Smart Home Controller

## 📌 Project Overview

This project implements a two-channel Smart Home Controller using an ESP32, two push buttons, and two LEDs.

The system allows users to control two LEDs in two different ways:

1. **Physical Button Control:** Each push button controls its corresponding LED.
2. **MQTT Remote Control:** LEDs can be controlled remotely by publishing MQTT messages through an MQTT client.

The ESP32 connects to a Wi-Fi network and communicates with an MQTT broker to receive remote commands.

## 🎯 Objectives

- Build a two-channel Smart Home Controller using ESP32.
- Control two LEDs using two physical push buttons.
- Establish Wi-Fi connectivity with ESP32.
- Implement MQTT communication using a public MQTT broker.
- Receive remote ON/OFF commands through MQTT.
- Read button states using ESP32 GPIO input registers instead of `digitalRead()`.
- Implement button debouncing using `millis()`.
- Control LEDs using GPIO output pins.

## 🛠️ Components Required

- ESP32 Development Board
- 2 × Push Buttons
- 2 × LEDs
- 2 × 220Ω Resistors
- 1 × 10kΩ Pull-up Resistor
- Jumper Wires
- Breadboard

## 🔌 Pin Configuration

| Component | ESP32 Pin | GPIO |
| --- | --- | --- |
| Push Button 1 | D34 | GPIO 34 |
| Push Button 2 | D27 | GPIO 27 |
| LED 1 | D25 | GPIO 25 |
| LED 2 | D26 | GPIO 26 |

**Note:** GPIO 34 does not have an internal pull-up resistor. An external 10kΩ pull-up resistor is used for Push Button 1.

## ⚙️ Working Principle

1. The ESP32 connects to the Wi-Fi network.
2. The ESP32 connects to a public MQTT broker.
3. Two push buttons are configured as digital inputs.
4. Button states are read directly from ESP32 GPIO input registers.
5. When Push Button 1 is pressed, LED 1 toggles its state.
6. When Push Button 2 is pressed, LED 2 toggles its state.
7. MQTT messages are received through a subscribed topic.
8. Publishing `ON` turns both LEDs ON.
9. Publishing `OFF` turns both LEDs OFF.
10. A 50 ms debounce mechanism is implemented using `millis()`.

## 📡 MQTT Configuration

**MQTT Broker:** broker.hivemq.com

**MQTT Port:** 1883

**MQTT ClientID:** mukta_esp32_test01

**MQTT Topic:** `cse08/smarthome/command`

**Supported Commands:**

| MQTT Message | Expected Result |
| --- | --- |
| ON | Both LEDs ON |
| OFF | Both LEDs OFF |

An MQTT client, such as HiveMQ WebSocket Client, can be used to publish commands remotely.

## 📊 Expected Output

| Input | LED 1 | LED 2 |
| --- | --- | --- |
| Button 1 Press | Toggle | No Change |
| Button 2 Press | No Change | Toggle |
| MQTT `ON` | ON | ON |
| MQTT `OFF` | OFF | OFF |

## 💻 Code

The complete ESP32 Arduino source code is available in `sketch.ino`.

The code uses register-level GPIO input reading and MQTT communication through the PubSubClient library.

## 🔗 Simulation Links

- **Wokwi Simulation:** https://wokwi.com/projects/479211960344269825
- **MQTT Client:** https://www.hivemq.com/demos/websocket-client/
- **Simulation Video:** https://drive.google.com/file/d/1wjKq73c-OcqxF-7VIhQGYnGtJG4dM9Hf/view?usp=drive_link

## 📚 Registers and Functions Used

- `GPIO_IN_REG` – Reads GPIO input states for GPIO 0–31.
- `GPIO_IN1_REG` – Reads GPIO input states for GPIO 32–39.
- `REG_READ()` – Reads ESP32 hardware register values.
- `pinMode()` – Configures GPIO pin modes.
- `digitalWrite()` – Controls LED output states.
- `millis()` – Implements non-blocking button debouncing.
- `PubSubClient` – Handles MQTT communication.

## 👩‍💻 Project Information

**Platform:** ESP32

**Simulator:** Wokwi

**Programming Language:** C++

**Communication Protocol:** MQTT

**MQTT Broker:** HiveMQ Public Broker

**Project Type:** Embedded Systems / IoT / Smart Home Automation

---

This project was developed as part of an Embedded Systems / ESP32 MQTT and Register Manipulation task.
