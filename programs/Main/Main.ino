/*
 * ====================================================
 *  Zumobot Testprogramm #6b - Zumo Reflektionssensoren
 * ====================================================
 *
 * Liest die Roh-Werte der Reflektionssensoren und gibt sie über den Serial Monitor aus.
 *
 * Neue Inhalte:
 * ============
 * - ZumoReflectanceSensorArray Library
 * - Arrays
 * - Zeiger
 *
 * Hardware:
 * =========
 * Benötigt nur den Zumobot; keine externe Schaltung.
 * PWM-Pins sind auf dem Arduino markiert mit einer Tilde (~).
 *
 * Referenzen:
 * ===========
 * - immer auf unserer Lieblingswebseite nachschauen (leider nur auf englisch verfügbar):
 *   https://www.arduino.cc/en/Reference/HomePage
 * - deutsche Übersetzung:
 *   https://www.arduinoforum.de/arduino_referenz_down.php
 * - und Toms Cheat Sheet nicht zu vergessen
 * - ZumoShield Libraries
 *   https://github.com/pololu/zumo-shield/blob/master/README.textile
 */
/** 1. Farbsensor:
 *  Anschlüsse:
 *  Sensor VIN -> Arduino 3.3V
 *  Sensor GND -> Arduino GND
 *  Sensor SDA -> Arduino SDA (z.B. Pin 20 SDA auf dem Mega)
 *  Sensor SCL -> Arduino SCL (z.B. Pin 21 SCL auf dem Mega)
 */
/** 2. Farbsensor:
 *  Anschlüsse:
 *  Sensor VIN -> Arduino 3.3V
 *  Sensor GND -> Arduino GND
 *  Sensor SDA -> Arduino SDA (z.B. Pin SDA1 auf dem Mega)
 *  Sensor SCL -> Arduino SCL (z.B. Pin SCL1 auf dem Mega)
 */

#include "ZumoReflectanceSensorArray.h"
#include "ZumoMotors.h"
#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include "Adafruit_TCS34725_1.h"

