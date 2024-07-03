#include <Arduino.h>
#include <Wire.h>

#define ADDRESS 0x60
uint16_t direction_16;
uint8_t direction_8;

float Yaw;

void ReadDirection() {
  Wire.beginTransmission(ADDRESS);
  Wire.write(0X01);
  Wire.endTransmission(false);      // stop transmitting

  Wire.requestFrom(ADDRESS, 3);
  direction_8 = Wire.read();
  Yaw=(int16_t)(Wire.read()<<8|Wire.read())/10.00; //Two bytes Yaw in range of (0 to 359 degrees) 
  direction_16 = Yaw;
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  Wire.setClock(1000000);
  Serial.println("setup complete");
}

void loop() {
  ReadDirection();
  Serial.print("1: ");
  Serial.println(direction_16);
  // Serial.print("2: ");
  // Serial.println(direction_8);
}