# 💻 Arduino Uno Software Guide

## 1. Arduino IDE Setup

Unlike ESP32/ESP8266, Uno support is **built into Arduino IDE by default** — no board manager URL to add.

```text
Install Arduino IDE (2.x)
       ↓
Tools → Board → Arduino AVR Boards → "Arduino Uno"
       ↓
Select the correct serial port
       ↓
Upload → Open Serial Monitor at chosen baud rate (commonly 9600)
```

## 2. Driver notes

- Genuine boards (ATmega16U2 chip) usually work with no driver install on Windows 10/11, macOS, and Linux.
- Clone boards with a **CH340** or **CP2102** chip need a driver installed on Windows — search "CH340 driver" for your OS if the port doesn't show up.

## 3. Libraries you'll actually need

```cpp
#include <EEPROM.h>  // built-in, for example 04 (data logging)
```

No Wi-Fi, camera, or filesystem libraries apply here — Uno doesn't have that hardware. If a tutorial tells you to `#include <WiFi.h>` on an Uno, it's either wrong for this board or assumes an add-on Wi-Fi shield, which is a different setup entirely.

## 4. Memory is the real constraint on this board

This is the single biggest difference from ESP32/ESP8266 projects: **32KB flash, 2KB RAM total.**

- The IDE will warn you: `Low memory available, stability problems may occur.` Take this seriously — on Uno, running low on RAM causes silent, hard-to-debug crashes (not a clean error message).
- Avoid `String` concatenation in loops (`myString += "x"`) — it fragments the tiny RAM. Prefer fixed char arrays or `F()` macro for constant text: `Serial.println(F("Hello"));` stores the string in flash instead of RAM.
- If you get "Sketch too big" at compile time, you've run out of the 32KB program space — trim unused libraries/features rather than trying to change a setting (there's no partition scheme trick like on ESP32).

## 5. Serial Monitor basics

```cpp
void setup() {
  Serial.begin(9600);
  Serial.println("Arduino Uno started!");
}

void loop() {
  Serial.println("Running...");
  delay(1000);
}
```

Set the Serial Monitor's baud rate dropdown to match `Serial.begin(...)` exactly, or you'll see garbled text.

## 6. Compile vs Upload

```text
Source Code → Compiler (avrdude toolchain) → Firmware (compile)
Firmware → USB → ATmega328P (upload)
```

Uno resets itself automatically when upload starts — no manual boot-mode jumper like ESP32-CAM's IO0.

## 7. Development loop

```text
Write → Compile → Upload → Run → Read Serial Monitor → Debug → Improve → Repeat
```

See `docs/troubleshooting.md` for specific error messages and fixes.
