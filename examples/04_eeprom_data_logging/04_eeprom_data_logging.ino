/*
  Arduino Uno - Example 04
  Data Logging to EEPROM

  Uno has no SD card slot and no filesystem like ESP's LittleFS.
  Instead, it has 1KB of built-in EEPROM that survives power loss -
  useful for small amounts of data like a counter, a saved setting,
  or a short log.

  What it does:
  - Every time you send anything in the Serial Monitor, it reads the
    current analog A0 value and stores it in the next EEPROM slot
  - Type "read" and press Enter to print everything stored so far
  - Type "clear" and press Enter to reset the stored count to zero

  Note: EEPROM has a limited write endurance (~100,000 writes per
  cell) - fine for occasional logging, not for writing every loop().
*/

#include <EEPROM.h>

#define COUNT_ADDRESS 0     // EEPROM address 0 stores how many entries we have
#define DATA_START_ADDRESS 1 // entries are stored starting at address 1
#define MAX_ENTRIES 100      // 1KB EEPROM, 1 byte per entry, minus the counter byte

void logReading() {
  int count = EEPROM.read(COUNT_ADDRESS);

  if (count == 255) count = 0; // freshly erased EEPROM reads as 255, treat as "no data yet"

  if (count >= MAX_ENTRIES) {
    Serial.println(F("EEPROM full - clear it first with 'clear'."));
    return;
  }

  int rawValue = analogRead(A0);
  byte scaledValue = map(rawValue, 0, 1023, 0, 255); // shrink to fit in 1 byte

  EEPROM.write(DATA_START_ADDRESS + count, scaledValue);
  EEPROM.write(COUNT_ADDRESS, count + 1);

  Serial.print(F("Logged entry #"));
  Serial.print(count + 1);
  Serial.print(F(": "));
  Serial.println(scaledValue);
}

void printLog() {
  int count = EEPROM.read(COUNT_ADDRESS);
  if (count == 255) count = 0;

  if (count == 0) {
    Serial.println(F("No entries yet - send anything first to log a reading."));
    return;
  }

  Serial.println(F("----- EEPROM log -----"));
  for (int i = 0; i < count; i++) {
    Serial.print(F("Entry "));
    Serial.print(i + 1);
    Serial.print(F(": "));
    Serial.println(EEPROM.read(DATA_START_ADDRESS + i));
  }
  Serial.println(F("----- end -----"));
}

void clearLog() {
  EEPROM.write(COUNT_ADDRESS, 0);
  Serial.println(F("Log cleared."));
}

void setup() {
  Serial.begin(9600);
  Serial.println(F("EEPROM logging demo started."));
  Serial.println(F("Type anything + Enter to log a reading."));
  Serial.println(F("Type 'read' + Enter to print the log."));
  Serial.println(F("Type 'clear' + Enter to erase the log."));
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    if (input == "read") {
      printLog();
    } else if (input == "clear") {
      clearLog();
    } else if (input.length() > 0) {
      logReading();
    }
  }
}
