# 🛠️ Arduino Uno Troubleshooting (Real Errors & Fixes)

## 1. "avrdude: stk500_getsync(): not in sync resp=0x00"

The most common Uno upload error. Causes, in order of likelihood:

1. Wrong **board** or **port** selected in Tools menu — double check both.
2. Something is wired to pins **0 (RX) / 1 (TX)** — disconnect any external circuit from these two pins, upload, then reconnect.
3. A CH340/CP2102 driver isn't installed (clone boards) — the port won't be recognized properly even if it shows up in the list.
4. Another program (a Serial Monitor from a different tool, a second Arduino IDE window) is holding the port open — close it first.
5. Faulty or charge-only USB cable — try a different one.
6. **Less common but real:** the bootloader itself is corrupted or missing (can happen after a botched upload interrupted mid-write, or a board that shipped with a bad flash). If the port is correctly detected and none of the above fix it, the board may need its bootloader re-burned with a separate USB-to-serial programmer or a second Arduino acting as an "Arduino as ISP" — a rare fix, but it's the actual next step if you've exhausted the common causes.

## 2. "Low memory available, stability problems may occur"

This is a real warning, not decoration — Uno has only 2KB of RAM total. Ignoring it often leads to bizarre, intermittent behavior (variables changing value on their own, `Serial.print` output getting garbled) rather than a clean crash.

**Fix:**
- Wrap fixed text in `F()`: `Serial.println(F("some text"));` — keeps it in flash memory instead of RAM.
- Avoid growing `String` objects in `loop()` — use `char` arrays or restructure to avoid concatenation.
- Remove unused library `#include`s — some pull in surprisingly large buffers even if you only use one function.
- If the symptom shows up only after the sketch has been running a while (not immediately at boot), it's very likely **heap fragmentation** from repeated `String` concatenation rather than a one-time over-allocation — the [`software.md`](software.md#4-memory-is-the-real-constraint-on-this-board) free-RAM snippet will show this as a slow downward drift instead of a sudden drop.

## 3. "Sketch too big" / exceeds the maximum program storage space

Uno has 32KB of flash total (minus ~0.5KB for the bootloader). There is no partition-scheme workaround like on ESP32/ESP8266.

**Fix:** remove unused libraries, avoid pulling in heavy libraries for a single function, or split the project — Uno is not the right board for large programs; consider an ESP32 if you're consistently hitting this.

## 4. Board resets when a motor/relay/servo turns on

**Cause:** the sudden current draw (and electrical noise) from the motor pulls down the 5V rail, or induces a voltage spike back into the board.

**Fix:**
- Power motors/relays/servos from a separate supply, not the Uno's 5V pin.
- Connect the external supply's GND to the Uno's GND (common ground) — required for signals to work correctly even with separate power.
- Add a flyback diode across relay coils and motors if you aren't using a driver module that already includes one.
- If the reset happens the instant you *plug in* the motor supply (not when it spins), suspect a brief short or reversed polarity during connection rather than the running current draw — check wiring before assuming it's a noise/decoupling problem.

## 5. Button/sensor reads randomly HIGH and LOW with nothing connected

Classic floating-pin symptom — see [`GPIO.md` §3](GPIO.md#3-floating-inputs--a-common-beginner-bug). Use `INPUT_PULLUP` (or an external pull-down/pull-up resistor) instead of leaving a digital input unconnected.

## 6. Serial Monitor shows garbled or scrambled text

The Serial Monitor's baud rate (bottom-right dropdown) doesn't match the value in `Serial.begin(...)` in your code. Match them exactly (commonly `9600` or `115200`).

If the baud rates *do* match and you still see occasional garbage characters (not full gibberish, just the odd corrupted line), check for something toggling pins 0/1 elsewhere in your circuit, or a marginal USB cable/hub — this is a different symptom from a baud mismatch and points at the physical connection instead.

## 7. EEPROM values look wrong or don't persist as expected

- EEPROM has a limited write endurance (~100,000 cycles per cell) — don't write to the same address in a tight loop; write only when a value actually changes. Prefer `EEPROM.update()` over `EEPROM.write()` — it's functionally the same (skips the write if the value hasn't changed) but makes that intent explicit in the code.
- A freshly-purchased or fully-erased chip may have `EEPROM.read()` return `255` (0xFF) at every address, not `0` — code should handle this as "no value saved yet," not treat it as valid data.
- **Uploading new code does *not* erase EEPROM** — it's a physically separate memory from flash and survives both re-uploads and power loss. If you need a clean slate while developing (e.g. testing example 04 or 05 from scratch), you have to clear it deliberately — example 04's `clear` command, or Arduino's built-in "Erase EEPROM" example sketch, or writing 0xFF across the used range.
- If you're storing anything larger than a single byte (like a `float` sensor reading or a `struct`), use `EEPROM.put()`/`EEPROM.get()` instead of `.write()`/`.read()` — they handle multi-byte values correctly and safely, where doing it by hand with single-byte writes is a common source of silently corrupted values.

## 8. Uploading works, but the sketch doesn't seem to run / pin doesn't respond

- Double-check pins used for SPI (10–13) or I2C (A4/A5) aren't double-booked with another part of your circuit — see [`GPIO.md` §1–2](GPIO.md).
- Confirm the component's power pin is actually connected — a surprisingly common wiring miss when breadboards get crowded.
- If it's a PIR sensor specifically (example 05), remember it needs **30–60 seconds after power-up to calibrate** — readings during that window are expected to be unreliable, not a wiring fault.
- If the sketch *used to work* and now doesn't after a fresh upload with no code changes, check **Tools → Port** — a genuine Uno's assigned port number can shift after a driver update or a different USB port, and the IDE will happily "successfully upload" to the wrong device or fail silently on a stale port reference.

## 9. `analogRead()` values are noisier than expected

- Switching rapidly between different analog pins can leak a little of the previous reading into the next one — discard the first read after switching pins and use the second (see the main README's "Things Nobody Tells You").
- Long, unshielded wires to an analog sensor pick up mains hum (50/60Hz) and other electrical noise easily, since the ADC input is high-impedance. A simple software average of a handful of consecutive readings fixes most of this without needing extra hardware.
- A motor, relay, or anything switching real current running off the *same* power supply as the analog sensor will inject noise into the readings even with a common ground — this is a good reason to power motors/relays separately (see [`hardware.md` §3](hardware.md#3-power)) beyond just the reset-risk concern.
