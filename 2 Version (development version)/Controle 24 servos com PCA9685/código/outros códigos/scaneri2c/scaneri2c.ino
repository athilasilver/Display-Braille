#include <Wire.h>

void setup() {
  Wire.begin();
  Serial.begin(9600);
  Serial.println("\nI2C Scanner");

  for (byte i = 0; i < 127; i++) {
    Wire.beginTransmission(i);
    if (Wire.endTransmission() == 0) {
      Serial.print("Dispositivo encontrado no endereço 0x");
      if (i < 16) Serial.print("0");
      Serial.println(i, HEX);
    }
  }
}

void loop() {
}
