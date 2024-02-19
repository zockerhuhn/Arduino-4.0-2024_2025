/** importiert Arduino automatisch, muss man also hier nicht unbedingt auch noch mal importieren: */
#include <Arduino.h>

/**
 * !!! Immer darauf achten, dass unten in der Statusleiste...
 *     ... das richtige Arduino Board eingestellt ist
 *     ... das richtige "Sketch File" ausgewählt ist (das ändert sich nämlich nicht automatisch)
 *     ... die richtige C/C++ Konfiguration eingestellt ist (sonst gibt es noch mehr "rote swiggels")
 * 
 * :: Externen RGB Farbsensor auslesen ::
 * :: Serial Plotter ausprobieren ::
 * :: Hauptprogramm-Schleife in Zustände unterteilen ::
 * :: Programm in Funktionen unterteilen ::
 * 
 * Hardware-Aufbau:
 * TCS34725:    Arduino Due / Arduino Nano RP2040 Connect:
 *     [LED <-- (kann man optional an irgendeinen Digital-Pin oder den INT Pin vom Sensor anschließen,
 *              um die LED abzuschalten)]
 *     [INT  --> (kann man optional an irgendeinen Digital-Pin anschließen,
 *                wenn man den Sensor effizienter auslesen will)]
 *      SDA <-> SDA
 *      SCL <-- SDA
 *     [3V3 --> (2,3V Spannungsversorgung, falls man sowas irgendwo benötigt)]
 *      GND <-- GND
 *      VIN <-- 3V3
 * 
 * Der VL53L0X ist ein externer Sensor, kann also entweder
 * - an   Bus I2C0 ("Wire")
 * - oder Bus I2C1 ("Wire1") verbunden werden.
*/

/** der I2C Bus */
#include <Wire.h>
/** I2C Adresse: 0x29 (7-bit) (unveränderlich) */
#include <Adafruit_TCS34725.h>
/** Sensor sehr schnell einstellen (ungenauer):
 *  Gain 4x fand ich am besten, aber dann sind die Werte so stabil,
 *   dass die Fehlerdetektion immer ausgelöst hat (siehe unten "helligkeitStatischStoppuhr.hasPassed"). */
Adafruit_TCS34725 rgbSensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
Adafruit_TCS34725 rgbSensor2 = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

enum Modus {
    /* Werte im Serial Monitor anzeigen. */
    WERTE_LOGGEN,
    WERTE_VERARBEITEN,
};

/** hier einstellen, was das Programm mit den Sensorwerten anfangen soll: */
Modus modus = WERTE_LOGGEN;

void setup() {
    Serial.begin(115200);
    // I2C Bus 1x für alle Bus-Teilnehmer initialisieren (sonst crasht das Betriebssystem)
    Wire.begin(); // Bus I2C0
    Wire.setClock(1000000); // 1MHz Kommunikationsgeschwindigkeit
    Wire1.begin();  // Bus I2C1
    // hier den zu nutzenden I2C Bus einstellen:
    if (!rgbSensor.begin(TCS34725_ADDRESS, &Wire)) {
        delay(10000); // damit wir Zeit haben den Serial Monitor zu öffnen nach dem Upload
        Serial.println("RGB Farbsensor Verdrahtung prüfen! Programm Ende.");
        while (1);
    }
    if (!rgbSensor2.begin(TCS34725_ADDRESS, &Wire1)) {
        delay(10000); // damit wir Zeit haben den Serial Monitor zu öffnen nach dem Upload
        Serial.println("RGB Farbsensor Verdrahtung prüfen! Programm Ende.");
        while (1);
    }
    Serial.println("Initialisierung abgeschlossen");
}

// hier speichern wir die Sensorwerte ab:
// Roh-Werte (Es gibt auch kalibierte Werte, aber die sind sehr langsam auszulesen):
uint16_t rot, gruen, blau, helligkeit;
uint16_t rot2, gruen2, blau2, helligkeit2;
int sensor1_verg[4] = {0,0,0,0};
int sensor2_verg[4] = {0,0,0,0};
int counter = 0;

void loop() {
    readColor();
    switch (modus) {
        case WERTE_LOGGEN:
            werteLoggen();
            calculatecolor();
            break;

        case WERTE_VERARBEITEN:
            addvalues();
            if (counter % 10 == 0) {
                werteausgeben();
            }
            break;
    }
}

const uint16_t VERBINDUNG_VERLOREN = 0;
uint16_t vorheriges_rot, vorheriges_gruen, vorheriges_blau, vorherige_helligkeit = VERBINDUNG_VERLOREN;

void readColor() {
    rgbSensor.getRawData(&rot, &gruen, &blau, &helligkeit);
    rgbSensor2.getRawData(&rot2, &gruen2, &blau2, &helligkeit2);
}

void werteLoggen() {
    Serial.println("R:" + String(rot) + " G:" + String(gruen) + " B:" + String(blau) + " C:" + String(helligkeit));
    Serial.println("R:" + String(rot2) + " G:" + String(gruen2) + " B:" + String(blau2) + " C:" + String(helligkeit2));
}

void calculatecolor() {
    if (((gruen >= blau) && (gruen >= rot)) && (gruen <= 1000) && (gruen >= 650))
    {
        Serial.println("green");
    }
    else {
        Serial.println("not green");
    }
    
}

void addvalues() {
    sensor1_verg[0] += rot;
    sensor1_verg[1] += gruen;
    sensor1_verg[2] += blau;
    sensor1_verg[3] += helligkeit;
    sensor2_verg[0] += rot2;
    sensor2_verg[1] += gruen2;
    sensor2_verg[2] += blau2;
    sensor2_verg[3] += helligkeit2;
    counter += 1;
}

void werteausgeben() {
    Serial.println("Sensor 1:");
    for (int i : sensor1_verg) {
        Serial.print(String(i/counter));
    }
    Serial.println("Sensor 2:");
    for (int i : sensor2_verg) {
        Serial.print(String(i/counter));
    }
}