/*
  Arduino Uno - Example 01
  Basic Serial Test

  Purpose:
  - Check that the board boots and uploads correctly
  - Check Serial Monitor communication
*/

void setup() {
  Serial.begin(9600);
  delay(500);

  Serial.println();
  Serial.println(F("=========================="));
  Serial.println(F("      ARDUINO UNO TEST"));
  Serial.println(F("=========================="));
  Serial.println(F("Board started!"));
}

void loop() {
  Serial.println(F("Arduino Uno is running..."));
  delay(1000);
}
