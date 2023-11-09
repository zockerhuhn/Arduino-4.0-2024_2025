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

#include "ZumoReflectanceSensorArray.h"

ZumoReflectanceSensorArray reflektionsSensoren;

void setup() {
  // Seriellen Monitor initialisieren
  Serial.begin(9600); // 9600 ist die Übertragungsgeschwindigkeit in Bits/s

  // Library initialisieren
  reflektionsSensoren.init();
}

void loop() {
  delay(100);
  unsigned int sensorwerte[6];
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
