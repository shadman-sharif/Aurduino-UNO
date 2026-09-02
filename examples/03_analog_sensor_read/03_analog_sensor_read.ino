/*
  Arduino Uno - Example 03
  Analog Sensor Read -> PWM LED Control

  What it does:
  - Reads a potentiometer (or LDR/any analog sensor) on A0
  - Maps the 0-1023 reading to a 0-255 PWM brightness
  - Fades an LED on pin 9 to match
  - Prints the raw and mapped values to Serial (open Serial Plotter
    to see it as a live graph, not just numbers)

  Wiring:
    Potentiometer middle pin -> A0
    Potentiometer outer pins -> 5V and GND
    LED (+ resistor) -> pin 9 (PWM-capable) -> GND

  This is the Uno equivalent of a "live dashboard" - since Uno has
  no Wi-Fi, the "dashboard" is the Serial Plotter instead of a web page.
*/

#define SENSOR_PIN A0
#define LED_PIN 9  // must be a PWM (~) pin

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  Serial.println(F("Analog sensor + PWM LED demo started."));
  Serial.println(F("Open Tools -> Serial Plotter to see a live graph."));
}

void loop() {
  int rawValue = analogRead(SENSOR_PIN);           // 0-1023
  int brightness = map(rawValue, 0, 1023, 0, 255);  // 0-255

  analogWrite(LED_PIN, brightness);

  Serial.print(F("raw:"));
  Serial.print(rawValue);
  Serial.print(F(",brightness:"));
  Serial.println(brightness);

  delay(50);
}
