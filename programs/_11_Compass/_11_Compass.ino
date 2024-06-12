#include <Arduino.h>
#include <Wire.h>

#define ADDRESS 0xC0
uint16_t direction_16;
uint8_t direction_8;

void ReadDirection() {
  Wire.beginTransmission(ADDRESS);
  Wire.write(1);      // sets register pointer to echo #1 register (0x1E)
  Wire.endTransmission();      // stop transmitting

  Wire.requestFrom(ADDRESS, 1);
  byte x1 = Wire.read();
  // byte x2 = Wire.read();
  // byte x3 = Wire.read();
  // direction_16 = x3 | x2 << 8;
  direction_8 = x1;
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  Wire.setClock(1000000);
  Serial.println("setup complete");
}

void loop() {
  ReadDirection();
  // Serial.print("1: ");
  // Serial.println(direction_16);
  Serial.print("2: ");
  Serial.println(direction_8);
}