/**
 * Adafruit Farbsensor TCS34725 Testprogramm.
 * 
 * Dieses Testprogramm verwendet 2 Farbsensoren (2 Platinen).
 * Normalerweise wird der Farbsensor einfach mit den I2C-Pins
 * (SDA und SCL) verbunden. Die gibt es allerdings nur 1x am
 * Arduino und die Sensoradresse kann auch nicht geändert werden.
 * Für den 2. Sensor muss I2C also simuliert werden (geht mit jedem
 * Digitalpin).
 */

/** 1. Sensor: "echtes" (Hardware) I2C
 *  Anschlüsse:
 *  Sensor VIN -> Arduino 5V
 *  Sensor GND -> Arduino GND
 *  Sensor SDA -> Arduino SDA (z.B. Pin 20 auf dem Mega)
 *  Sensor SCL -> Arduino SCL (z.B. Pin 21 auf dem Mega)
 */
#include <Wire.h>
#include "Adafruit_TCS34725.h"
// tcs ist unser 2. Sensor
Adafruit_TCS34725 tcs2 = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

void setup() {
  Serial.begin(9600);
  Wire.begin();
  Serial.println("Adafruit Farbsensor Test!");
  if (tcs2.begin()) {
    // Initialisierung von Sensor 2 erfolgreich
    Serial.println("Sensor 2 gefunden!");
  } else {
    // Initialisierung von Sensor 2 fehlgeschlagen
    Serial.println("Sensor 2 nicht gefunden... Verdrahtung pruefen.");
  }
}


void loop() {
  unsigned int helligkeit2;
  unsigned int rot2;
  unsigned int gruen2;
  unsigned int blau2;

  // Sensor 2: lesen starten
  tcs2.setInterrupt(false);
  // nach 50 ms haben wir Werte; lieber 60ms warten, um sicher zu sein
  delay(60);
  // Werte in unseren Variablen speichern:
  tcs2.getRawData(&rot2, &gruen2, &blau2, &helligkeit2);
  // Sensor 2: lesen beenden
  tcs2.setInterrupt(true);
  // Werte ausgeben
  Serial.print(" HW: C:\t"); Serial.print(helligkeit2);
  Serial.print("\tR:\t"); Serial.print(rot2);
  Serial.print("\tG:\t"); Serial.print(gruen2);
  Serial.print("\tB:\t"); Serial.print(blau2);
  Serial.print("\n"); // Zeilenumbruch
}
