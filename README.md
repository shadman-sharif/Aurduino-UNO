<div align="center">

# 🔩 Arduino Uno — Complete Guide

**A clean, beginner-to-intermediate guide to the Arduino Uno R3** — hardware, setup, an analog sensor demo, EEPROM data logging, and a motion-triggered alert project.
Includes the real, specific errors people hit that most tutorials skip, plus a handful of things that genuinely don't show up on the first page of Google.

[![Board](https://img.shields.io/badge/Board-Arduino%20Uno%20R3-00979D?logo=arduino&logoColor=white)](https://www.arduino.cc/)
[![Framework](https://img.shields.io/badge/Framework-Arduino-00979D?logo=arduino&logoColor=white)](https://www.arduino.cc/)
[![MCU](https://img.shields.io/badge/MCU-ATmega328P-A22846)](https://www.microchip.com/en-us/product/atmega328p)
[![License](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg)](#-contributing)

</div>

> [!NOTE]
> Uno has **no Wi-Fi, no camera, and no SD card slot.** If you need networking or a camera, that's an ESP32/ESP8266 project, not Uno. This guide sticks to what Uno is actually great at: fast, deterministic, no-network sensor/actuator control. The "storage" example uses the chip's built-in EEPROM instead of an SD card, and the "dashboard" example uses the Serial Plotter instead of a web page.

> [!TIP]
> **Target board:** all examples target a stock **Arduino Uno R3** (genuine or clone). Other AVR boards (Nano, Mega, Leonardo, Micro) are similar but have different pin counts, layouts, and in Leonardo/Micro's case a completely different USB stack — check [`docs/GPIO.md`](docs/GPIO.md) before assuming a pin or feature carries over.

---

## 📖 Table of Contents

- [What is Arduino Uno?](#-what-is-arduino-uno)
- [Repository Structure](#️-repository-structure)
- [What You Need](#-what-you-need)
- [Quick Start](#-quick-start)
- [Examples at a Glance](#-examples-at-a-glance)
- [Pins — Read This Before Wiring Anything](#-pins--read-this-before-wiring-anything)
- [Things Nobody Tells You](#-things-nobody-tells-you) — the section most guides skip
- [Learning Path](#-learning-path)
- [The One Thing That's Different About Uno](#️-the-one-thing-thats-different-about-uno)
- [Next Project Ideas](#-next-project-ideas)
- [Contributing](#-contributing)
- [License](#-license)

---

## 📚 What is Arduino Uno?

A simple, rock-solid microcontroller board built around the **ATmega328P** — no networking, no camera, just fast and predictable digital/analog I/O. It's the board most people learn embedded electronics on, and it's still the right choice for a huge share of real projects precisely *because* it doesn't try to do everything.

```text
   Sensors → Arduino Uno (ATmega328P @ 16MHz) → Actuators (LEDs, motors, buzzers)
                        │
                        ├──→ EEPROM (1KB)  → small saved data / logs, survives power loss
                        └──→ Serial (UART) → live readings on your computer
```

| | |
|---|---|
| **MCU** | ATmega328P, 16 MHz |
| **Flash (program storage)** | 32 KB (≈31.5 KB usable after bootloader) |
| **SRAM (variables at runtime)** | 2 KB |
| **EEPROM (persists across power cycles)** | 1 KB |
| **Digital I/O** | 14 pins (6 PWM-capable) |
| **Analog input** | 6 pins, 10-bit (0–1023) |
| **Logic level** | 5V |
| **Operating voltage / input voltage (barrel jack)** | 5V / 7–12V recommended |

---

## 🗂️ Repository Structure

```text
Arduino-Uno-Guide/
├── README.md
├── UNO file/
│   ├── hardware.md         Board parts, genuine vs clone, power (read first)
│   ├── software.md         Arduino IDE setup, the memory constraint that matters most
│   ├── GPIO.md             Pin map + which pins are actually safe to use
│   └── troubleshooting.md  Real error messages and their fixes
└── Demo codes/
    ├── 01_serial_test/            Confirm the board boots
    ├── 02_led_blink/               Onboard LED (pin 13)
    ├── 03_analog_sensor_read/      Potentiometer → PWM LED + Serial Plotter
    ├── 04_eeprom_data_logging/     Log small data to onboard EEPROM
    └── 05_pir_motion_alert/        PIR sensor triggers buzzer + LED + EEPROM count
```

---

## 🧰 What You Need

**For the basic examples (01–03):**
- Arduino Uno R3 (genuine or clone)
- USB cable — **B-type** for genuine boards, often **Micro-USB** for clones (and it must be a *data* cable, not charge-only)
- A potentiometer (example 03)

**Extra for example 05:**
- HC-SR501 PIR motion sensor, a small buzzer, an LED + resistor

No Wi-Fi module, no SD card, and no camera needed for anything in this guide.

---

## 🚀 Quick Start

1. **Read [`docs/hardware.md`](docs/hardware.md)** first — especially genuine-vs-clone driver differences and the 5V logic level.
2. Set up Arduino IDE using [`docs/software.md`](docs/software.md) — Uno support is built in, but the **2KB RAM limit** is the thing that will bite you later if you skip it now.
3. Upload [`examples/01_serial_test`](examples/01_serial_test) — confirms the board and upload process work.
4. Upload [`examples/02_led_blink`](examples/02_led_blink) — confirms basic digital output.
5. Upload [`examples/03_analog_sensor_read`](examples/03_analog_sensor_read) — wire a potentiometer to A0, open **Tools → Serial Plotter** to see a live graph.
6. Try [`examples/04_eeprom_data_logging`](examples/04_eeprom_data_logging) and [`examples/05_pir_motion_alert`](examples/05_pir_motion_alert) once the basics work.

If anything fails, check [`docs/troubleshooting.md`](docs/troubleshooting.md) — real error text (like `avrdude: stk500_getsync()`) and fixes, not just "check your wiring."

---

## 🧪 Examples at a Glance

| # | Sketch | Teaches | Hardware needed |
|---|---|---|---|
| 01 | [`serial_test`](examples/01_serial_test) | Serial Monitor basics, confirming a clean upload | None |
| 02 | [`led_blink`](examples/02_led_blink) | `digitalWrite()`, timing with `delay()` | None (onboard LED) |
| 03 | [`analog_sensor_read`](examples/03_analog_sensor_read) | `analogRead()`, `analogWrite()` (PWM), `map()`, Serial Plotter | Potentiometer, LED |
| 04 | [`eeprom_data_logging`](examples/04_eeprom_data_logging) | `EEPROM.read/write`, persisting data across power loss, Serial input parsing | None (uses A0 as source) |
| 05 | [`pir_motion_alert`](examples/05_pir_motion_alert) | Interrupt-free event detection, debouncing with `millis()`, `tone()`, combining sensors + EEPROM | HC-SR501 PIR, buzzer, LED |

---

## 🔌 Pins — read this before wiring anything

Unlike ESP boards, Uno's printed pin numbers match the code directly — no label translation needed. But a few pins are reserved:

```text
Pins 0/1   → Serial (disconnect before uploading)
Pins 10-13 → also SPI, if you use an SPI device
Pins A4/A5 → also I2C, if you use an I2C device
```

Full breakdown: [`docs/GPIO.md`](docs/GPIO.md).

---

## 🕵️ Things Nobody Tells You

Small, real gotchas that cost people hours — collected from actual failure reports, not repeated from other tutorials.

- **A "dead" Uno is often just a brown-out from a bad USB port.** Laptop USB hubs and some front-panel desktop ports can't reliably supply the inrush current when a sensor draws a spike. If a board resets the instant you touch a servo or a strip of LEDs but works fine standalone, it's the power path, not your code — try a different port or add a barrel-jack supply before you start debugging software.
- **`Serial.print()` of a `float` truncates to 2 decimal places by default**, silently — `Serial.println(3.14159)` prints `3.14`, not an error. Use `Serial.println(value, 4)` to get more digits; this bites people doing sensor calibration who assume their math is wrong.
- **The bootloader eats real space and real time you didn't budget for.** ~0.5KB of the 32KB flash is the bootloader, and it also adds roughly a **1–2 second reset delay** before your `setup()` runs after every power-up or upload — this is normal, not a hang, and it's why a "device not responding immediately" bug reproduces every single time.
- **`analogRead()` isn't instant and the ADC has memory.** Switching between analog pins too fast (reading A0 then A1 back-to-back with no settle time) can leak a bit of the previous pin's voltage into the new reading via the sample-and-hold capacitor. If you're chasing noisy readings across multiple analog pins, throw away the first read after switching pins and use the second.
- **`INPUT_PULLUP`'s internal resistor is ~20–50kΩ and varies chip to chip** — it's not a fixed, documented value like a resistor you'd buy. Fine for simple buttons; don't rely on it for anything where the exact pull-up value matters (like some I2C bus configurations).
- **`millis()` overflows after ~49.7 days** of continuous uptime, wrapping back to 0. Code that does `if (millis() - lastTime > interval)` handles this correctly automatically (unsigned arithmetic wraps safely); code that does `if (millis() > lastTime + interval)` does **not** and will glitch once every 49 days — a bug that's nearly impossible to reproduce on a bench test.
- **Never call `delay()`, `Serial.print()`, or anything that itself waits, inside an interrupt handler (ISR).** ISRs on AVR run with interrupts disabled by default, so anything that depends on `millis()`/`micros()` advancing, or that blocks, will hang the whole board rather than just misbehave. Set a flag in the ISR and act on it in `loop()` instead.
- **A brand-new/erased EEPROM cell reads `255` (`0xFF`), not `0`.** Both example 04 and 05 in this repo handle this already, but it's the single most common cause of "my counter starts at some huge random number" reports — code that assumes a fresh chip reads `0` is checking the wrong sentinel value.
- **`EEPROM.write()` only writes a byte if the value is actually changing** (the library checks internally), but calling `EEPROM.update()` explicitly is the documented, intention-revealing way to do the same thing — prefer it over `.write()` when you're not sure the value changed, since it makes the write-endurance protection obvious to the next reader of your code.
- **A genuine Uno's serial port can silently reappear as a *different* COM/tty number** after a driver update or a different USB port — if uploads suddenly fail with a port timeout after nothing in your code changed, re-check **Tools → Port** before touching anything else.
- **PWM "analog output" isn't a smooth voltage** — it's a fast on/off square wave (~490Hz on most pins, ~980Hz on pins 5 & 6) whose *average* looks like a voltage to an LED or your eye, but will read as a switching signal, not a clean DC level, on an oscilloscope or a sensitive analog input. This matters if you ever try to feed a PWM pin into another board's analog input expecting a real DAC.

---

## 🧩 Learning Path

```text
1. Serial Test → 2. LED Blink → 3. Analog Sensor + PWM →
4. Log Data to EEPROM → 5. Motion-Triggered Alert →
6. Add more sensors / build your own standalone project
```

---

## ⚠️ The One Thing That's Different About Uno

Every other board in this series of guides (ESP32-CAM, ESP8266 NodeMCU) has plenty of RAM and flash to be sloppy with. **Uno does not** — 2KB RAM total. Read [`docs/software.md` §4](docs/software.md#4-memory-is-the-real-constraint-on-this-board) before your project grows past these five examples, or you'll hit mysterious crashes that don't show a clean error message.

---

## 📌 Next Project Ideas

**Beginner:** serial test → LED blink → read a sensor → log a value to EEPROM (all in this repo).

**Intermediate:** temperature/humidity display on an LCD, button-controlled relay, motion-triggered buzzer alarm (example 05 is a working starting point).

**Advanced:** multi-sensor data logger with a real-time clock module, a small robot with motor control, pairing with an ESP32 over Serial to add Wi-Fi to an Uno-based project.

---

## 🤝 Contributing

Found a real error message this guide doesn't cover, or a gotcha that cost you an evening? PRs and issues are welcome — the goal of this repo is to stay closer to "what actually happens" than to a generic tutorial.

---

## 📜 License

MIT — see [`LICENSE`](LICENSE).
