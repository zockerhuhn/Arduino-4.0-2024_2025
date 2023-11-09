#pragma once

#include <Arduino.h>

// Bibliothek ("library") importieren
#include <VL53L0X.h>

// Sensor-Objekt erzeugen
VL53L0X abstandsSensor2 = VL53L0X();

// Einstellungen
const uint8_t NEUE_ADDRESSE2 = 0x30;

// Einstellungen anwenden
void initialisiereTof1Kanal() {
    abstandsSensor2.setBus(&Wire);
    abstandsSensor2.setAddress(NEUE_ADDRESSE2);
    if (!abstandsSensor2.init()) {
        Serial.println("ToF Verdrahtung prüfen! Roboter aus- und einschalten! Programm Ende.");
        while (1);
    }
    abstandsSensor2.setTimeout(500);
    abstandsSensor2.setSignalRateLimit(0.1);
    abstandsSensor2.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
    abstandsSensor2.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
}

// Hier werden die Sensorwerte abgespeichert. Man kann sie im Hauptprogramm verwenden.
uint16_t abstand = 0;

// Sensorwerte aktualisieren (also den Sensor auslesen)
void leseTof1Kanal() {
    abstand = abstandsSensor2.readRangeSingleMillimeters();
}
