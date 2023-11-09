#pragma once

#include <Arduino.h>

// Bibliothek ("library") importieren
#include <Arduino_LSM6DSOX.h>
#include <quaternion_type.h>

// Einstellungen anwenden
void initialisiereImu() {
    if (!IMU.begin()) {
        Serial.println("IMU Verdrahtung prüfen! Programm Ende.");
        while (1);
    }
}

// Hier werden die Sensorwerte abgespeichert. Man kann sie im Hauptprogramm verwenden.
vec3_t acc = vec3_t(0, 0, 0); // (x, y, z)
vec3_t gyro = vec3_t(0, 0, 0); // (x, y, z)

// Sensorwerte aktualisieren (also den Sensor auslesen)
void leseImu() {
    if (IMU.accelerationAvailable()) {
        IMU.readAcceleration(acc.x, acc.y, acc.z);
    }
    if (IMU.gyroscopeAvailable()) {
        IMU.readGyroscope(gyro.x, gyro.y, gyro.z);
    }
}
