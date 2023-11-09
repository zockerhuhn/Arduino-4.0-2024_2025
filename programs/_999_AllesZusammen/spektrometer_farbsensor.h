#pragma once

#include <Arduino.h>

// Bibliothek ("library") importieren
#include <AS726X.h>

// Sensor-Objekt erzeugen
AS726X farbsensor = AS726X();

// Einstellungen

// Einstellungen anwenden
void initialisiereSpekrometer() {
    if (!farbsensor.begin(Wire)) {
        Serial.println("Spektrometer Farbsensor Verdrahtung prüfen! Programm Ende.");
        while (1);
    }
    if (farbsensor.getVersion() != SENSORTYPE_AS7262) {
        Serial.println("Falscher Sensor! Muss AS7262 sein!");
        while (1);
    }
    farbsensor.setMeasurementMode(CONTINUOUS_READ_ALL);
    farbsensor.setIntegrationTime(1);
    // Die Lampe verbraucht viel Strom! Auf 12.5mA begrenzen, da sonst 3V3 einbricht und kein Sensor mehr funktioniert.
    farbsensor.setBulbCurrent(0);
    farbsensor.disableBulb(); // Lampe AUS
}

// Hier werden die Sensorwerte abgespeichert. Man kann sie im Hauptprogramm verwenden.
float violett, blau, gruen, gelb, orange, rot;

// Sensorwerte aktualisieren (also den Sensor auslesen)
void leseSpektrometer() {
    farbsensor.enableBulb();
    delay(10);
    if (farbsensor.dataAvailable()) {
        // Bei Verbindungsverlust werden alle Werte automatisch 0 (VERBINDUNG_VERLOREN):
        violett = farbsensor.getViolet();
        blau = farbsensor.getBlue();
        gruen = farbsensor.getGreen();
        gelb = farbsensor.getYellow();
        orange = farbsensor.getOrange();
        rot = farbsensor.getRed();
    }
    farbsensor.disableBulb();
    delay(10);
}
