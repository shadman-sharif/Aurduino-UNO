/*
  Arduino Uno - Example 02
  Onboard LED Blink (pin 13)

  Note: pin 13 has the onboard LED wired in series with a resistor.
  Normal logic here (HIGH = on), unlike ESP8266's inverted D4 LED.
*/

#define LED_PIN 13

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  Serial.println(F("LED blink test started."));
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  Serial.println(F("LED ON"));
  delay(500);

  digitalWrite(LED_PIN, LOW);
  Serial.println(F("LED OFF"));
  delay(500);
}
