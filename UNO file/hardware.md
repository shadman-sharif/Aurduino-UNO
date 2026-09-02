# 🔧 Arduino Uno Hardware Guide

## 1. Main Parts

```text
┌───────────────────────────────────┐
│            Arduino Uno             │
│                                     │
│   ATmega328P (main chip, 16MHz)    │
│   USB-to-serial chip (ATmega16U2   │
│     on genuine boards, CH340 on    │
│     most clones)                   │
│   5V voltage regulator             │
│   Onboard LED on pin 13            │
│   Digital pins 0-13, Analog A0-A5  │
│   Barrel jack (7-12V input)        │
└───────────────────────────────────┘
```

- **ATmega328P** — the actual microcontroller. No Wi-Fi, no Bluetooth, no camera support built in — this is a plain microcontroller board, not a networked one. If you need Wi-Fi, that's an ESP32/ESP8266 project, not Uno.
- **Very limited memory:** 32KB flash (program storage) and only **2KB of RAM**. This matters a lot more here than on ESP boards — see [`troubleshooting.md`](troubleshooting.md) for the "low memory" warning.
- **Logic level is 5V**, unlike ESP32/ESP8266 which are 3.3V. This means Uno is generally safer to wire directly to common 5V sensor modules, but you **cannot** connect a 3.3V-only sensor's output straight to an Uno input without a level shifter — it can read unreliably or, in some cases, damage the sensor.
- **No onboard storage beyond EEPROM** (1KB) — no SD slot, no filesystem like ESP's LittleFS. Example 04 in this repo uses the built-in EEPROM for small amounts of saved data.
- **Onboard 16MHz ceramic resonator, not a crystal oscillator.** It's why Uno is a couple of dollars cheaper than boards using a true crystal, and also why its clock accuracy is a bit looser (±0.5% typical) — irrelevant for blinking LEDs, but worth knowing if you're building something that needs precise timing (like generating audio sample rates or talking to a device with tight UART timing tolerances at high baud rates).

## 2. Genuine vs Clone boards

| Type | USB chip | Notes |
|---|---|---|
| **Genuine Arduino Uno R3** | ATmega16U2 | Usually auto-detected without installing a driver on modern OSes |
| **Common clones** | CH340 or CP2102 | Need a separate USB driver installed on Windows; usually fine out of the box on Linux/macOS |

If your board isn't recognized in Arduino IDE's port list, this is almost always a missing CH340/CP2102 driver, not a broken board.

> [!TIP]
> **A quick way to tell genuine vs. clone without opening the IDE:** look at the chip next to the USB port. A small chip labeled `16U2` (or the older `8U2`) is genuine-style; a chip labeled `CH340`, `CH340G`, or `CP2102`/`CP2104` is a clone's USB bridge. This also predicts driver behavior before you plug anything in.

## 3. Power

- **USB power** (5V from your computer) is enough for the board itself plus a handful of low-current sensors/LEDs. A standard USB 2.0 port is rated for **500mA** — that's a real ceiling, not a formality, once you add more than a couple of LEDs or a small servo.
- **Barrel jack (7–12V)** — use this if you're driving motors, several LEDs, or anything current-hungry. Anything much above 12V can overheat the onboard regulator. The regulator wastes the difference between input voltage and 5V as heat, so a 12V supply running anything current-hungry gets noticeably warmer than a 7–9V one — this is expected, not a fault, but keep it ventilated.
- Do **not** power motors, relays, or servos directly from the Uno's 5V pin — their current draw and electrical noise can reset or damage the board. Use a separate power supply for these, with a shared/common ground to the Uno.
- **The Uno auto-switches between USB and barrel-jack power** — you can leave USB connected for Serial/uploading while also running the barrel jack for a hungrier load, and it won't fight itself. It'll prefer whichever source is higher voltage.
- **A brown-out (board resets or behaves erratically under load) can look exactly like a software bug.** If a project only crashes when a motor spins up or several LEDs turn on together, suspect the power rail before the code — see the "Things Nobody Tells You" section in the main README.

## 4. USB Connection

Plug in a USB-B (genuine) or Micro-USB (many clones) cable.

- If not detected: install the CH340 or CP2102 driver depending on the chip printed near the USB port.
- Try a different cable — many are charge-only with no data lines. If a cable charges your phone but the Uno never shows up as a port no matter what drivers you install, this is the first thing to suspect, not the last.

## 5. Pins that are "special" — read before wiring

| Pin | Special use |
|---|---|
| 0 (RX), 1 (TX) | Used by Serial / USB upload. Anything wired here can block uploading or corrupt Serial communication — disconnect external wiring on these two pins before uploading code |
| 13 | Has the onboard LED wired in series with a resistor. Still usable as a normal I/O pin, but expect the LED to flicker along with whatever you do on pin 13 |
| A4, A5 | Default I2C (SDA/SCL) pins for sensors like OLED displays, some IMUs |
| 3, 5, 6, 9, 10, 11 | PWM-capable (marked with `~` on the board) — use these for dimming LEDs or controlling motor speed |
| AREF | An often-overlooked pin — lets you feed a precise external reference voltage to the ADC instead of the default 5V rail, which improves `analogRead()` resolution/accuracy for sensors that only swing over a small voltage range. Rarely needed for beginner projects, but worth knowing it exists once you outgrow the default. |

## 6. Hardware Checklist

- [ ] Confirmed genuine vs clone (driver needed or not)
- [ ] Correct USB driver installed if using a clone
- [ ] Using a data-capable USB cable
- [ ] Nothing wired to pins 0/1 while uploading
- [ ] Motors/relays/servos powered separately, not from the 5V pin, with a shared ground
- [ ] Any 3.3V-only sensor uses a level shifter, not a direct connection
- [ ] If powering current-hungry loads from the barrel jack, regulator has some airflow
