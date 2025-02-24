/** importiert Arduino automatisch, muss man also hier nicht unbedingt auch noch mal importieren: */
#include <Arduino.h>

/**
 * !!! Immer darauf achten, dass unten in der Statusleiste...
 *     ... das richtige Arduino Board eingestellt ist
 *     ... das richtige "Sketch File" ausgewählt ist (das ändert sich nämlich nicht automatisch)
 *     ... die richtige C/C++ Konfiguration eingestellt ist (sonst gibt es noch mehr "rote swiggels")
 * 
 * :: Externen 64-Kanal Time of Flight Abstandssensor auslesen ::
 * :: mehrere Variablen zusammen in einen Block (struct) verpacken ::
 * :: Serial Plotter ausprobieren ::
 * :: Hauptprogramm-Schleife in Zustände unterteilen ::
 * :: Programm in Funktionen unterteilen ::
 * 
 * Hardware-Aufbau:
 * VL53L5CX:    Arduino Due / Arduino Nano RP2040 Connect:
 *     [RST <-- (kann man optional an irgendeinen Digital-Pin anschließen,
 *               um den Sensor programmatisch abzuschalten,
 *               während man die I2C Adresse eines anderen Sensors auf demselben I2C Bus ändert.)]
 *     [INT --> (kann man optional an irgendeinen Digital-Pin anschließen,
 *               wenn man den Sensor effizienter auslesen will)]
 *      SCL <-- SDA
 *      SDA <-> SDA
 *      3V3 <-- 3V3
 *      GND <-- GND
 * 
 * Der VL53L5CX ist ein externer Sensor, kann also entweder
 * - an   Bus I2C0 ("Wire")
 * - oder Bus I2C1 ("Wire1") verbunden werden.
*/

/** der I2C Bus */
#include <Wire.h>
/** I2C Adresse: 0x29 (7-bit) (Das Beispiel programmiert die Adresse auf 0x35 um) */
#include <SparkFun_VL53L5CX_Library.h>
SparkFun_VL53L5CX abstandsSensor = SparkFun_VL53L5CX();
const uint8_t NEUE_ADDRESSE = 0x35;

typedef struct Einstellungen {
    uint8_t aufloesung;
    uint8_t bildSeitenlaenge;
    uint8_t maxMessfrequenz;
} Einstellungen_t;
// Bei meinem Sensor habe ich hier ein paar "kaputte Pixel":
const Einstellungen ACHT_MAL_ACHT = { VL53L5CX_RESOLUTION_8X8, 8, 15 }; // 8x8: max 15Hz
// Bei meinem Sensor sieht hier alles gut aus:
const Einstellungen VIER_MAL_VIER = { VL53L5CX_RESOLUTION_4X4, 4, 60 }; // 4x4: max 60Hz
Einstellungen einstellungen = ACHT_MAL_ACHT;

/** optional: Stoppuhr, um zu Verbindungsverluste zu erkennen */
#include <Chrono.h> 
Chrono keineNeuenDatenStoppuhr = Chrono(Chrono::MILLIS, false); // noch nicht gestartet

enum Modus {
    /* Werte im Serial Monitor anzeigen. */
    ABSTANDS_WERTE_LOGGEN,
    /* Sensor gehen eine Fläche richten (z.B. Monitor) und in weniger als SCHWELLENWERT mm etwas davor halten.  */
    SCHWELLENWERT_VISUALISIERUNG,
};
const int SCHWELLENWERT = 100;

/** hier einstellen, was das Programm mit den Sensorwerten anfangen soll: */
Modus modus = SCHWELLENWERT_VISUALISIERUNG;

void setup() {
    Serial.begin(115200);

    delay(5000); // weil es sein kann, dass wir sonst den ToF nicht erkennen und den Roboter nochmal aus- und anschalten müssen

    // I2C Bus 1x für alle Bus-Teilnehmer initialisieren (sonst crasht das Betriebssystem)
    Wire.begin(); // Bus I2C0
    Wire.setClock(1000000); // 1MHz Kommunikationsgeschwindigkeit
    //Wire1.begin();  // Bus I2C1

    Serial.println("Initialisierung des 64-Kanal ToF kann bis zu 10 Sekunden dauern...");
    // hier den zu nutzenden I2C Bus und die zu nutzende I2C Adresse eintragen:
    if (!abstandsSensor.begin(NEUE_ADDRESSE, Wire)) {
        delay(10000); // damit wir Zeit haben den Serial Monitor zu öffnen nach dem Upload
        Serial.println("ToF64 Verdrahtung prüfen! Roboter aus- und einschalten! Programm Ende.");
        while (1) ;
    }
    if (!abstandsSensor.setResolution(einstellungen.aufloesung) ||
        !abstandsSensor.setRangingFrequency(einstellungen.maxMessfrequenz)) {  // siehe oben
            delay(10000); // damit wir Zeit haben den Serial Monitor zu öffnen nach dem Upload
            Serial.println("ToF64 Auflösung oder Messfrequenz konnte nicht geändert werden! Programm Ende.");
            while (1);
    }

    abstandsSensor.startRanging();
    keineNeuenDatenStoppuhr.start();
    Serial.println("Initialisierung abgeschlossen");
}

// hier speichern wir die 6 Sensorwerte ab:
VL53L5CX_ResultsData messDaten;

void loop() {
    readDistances();
    abstandsWerteLoggen();
    delay(100); // damit man auch was erkennt im Serial Monitor
}

const uint16_t VERBINDUNG_VERLOREN = 0;
uint16_t vorheriger_abstand = VERBINDUNG_VERLOREN;
void readDistances() {
    if (abstandsSensor.isDataReady()) {
        // diese Zeile speicher bereits die Daten ab:
        if (abstandsSensor.getRangingData(&messDaten)) {
            // alles OK
            keineNeuenDatenStoppuhr.restart();
            return;  // rausgehen aus der Funktion, damit wir nicht zum Fehler kommen
        } // else: Fehler
    } // else: wenn es keine neuen Daten gibt, müssen wir sie auch nicht lesen

    /* können wir bei Fehlern nutzen, sonst isses unnötig */
    // Fehler:
    if (keineNeuenDatenStoppuhr.hasPassed(1000)) {
        Serial.println("ToF64 Verdrahtung prüfen! Roboter aus- und einschalten!");
    } 
}

/** Kopiert aus dem Beispiel der Bibliothek: */
void abstandsWerteLoggen() {
    //The ST library returns the data transposed from zone mapping shown in datasheet
    //Pretty-print data with increasing y, decreasing x to reflect reality
    for (int y = 0;
         y <= einstellungen.bildSeitenlaenge * (einstellungen.bildSeitenlaenge - 1);
         y += einstellungen.bildSeitenlaenge) {
            for (int x = einstellungen.bildSeitenlaenge - 1; x >= 0; x--) {
                Serial.print("\t");
                uint8_t pixelStatus = messDaten.target_status[x + y];
                uint8_t pixelAbstandMm = messDaten.distance_mm[x + y];
                // 5 und 8 OK, ansonsten kann man dem Wert nicht vertrauen:
                if (pixelStatus == 5 || pixelStatus == 8) {
                    if (modus == SCHWELLENWERT_VISUALISIERUNG) {
                        if (pixelAbstandMm < SCHWELLENWERT) {
                            Serial.print('#');
                        } else {
                            Serial.print('.');
                        }
                    } else {
                        Serial.print(pixelAbstandMm);
                    }
                } else {
                    Serial.print('.');
                }
            }
            Serial.println();
    }
    Serial.println();
}
