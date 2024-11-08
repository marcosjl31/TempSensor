#include <Arduino.h>

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN,OUTPUT);
  Serial.begin(115200);
  Serial.println("Setup done.");
}

void loop() {
  digitalWrite(LED_BUILTIN,LOW);
  Serial.println("Led is ON");
  delay(1000);
  digitalWrite(LED_BUILTIN,HIGH);
  Serial.println("Led is OFF");
  delay(1000);
}