ZumoMotors motoren;
ZumoReflectanceSensorArray reflektionsSensoren;
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
Adafruit_TCS34725_1 tcs2 = Adafruit_TCS34725_1(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

int varrechts = 0;
int varlinks = 0;
uint16_t helligkeit2;
uint16_t rot2;
uint16_t gruen2;
uint16_t blau2;
uint16_t helligkeit; // Helligkeit vom Farbsensor 1
uint16_t rot;        // Rot-Wert vom Farbsensor 1
uint16_t gruen;      // Grün-Wert vom Farbsensor 1
uint16_t blau;       // Blau-Wert vom Farbsensor 1
unsigned int sensorwerte[6];

void straight()
{
  motoren.flipLeftMotor(true);
  motoren.flipRightMotor(true);
  motoren.setSpeeds(75, 75);
}

void left()
{
  motoren.flipLeftMotor(false);
  motoren.flipRightMotor(true);
  motoren.setSpeeds(75, 75);
}

void right()
{
  motoren.flipLeftMotor(true);
  motoren.flipRightMotor(false);
  motoren.setSpeeds(75, 75);
}

void turn()
{
  delay(1); // placeholder
}

void setup()
{
  // Seriellen Monitor initialisieren
  Serial.begin(9600); // 9600 ist die Übertragungsgeschwindigkeit in Bits/s
  Wire.begin();
  Wire1.begin();
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
  // Library initialisieren
  reflektionsSensoren.init();
  motoren.flipLeftMotor(true);
  motoren.flipRightMotor(true);
}

void read_reflectionandprint()
{
  reflektionsSensoren.read(sensorwerte);
  Serial.print(sensorwerte[0]); // Sensor links
  Serial.print("\t");           // Tab
  Serial.print(sensorwerte[1]); // Sensor fast links
  Serial.print("\t");           // Tab
  Serial.print(sensorwerte[2]); // Sensor links mittig
  Serial.print("\t");           // Tab
  Serial.print(sensorwerte[3]); // Sensor rechts mittig
  Serial.print("\t");           // Tab
  Serial.print(sensorwerte[4]); // Sensor fast rechts
  Serial.print("\t");           // Tab
  Serial.print(sensorwerte[5]); // Sensor rechts
  Serial.print("\n");           // Neue Zeile
}

void read_color2andprint()
{
  // Sensor 2: lesen starten
  tcs2.setInterrupt(false);
  // nach 50 ms haben wir Werte; lieber 60ms warten, um sicher zu sein
  delay(60);
  // Werte in unseren Variablen speichern:
  tcs2.getRawData(&rot2, &gruen2, &blau2, &helligkeit2);
  // Sensor 2: lesen beenden
  tcs2.setInterrupt(true);
  Serial.print(" HW: C:\t");
  Serial.print(helligkeit2);
  Serial.print("\tR:\t");
  Serial.print(rot2);
  Serial.print("\tG:\t");
  Serial.print(gruen2);
  Serial.print("\tB:\t");
  Serial.print(blau2);
  Serial.print("\n"); // Zeilenumbruch
}

void read_color1andprint()
{
  // Sensor 2: lesen starten
  tcs.setInterrupt(false);
  // nach 50 ms haben wir Werte; lieber 60ms warten, um sicher zu sein
  delay(60);
  // Werte in unseren Variablen speichern:
  tcs.getRawData(&rot, &gruen, &blau, &helligkeit);
  // Sensor 2: lesen beenden
  tcs.setInterrupt(true);
  Serial.print(" HW: C:\t");
  Serial.print(helligkeit);
  Serial.print("\tR:\t");
  Serial.print(rot);
  Serial.print("\tG:\t");
  Serial.print(gruen);
  Serial.print("\tB:\t");
  Serial.print(blau);
  Serial.print("\n"); // Zeilenumbruch
}

void doppelschwarz()
{
  Serial.print("\n");
  Serial.print("alles-schwarz");
  motoren.setSpeeds(0, 0);
  delay(1000);
  read_color2andprint();
  read_color1andprint();
  if (rot2 <= 1000 && (gruen2 - blau2 >= 200))
  {
    if (rot <= 1000 && (gruen - blau >= 200))
    {
      turn();
    }
    else
    {
      straight();
      delay(1250);
      right();
      delay(1000);
      reflektionsSensoren.read(sensorwerte);
      while (sensorwerte[5] <= 120)
      {
        reflektionsSensoren.read(sensorwerte);
      }
    }
  }
  else
  {
    if (rot <= 1000 && (gruen - blau >= 200))
    {
      straight();
      delay(1250);
      left();
      delay(1000);
      reflektionsSensoren.read(sensorwerte);
      while (sensorwerte[1] <= 120)
      {
        reflektionsSensoren.read(sensorwerte);
      }
    }
    else
    {
      straight();
      delay(1200);
      motoren.setSpeeds(0, 0);
      reflektionsSensoren.read(sensorwerte);
      if (sensorwerte[0] >= 120 || sensorwerte[1] >= 120 || sensorwerte[2] >= 120 || sensorwerte[3] >= 120 || sensorwerte[4] >= 120 || sensorwerte[5] >= 120)
      {
        // not else lol
      }
      else
      {
        left();
        delay(2500);
        right();
        reflektionsSensoren.read(sensorwerte);
        while (sensorwerte[1] <= 120 && sensorwerte[2] <= 120 && sensorwerte[3] <= 120 && sensorwerte[4] <= 120)
        {
          reflektionsSensoren.read(sensorwerte);
          Serial.print("\n");
          Serial.print("suche...");
        }
      }
    }
  }
}

void loop()
{
  read_reflectionandprint();
  if ((sensorwerte[0] >= 120) && (sensorwerte[5] >= 120))
  {
    doppelschwarz();
  }
  else if ((sensorwerte[2] >= 120 || sensorwerte[3] >= 120) && (sensorwerte[0] <= 120 && sensorwerte[1] <= 120 && sensorwerte[4] <= 120 && sensorwerte[5] <= 120))
  {
    Serial.print("\n");
    Serial.print("Linie");
    straight();
    if (varrechts > 0)
    {
      varrechts = varrechts - 5;
    }
    if (varlinks > 0)
    {
      varlinks = varlinks - 5;
    }
  }
  else if (sensorwerte[0] >= 120 || sensorwerte[1] >= 120)
  {
    Serial.print("\n");
    Serial.print("links");
    left();
    varlinks = varlinks + 5;
    if (varlinks + varrechts >= 400)
    {
      varlinks = 0;
      varrechts = 0;
      doppelschwarz();
    }
  }
  else if (sensorwerte[4] >= 120 || sensorwerte[5] >= 120)
  {
    Serial.print("\n");
    Serial.print("rechts");
    right();
    varrechts = varrechts + 5;
    if (varlinks + varrechts >= 400)
    {
      varlinks = 0;
      varrechts = 0;
      doppelschwarz();
    }
  }
  else if (sensorwerte[0] <= 120 && sensorwerte[1] <= 120 && sensorwerte[2] <= 120 && sensorwerte[3] <= 120 && sensorwerte[4] <= 120 && sensorwerte[5] <= 120)
  {
    Serial.print("\n");
    Serial.print("keine Linie...");
    straight();
  }
  delay(50);
}