# 🔌 Arduino Uno Pin Guide

Unlike ESP32/ESP8266, Uno's silkscreen labels **do match** what you use in code — pin `7` on the board is `7` in code. No Dx-to-GPIO translation needed here.

## 1. Digital Pins (0–13)

| Pin | Notes |
|---|---|
| 0 (RX), 1 (TX) | Serial / USB upload. Disconnect external wiring here before uploading, or upload will fail and Serial data will be garbled |
| 2, 3 | Only two pins with hardware **interrupt** support (`attachInterrupt`) |
| 3, 5, 6, 9, 10, 11 | PWM-capable (marked `~`) — use `analogWrite()` for dimming/speed control here |
| 5, 6 | PWM here runs at a different base frequency (~980Hz) than the other PWM pins (~490Hz) — usually invisible to the eye on an LED, but can matter for motor drivers or audio-adjacent uses that are sensitive to switching frequency |
| 9, 10 | Shared timer with the `Servo` library — using `Servo` can affect PWM behavior on these two pins |
| 10, 11, 12, 13 | Also the SPI pins (SS, MOSI, MISO, SCK) — avoid conflicts if using an SPI device (e.g. an SD card module, radio module) |
| 13 | Has the onboard LED + resistor already wired — fine to use, but the LED will blink along with your code |

## 2. Analog Pins (A0–A5)

| Pin | Notes |
|---|---|
| A0–A5 | Analog input only by default, 0–5V range, 10-bit resolution (`analogRead()` returns 0–1023) |
| A4, A5 | Also double as I2C SDA/SCL — used automatically by the `Wire` library |
| A0–A5 | Can also be used as extra digital pins if needed (`pinMode(A0, OUTPUT)` works) |

There is no analog *output* pin — "analog output" via `analogWrite()` on a PWM digital pin is actually a simulated average voltage (PWM), not a true DAC. See the main README's "Things Nobody Tells You" for what this means if you ever feed it into another device's analog input.

A single `analogRead()` call takes roughly **100 microseconds** on stock Uno — fast for one reading, but worth knowing if you're trying to sample many channels at a high, consistent rate (e.g. for audio-ish sampling), since it adds up and isn't instantaneous.

## 3. Floating Inputs — a common beginner bug

An input pin with nothing connected doesn't read a clean 0 — it "floats" and picks up electrical noise, giving random HIGH/LOW readings. For buttons or simple switches:

```cpp
pinMode(2, INPUT_PULLUP);  // enables Uno's internal pull-up resistor
// button reads LOW when pressed (if wired to GND), HIGH when not pressed
```

Using `INPUT_PULLUP` avoids needing an external resistor for simple button circuits. The internal pull-up resistor is roughly 20–50kΩ but isn't a precisely specified value — fine for a button, not something to rely on for exact current calculations.

## 4. Planning Your Pins

```text
Reserve 0/1 for Serial → Reserve 10-13 if using SPI →
Reserve A4/A5 if using I2C → Note which PWM pins your project needs →
Only then assign the rest to sensors/actuators
```

**Realistically free pins on a basic project** (no SPI, no I2C): 2, 3, 4, 5, 6, 7, 8, 9, 12, A0–A3.

## 5. Current limits — the numbers people don't look up until something smells hot

| What | Limit |
|---|---|
| Per digital I/O pin | 20 mA recommended max (40 mA absolute max, briefly) |
| Total across all I/O pins combined | ~200 mA for the whole chip |
| 5V pin (sourced from USB) | ~500 mA total, shared with the board itself |

A single LED with the right resistor is nowhere near these limits. Driving several LEDs directly from pins with no resistor, or trying to power a small motor straight off a digital pin "just to test it," is the usual way people find the limit the hard way — use a transistor, MOSFET, or a proper motor driver module instead of wiring a motor directly to a pin.
