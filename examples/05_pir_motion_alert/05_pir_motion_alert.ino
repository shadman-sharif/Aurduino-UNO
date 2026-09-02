/*
  Arduino Uno - Example 05
  Motion-Triggered Alert (PIR sensor -> buzzer + LED + EEPROM counter)

  Uno has no Wi-Fi and no camera, so this project reacts locally:
  sounds a buzzer, lights an LED, and keeps a running count of motion
  events in EEPROM (survives power loss).

  Wiring:
    PIR (HC-SR501) VCC -> 5V
    PIR (HC-SR501) GND -> GND
    PIR (HC-SR501) OUT -> pin 2
    Buzzer (+)          -> pin 8
    Buzzer (-)          -> GND
    Alert LED (+ resistor) -> pin 7 -> GND

  Before uploading:
  - Give the PIR sensor 30-60 seconds after power-up to calibrate
    before trusting its readings (this is normal for HC-SR501).
*/

#include <EEPROM.h>

#define PIR_PIN 2
#define BUZZER_PIN 8
#define LED_PIN 7
#define COOLDOWN_MS 5000  // minimum time between two counted events
#define COUNT_ADDRESS 0

unsigned long lastEventTime = 0;

int readCount() {
  int count = EEPROM.read(COUNT_ADDRESS);
  return (count == 255) ? 0 : count; // freshly erased EEPROM = no data yet
}

void triggerAlert() {
  int count = readCount();
  count++;
  if (count < 255) {
    EEPROM.write(COUNT_ADDRESS, count);
  }

  Serial.print(F("Motion detected! Total events: "));
  Serial.println(count);

  digitalWrite(LED_PIN, HIGH);
  tone(BUZZER_PIN, 1000, 300); // 1kHz beep for 300ms
  delay(300);
  digitalWrite(LED_PIN, LOW);
}

void setup() {
  Serial.begin(9600);

  pinMode(PIR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  Serial.println(F("Motion alert demo started."));
  Serial.print(F("Events logged so far: "));
  Serial.println(readCount());
  Serial.println(F("Calibrating PIR sensor, please wait..."));
  delay(30000); // let the PIR sensor settle
  Serial.println(F("Watching for motion..."));
}

void loop() {
  bool motion = digitalRead(PIR_PIN) == HIGH;
  unsigned long now = millis();

  if (motion && (now - lastEventTime > COOLDOWN_MS)) {
    lastEventTime = now;
    triggerAlert();
  }
}
