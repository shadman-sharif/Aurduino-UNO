# 🛠️ Arduino Uno Troubleshooting (Real Errors & Fixes)

## 1. "avrdude: stk500_getsync(): not in sync resp=0x00"

The most common Uno upload error. Causes, in order of likelihood:

1. Wrong **board** or **port** selected in Tools menu — double check both.
2. Something is wired to pins **0 (RX) / 1 (TX)** — disconnect any external circuit from these two pins, upload, then reconnect.
3. A CH340/CP2102 driver isn't installed (clone boards) — the port won't be recognized properly even if it shows up in the list.
4. Another program (a Serial Monitor from a different tool, a second Arduino IDE window) is holding the port open — close it first.
5. Faulty or charge-only USB cable — try a different one.

## 2. "Low memory available, stability problems may occur"

This is a real warning, not decoration — Uno has only 2KB of RAM total. Ignoring it often leads to bizarre, intermittent behavior (variables changing value on their own, `Serial.print` output getting garbled) rather than a clean crash.

**Fix:**
- Wrap fixed text in `F()`: `Serial.println(F("some text"));` — keeps it in flash memory instead of RAM.
- Avoid growing `String` objects in `loop()` — use `char` arrays or restructure to avoid concatenation.
- Remove unused library `#include`s — some pull in surprisingly large buffers even if you only use one function.

## 3. "Sketch too big" / exceeds the maximum program storage space

Uno has 32KB of flash total (minus ~0.5KB for the bootloader). There is no partition-scheme workaround like on ESP32/ESP8266.

**Fix:** remove unused libraries, avoid pulling in heavy libraries for a single function, or split the project — Uno is not the right board for large programs; consider an ESP32 if you're consistently hitting this.

## 4. Board resets when a motor/relay/servo turns on

**Cause:** the sudden current draw (and electrical noise) from the motor pulls down the 5V rail, or induces a voltage spike back into the board.

**Fix:**
- Power motors/relays/servos from a separate supply, not the Uno's 5V pin.
- Connect the external supply's GND to the Uno's GND (common ground) — required for signals to work correctly even with separate power.
- Add a flyback diode across relay coils and motors if you aren't using a driver module that already includes one.

## 5. Button/sensor reads randomly HIGH and LOW with nothing connected

Classic floating-pin symptom — see `docs/GPIO.md` §3. Use `INPUT_PULLUP` (or an external pull-down/pull-up resistor) instead of leaving a digital input unconnected.

## 6. Serial Monitor shows garbled or scrambled text

The Serial Monitor's baud rate (bottom-right dropdown) doesn't match the value in `Serial.begin(...)` in your code. Match them exactly (commonly `9600` or `115200`).

## 7. EEPROM values look wrong or don't persist as expected

- EEPROM has a limited write endurance (~100,000 cycles per cell) — don't write to the same address in a tight loop; write only when a value actually changes.
- A freshly-purchased or fully-erased chip may have `EEPROM.read()` return `255` (0xFF) at every address, not `0` — code should handle this as "no value saved yet," not treat it as valid data.

## 8. Uploading works, but the sketch doesn't seem to run / pin doesn't respond

- Double-check pins used for SPI (10–13) or I2C (A4/A5) aren't double-booked with another part of your circuit — see `docs/GPIO.md` §1–2.
- Confirm the component's power pin is actually connected — a surprisingly common wiring miss when breadboards get crowded.
