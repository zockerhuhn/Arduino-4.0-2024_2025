// Motoren 1 und 2
#include "ZumoMotors.h"
ZumoMotors motoren;

#include <Wire.h> // Für Hardware-I2C

// Reflektionssensorleiste (Helligkeitssensoren)
#include "ZumoReflectanceSensorArray.h"
ZumoReflectanceSensorArray reflektionsSensoren;

/** Abstandssensor (Time of Flight (ToF))
 *  Anschlüsse:
 *  Sensor VIN -> Arduino 3.3V
 *  Sensor GND -> Arduino GND
 *  Sensor SDA -> Arduino SDA (z.B. Pin 20 SDA auf dem Mega)
 *  Sensor SCL -> Arduino SCL (z.B. Pin 21 SCL auf dem Mega)
 *  TODO: Auf Pins SDA1/SCL1 verschieben
 */
#include "VL53L0X.h"
VL53L0X tof = VL53L0X();

/** 1. Farbsensor:
 *  Anschlüsse:
 *  Sensor VIN -> Arduino 3.3V
 *  Sensor GND -> Arduino GND
 *  Sensor SDA -> Arduino SDA (z.B. Pin 20 SDA auf dem Mega)
 *  Sensor SCL -> Arduino SCL (z.B. Pin 21 SCL auf dem Mega)
 */
#include "Adafruit_TCS34725.h"
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

/** 2. Farbsensor:
 *  Anschlüsse:
 *  Sensor VIN -> Arduino 3.3V
 *  Sensor GND -> Arduino GND
 *  Sensor SDA -> Arduino SDA (z.B. Pin SDA1 auf dem Mega)
 *  Sensor SCL -> Arduino SCL (z.B. Pin SCL1 auf dem Mega)
 */
#include "Adafruit_TCS34725_1.h"
Adafruit_TCS34725_1 tcs2 = Adafruit_TCS34725_1(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

void setup()
{
  Serial.begin(9600);

  Serial.println("ALLES ZUSAMMEN TEST!");

  // Motor-Richtungen überall ändern
  motoren.flipLeftMotor(false);  // Drehrichtung linker Motor nicht umdrehen
  motoren.flipRightMotor(false); // Drehrichtung rechter Motor nicht umdrehen

  // Helligkeitssensoren initialisieren
  reflektionsSensoren.init();

  Wire.begin();  // SDA und SCL initialisieren
  Wire1.begin(); // SDA1 und SCL1 initialisieren

  // Adafruit Time-of-Flight Sensor initialisieren
  // Die Standardadresse ist 0x29, dieselbe wie beim Adafruit Farbsensor.
  //  tof.setAddress(0x30); // Adresse des Sensors ändern
  // if(tof.init(true)) {
  //   // Einstellung: Fehler, wenn der Sensor länger als 500ms lang nicht reagiert
  //   tof.setTimeout(500);
  //   // Reichweite vergrößern (macht den Sensor ungenauer)
  //   tof.setSignalRateLimit(0.1);
  //   tof.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  //   tof.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
  //   // lasse Sensor die ganze Zeit an
  //   tof.startContinuous();
  //   Serial.println("ToF Sensor gefunden!");
  // } else {
  //   Serial.println("ToF Sensor nicht gefunden... Verdrahtung pruefen.");
  // }

  Serial.println("Adafruit Farbsensor Test!");
  if (tcs.begin())
  {
    // Initialisierung von Sensor 1 erfolgreich
    Serial.println("Sensor 1 gefunden!");
  }
  else
  {
    // Initialisierung von Sensor 1 fehlgeschlagen
    Serial.println("Sensor 1 nicht gefunden... Verdrahtung pruefen.");
  }

  if (tcs2.begin())
  {
    // Initialisierung von Sensor 2 erfolgreich
    Serial.println("Sensor 2 gefunden!");
  }
  else
  {
    // Initialisierung von Sensor 2 fehlgeschlagen
    Serial.println("Sensor 2 nicht gefunden... Verdrahtung pruefen.");
  }
}

void loop()
{
  unsigned int reflektion[6]; // Helligkeiten vom Reflektionssensor

  uint16_t abstand = 0; // Abstand vom ToF Sensor

  uint16_t helligkeit; // Helligkeit vom Farbsensor 1
  uint16_t rot;        // Rot-Wert vom Farbsensor 1
  uint16_t gruen;      // Grün-Wert vom Farbsensor 1
  uint16_t blau;       // Blau-Wert vom Farbsensor 1

  uint16_t helligkeit2; // Helligkeit vom Farbsensor 2
  uint16_t rot2;        // Rot-Wert vom Farbsensor 2
  uint16_t gruen2;      // Grün-Wert vom Farbsensor 2
  uint16_t blau2;       // Blau-Wert vom Farbsensor 2

  // Helligkeitssensoren auslesen
  reflektionsSensoren.read(reflektion);
  // Werte ausgeben
  Serial.print(reflektion[0]); // Sensor links
  Serial.print("\t");          // Tab
  Serial.print(reflektion[1]); // Sensor fast links
  Serial.print("\t");          // Tab
  Serial.print(reflektion[2]); // Sensor links mittig
  Serial.print("\t");          // Tab
  Serial.print(reflektion[3]); // Sensor rechts mittig
  Serial.print("\t");          // Tab
  Serial.print(reflektion[4]); // Sensor fast rechts
  Serial.print("\t");          // Tab
  Serial.print(reflektion[5]); // Sensor rechts

  // Abstandssensor lesen starten
  // abstand = tof.readRangeContinuousMillimeters();
  // Serial.print("\tAbstand: \t"); Serial.print(abstand);

  // Farbsensor 1: lesen starten
  tcs.setInterrupt(false);
  // nach 50 ms haben wir Werte; lieber 60ms warten, um sicher zu sein
  delay(60);
  // Werte in unseren Variablen speichern:
  tcs.getRawData(&rot, &gruen, &blau, &helligkeit);
  // Sensor 1: lesen beenden
  tcs.setInterrupt(true);
  // Werte ausgeben
  Serial.print("\tSW: C:\t");
  Serial.print(helligkeit);
  Serial.print("\tR:\t");
  Serial.print(rot);
  Serial.print("\tG:\t");
  Serial.print(gruen);
  Serial.print("\tB:\t");
  Serial.print(blau);

  // Farbsensor 2: lesen starten
  tcs2.setInterrupt(false);
  // nach 50 ms haben wir Werte; lieber 60ms warten, um sicher zu sein
  delay(60);
  // Werte in unseren Variablen speichern:
  tcs2.getRawData(&rot2, &gruen2, &blau2, &helligkeit2);
  // Sensor 2: lesen beenden
  tcs2.setInterrupt(true);
  // Werte ausgeben
  Serial.print(" HW: C:\t");
  Serial.print(helligkeit2);
  Serial.print("\tR:\t");
  Serial.print(rot2);
  Serial.print("\tG:\t");
  Serial.print(gruen2);
  Serial.print("\tB:\t");
  Serial.print(blau2);

  Serial.print("\n"); // Zeilenumbruch

  // vorwärts
  // motoren.setLeftSpeed(400);   // anscheinend sind -400 bis +400 möglich?
  // motoren.setRightSpeed(400);  // Motorgeschwindigkeiten können einzeln gesetzt werden...
  // delay(2000);

  // rückwärts
  // motoren.setSpeeds(-400, -400); // ...oder beide gleichzeitig
  // delay(2000);
}
