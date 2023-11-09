/** importiert Arduino automatisch, muss man also hier nicht unbedingt auch noch mal importieren: */
#include <Arduino.h>

/**
 * Der Versuch, alle Sensoren
 * - am Arduino Nano RP2040 Connect (!!! geht nicht mit Arduino Due !!!)
 * - am selben I2C Bus
 * - nur mit USB zu betreiben.
 * 
 * Da die Farbsensoren und die Laser-Sensoren ein einziger Lampenladen sind, reicht zumindest mit USB
 * der Strom beiweitem nicht aus. Da müsste ein eigener Spannungsregler her.
 * Als Hack werden die LEDs hier nur eingeschaltet, wenn man sie benötigt.
 * 
 * Hierfür müssen beim TCS34725 Pins LED und INT miteinander verbunden werden.
 * 
 * I2C Adressen:
 * LSM6DSOX 0x6A
 * ATECC608 0x60
 * AS7262   0x49
 * VL53L0X  0x29 (umprogrammiert auf 0x30)
 * VL53L5CX 0x29 (umprogrammiert auf 0x35)
 * TCS34725 0x29 (darf erst nach der Umprogrammierung der anderen Sensoren initialisiert werden)
*/

#include <Wire.h>

#include "onboard_6achsen_imu.h"
#include "spektrometer_farbsensor.h"
#include "rgb_farbsensor.h"
#include "tof_1kanal.h"
#include "tof_64kanal.h"
#include "helligkeitssensoren.h"

void setup() {
    Serial.begin(115200);

    delay(5000);

    Wire.begin();
    Wire.setClock(1000000);

    initialisiereHelligkeitssensoren();
    initialisiereImu(); // mit Adresse 0x6A
    initialisiereSpekrometer(); // mit Adresse 0x49
    // Reihenfolge ist hier wichtig, da am Anfang alle die Adresse 0x29 haben:
    initialisiereTof1Kanal(); // mit NEUE_ADDRESSE2 (0x30)
    initialisiereTof64Kanal(); // mit NEUE_ADDRESSE (0x35)
    initialisiereRgbSensor(); // mit Adresse 0x29

    Serial.println("Initialisierung abgeschlossen");
}

void loop() {
    leseImu();
    leseSpektrometer();
    leseRgbSensor();
    leseTof1Kanal();
    leseTof64Kanal();
    // leseHelligkeitssensoren();
    // delay(100);

    Serial.println(
        "Beschleunigung x=" + String(acc.x) + " y=" + String(acc.y) + " z=" + String(acc.z)
        + " Rotationsgeschwindigkeit x=" + String(gyro.x) + " y=" + String(gyro.y) + " z=" + String(gyro.z));
    Serial.println(
        "violett=" + String(violett) + " blau=" + String(blau) + " grün=" + String(gruen)
        + " gelb=" + String(gelb) + " orange=" + String(orange) + " rot=" + String(rot));
    Serial.println("R:" + String(rot2) + " G:" + String(gruen2) + " B:" + String(blau2) + " C:" + String(helligkeit2));
    Serial.println("Abstand: " + String(abstand));
    Serial.print("Reflektionsleiste: ");
    for (int i = 0; i < SENSOR_LEISTE_ANZAHL_SENSOREN; i++) {
        Serial.print(String(helligkeiten[i]) + '\t');
    }
    Serial.println();
    Serial.println("ToF64:");
    for (int y = 0; y <= 8 * (8 - 1); y += 8) {
            for (int x = 8 - 1; x >= 0; x--) {
                Serial.print("\t");
                Serial.print(messDaten.distance_mm[x + y]);
            }
            Serial.println();
    }
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
}
