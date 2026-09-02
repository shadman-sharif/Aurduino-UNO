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
- **Check your actual free RAM instead of guessing.** The IDE's low-memory warning only fires at compile time based on *static* allocation — it can't see RAM you eat dynamically at runtime (like a growing `String`). Drop this classic function into a sketch and call it from `loop()` to print free RAM live over Serial:
  ```cpp
  extern "C" char* sbrk(int i);
  int freeRam() {
    char stackTop;
    return &stackTop - sbrk(0);
  }
  ```
  Watching this number drop over time (instead of holding steady) is the clearest sign of a memory leak — usually a `String` that keeps growing.
- **`sizeof()` a struct/array to sanity check EEPROM or RAM budgets** before you're surprised by a compile warning — e.g. `sizeof(int)` is 2 bytes on AVR (not 4, unlike most desktop platforms), which trips people up when porting code from other boards.
- Large constant data (lookup tables, string arrays) can also be forced into flash instead of RAM with `PROGMEM`, but reading it back requires special accessor functions (`pgm_read_byte()`, etc.) — worth knowing exists once `F()` alone isn't enough, though none of the examples in this repo need it.

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

> [!TIP]
> `Serial.begin()` **doesn't block** waiting for a Serial Monitor to actually be open — on Uno (unlike boards with native USB, e.g. Leonardo/Micro) the first few `Serial.println()` calls right after boot can be missed if you open the Monitor a moment too late, because the Uno's UART starts sending immediately rather than waiting for a listener. A short `delay(500)` before your first print (as in example 01) is a simple, common workaround — it's not required by the hardware, just a convenience for not missing the first lines.

## 6. Compile vs Upload

```text
Source Code → Compiler (avrdude toolchain) → Firmware (compile)
Firmware → USB → ATmega328P (upload)
```

Uno resets itself automatically when upload starts — no manual boot-mode jumper like ESP32-CAM's IO0.

> [!NOTE]
> This auto-reset works via a small capacitor on the DTR line that briefly pulls the chip's reset pin low when a new Serial connection opens. It's also why some other Serial programs (data loggers, plotting tools) that open the port cause the Uno to silently reboot every time they connect — the board isn't malfunctioning, that's the same mechanism the IDE uses to trigger uploads. If you don't want this behavior for a finished project, it's normally solved with a capacitor value change or a jumper, not a code fix.

## 7. Development loop

```text
Write → Compile → Upload → Run → Read Serial Monitor → Debug → Improve → Repeat
```

See [`troubleshooting.md`](troubleshooting.md) for specific error messages and fixes.
