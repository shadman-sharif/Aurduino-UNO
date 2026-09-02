# 🔩 Arduino Uno Complete Guide

[![Board](https://img.shields.io/badge/Board-Arduino%20Uno-00979D?logo=arduino)](https://www.arduino.cc/)
[![Framework](https://img.shields.io/badge/Framework-Arduino-00979D?logo=arduino)](https://www.arduino.cc/)
[![License](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

A clean, beginner-friendly guide for learning the **Arduino Uno** from zero — hardware, setup, an analog sensor demo, logging data to onboard EEPROM, and a motion-triggered alert project. Includes the real, specific errors people run into that most tutorials skip.

> **Note:** Uno has **no Wi-Fi, no camera, and no SD card slot** — if you need networking or a camera project, that's an ESP32/ESP8266 board, not Uno. This guide covers what Uno is actually good at: fast, reliable, no-network sensor/actuator control. The "storage" example here uses the chip's built-in EEPROM instead of an SD card, and the "dashboard" example uses the Serial Plotter instead of a web page.

> **Target board:** All examples target the common **Arduino Uno R3** (genuine or clone). Other AVR boards (Nano, Mega, Leonardo) are similar but have different pin counts/layouts — check `docs/GPIO.md` before assuming a pin exists.

---

## 📚 What is Arduino Uno?

A simple, reliable microcontroller board built around the **ATmega328P** chip — no networking, no camera, just fast and predictable digital/analog I/O.

```text
   Sensors → Arduino Uno (ATmega328P) → Actuators (LEDs, motors, buzzers)
                    │
                    └──→ EEPROM (1KB) → small saved data/logs
                    └──→ Serial → live readings on your computer
```

---

## 🗂️ Repository Structure

```text
Arduino-Uno-Guide/
├── README.md
├── LICENSE
├── docs/
│   ├── hardware.md         Board parts, genuine vs clone, power (read first)
│   ├── software.md         Arduino IDE setup, the memory constraint that matters most
│   ├── GPIO.md             Pin map + which pins are safe to use
│   └── troubleshooting.md  Real error messages and their fixes
└── examples/
    ├── 01_serial_test/            Confirm the board boots
    ├── 02_led_blink/              Onboard LED (pin 13)
    ├── 03_analog_sensor_read/     Potentiometer -> PWM LED + Serial Plotter
    ├── 04_eeprom_data_logging/    Log small data to onboard EEPROM
    └── 05_pir_motion_alert/       PIR sensor triggers buzzer + LED + EEPROM count
```

---

## 🧰 What You Need

**For the basic examples (01–03):**
- Arduino Uno R3 (genuine or clone)
- USB cable (B-type for genuine, often Micro-USB for clones)
- A potentiometer (example 03)

**Extra for example 05:**
- HC-SR501 PIR motion sensor, a small buzzer, an LED + resistor

No Wi-Fi module, no SD card, and no camera needed for anything in this guide.

---

## 🚀 Quick Start

1. **Read [`docs/hardware.md`](docs/hardware.md)** first — especially genuine-vs-clone driver differences and the 5V logic level.
2. Set up Arduino IDE using [`docs/software.md`](docs/software.md) — Uno support is built in, but the **2KB RAM limit** is the thing that will bite you later if skipped.
3. Upload `examples/01_serial_test` — confirms the board and upload process work.
4. Upload `examples/02_led_blink` — confirms basic digital output.
5. Upload `examples/03_analog_sensor_read` — wire a potentiometer to A0, open **Tools → Serial Plotter** to see a live graph.
6. Try `examples/04_eeprom_data_logging` and `examples/05_pir_motion_alert` once the basics work.

If anything fails, check [`docs/troubleshooting.md`](docs/troubleshooting.md) — real error text (like `avrdude: stk500_getsync()`) and fixes, not just "check your wiring."

---

## 🔌 Pins — read this before wiring anything

Unlike ESP boards, Uno's printed pin numbers match the code directly — no label translation needed. But a few pins are reserved:

```text
Pins 0/1  -> Serial (disconnect before uploading)
Pins 10-13 -> also SPI, if you use an SPI device
Pins A4/A5 -> also I2C, if you use an I2C device
```

Full breakdown: [`docs/GPIO.md`](docs/GPIO.md).

---

## 🧩 Learning Path

```text
1. Serial Test → 2. LED Blink → 3. Analog Sensor + PWM →
4. Log Data to EEPROM → 5. Motion-Triggered Alert →
6. Add more sensors / build your own standalone project
```

---

## ⚠️ The One Thing That's Different About Uno

Every other board in this series of guides (ESP32-CAM, ESP8266 NodeMCU) has plenty of RAM and flash to be sloppy with. **Uno does not** — 2KB RAM total. Read `docs/software.md` §4 before your project grows past these five examples, or you'll hit mysterious crashes that don't show a clean error message.

---

## 📌 Next Project Ideas

**Beginner:** serial test → LED blink → read a sensor → log a value to EEPROM (all in this repo).

**Intermediate:** temperature/humidity display on an LCD, button-controlled relay, motion-triggered buzzer alarm (example 05 is a working starting point).

**Advanced:** multi-sensor data logger with a real-time clock module, a small robot with motor control, pairing with an ESP32 over Serial to add Wi-Fi to an Uno-based project.

---

## 📜 License

MIT — see [`LICENSE`](LICENSE).
