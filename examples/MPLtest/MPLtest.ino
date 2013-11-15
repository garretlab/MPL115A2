#include <Wire.h>
#include <MPL115A2.h>

void setup () {
  Serial.begin(9600);
  MPL115A2.begin();
}

void loop () {
  Serial.println(MPL115A2.read());
  delay(1000);
}

