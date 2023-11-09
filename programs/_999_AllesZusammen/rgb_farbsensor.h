#pragma once

#include <Arduino.h>

// Bibliothek ("library") importieren
#include <Adafruit_TCS34725.h>

// Einstellungen
// Sensor-Objekt erzeugen
Adafruit_TCS34725 rgbSensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_16X);

// Einstellungen anwenden
void initialisiereRgbSensor() {
    if (!rgbSensor.begin(TCS34725_ADDRESS, &Wire)) {
        Serial.println("RGB Farbsensor Verdrahtung prüfen! Programm Ende.");
        while (1);
    }
    rgbSensor.setInterrupt(true); // LED aus
}

// Hier werden die Sensorwerte abgespeichert. Man kann sie im Hauptprogramm verwenden.
uint16_t rot2, gruen2, blau2, helligkeit2;

// Sensorwerte aktualisieren (also den Sensor auslesen)
void leseRgbSensor() {
    rgbSensor.setInterrupt(false); // LED an
    delay(10);
    rgbSensor.getRawData(&rot2, &gruen2, &blau2, &helligkeit2);
    rgbSensor.setInterrupt(true); // LED aus
    delay(10);
}
