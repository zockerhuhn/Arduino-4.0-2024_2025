#pragma once

#include <Arduino.h>

// Bibliothek ("library") importieren
#include <SparkFun_VL53L5CX_Library.h>

// Sensor-Objekt erzeugen
SparkFun_VL53L5CX abstandsSensor = SparkFun_VL53L5CX();

// Einstellungen
const uint8_t NEUE_ADDRESSE = 0x35;

// Einstellungen anwenden
void initialisiereTof64Kanal() {
    Serial.println("Initialisierung des 64-Kanal ToF kann bis zu 10 Sekunden dauern...");
    if (!abstandsSensor.begin(NEUE_ADDRESSE, Wire)) {
        Serial.println("ToF64 Verdrahtung prüfen! Roboter aus- und einschalten! Programm Ende.");
        while (1);
    }
    if (!abstandsSensor.setResolution(64) ||
        !abstandsSensor.setRangingFrequency(15)) {
            Serial.println("ToF64 Auflösung oder Messfrequenz konnte nicht geändert werden! Programm Ende.");
            while (1);
    }
}

// Hier werden die Sensorwerte abgespeichert. Man kann sie im Hauptprogramm verwenden.
VL53L5CX_ResultsData messDaten;

// Sensorwerte aktualisieren (also den Sensor auslesen)
void leseTof64Kanal() {
    abstandsSensor.startRanging(); // Laser an
    while (!abstandsSensor.isDataReady()) {
        delay(10);
    }
    abstandsSensor.getRangingData(&messDaten);
    abstandsSensor.stopRanging(); // Laser aus
    delay(10);
}
