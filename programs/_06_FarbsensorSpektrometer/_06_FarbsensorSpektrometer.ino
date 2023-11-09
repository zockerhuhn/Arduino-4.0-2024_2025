/** importiert Arduino automatisch, muss man also hier nicht unbedingt auch noch mal importieren: */
#include <Arduino.h>

/**
 * !!! Immer darauf achten, dass unten in der Statusleiste...
 *     ... das richtige Arduino Board eingestellt ist
 *     ... das richtige "Sketch File" ausgewählt ist (das ändert sich nämlich nicht automatisch)
 *     ... die richtige C/C++ Konfiguration eingestellt ist (sonst gibt es noch mehr "rote swiggels")
 * 
 * :: Externen Spektrometer-/Farbsensor auslesen ::
 * :: Serial Plotter ausprobieren ::
 * :: Hauptprogramm-Schleife in Zustände unterteilen ::
 * :: Programm in Funktionen unterteilen ::
 * 
 * Hardware-Aufbau:
 * AS7262 Sensor:    Arduino Nano RP2040 Connect / Arduino Due:
 *          3-6V <-- 3.3V
 *          SDA  <-> SDA
 *          SCL  <-- SCL
 *          [INT  --> (kann man optional an irgendeinen Digital-Pin anschließen,
 *                     wenn man den Sensor effizienter auslesen will)]
 *          GND  <-- GND
 * 
 * Der AS7262 ist ein externer Sensor, kann also entweder
 * - an   Bus I2C0 ("Wire")
 * - oder Bus I2C1 ("Wire1") verbunden werden.
 * 
 * Der AS7262 Sensor hängt am I2C Bus (SDA/SCL) mit 7-Bit Adresse 0x49.
 * Die Adresse kann nicht verändert werden. Man kann also keinen anderen Sensor mit derselben Adresse
 * an den selben Bus anschließen.
*/

/** der I2C Bus */
#include <Wire.h>
/** I2C Adresse: 0x49 (7-bit) (unveränderlich) */
#include <AS726X.h>
AS726X farbsensor = AS726X();

enum Modus {
    /* Werte im Serial Monitor anzeigen. */
    WERTE_LOGGEN,
    /* Geht nur in der offiziellen Arduino IDE: Komma-Separierte Liste für den Serial Plotter. */
    IM_SERIAL_PLOTTER_ZEIGEN,
};

/** hier einstellen, was das Programm mit den Sensorwerten anfangen soll: */
Modus modus = WERTE_LOGGEN;

void setup() {
    Serial.begin(115200);

    // I2C Bus 1x für alle Bus-Teilnehmer initialisieren (sonst crasht das Betriebssystem)
    Wire.begin(); // Bus I2C0
    Wire.setClock(1000000); // 1MHz Kommunikationsgeschwindigkeit
    // Wire1.begin();  // Bus I2C1

    if (!farbsensor.begin(Wire)) { // hier den zu nutzenden I2C Bus eintragen
        delay(10000); // damit wir Zeit haben den Serial Monitor zu öffnen nach dem Upload
        Serial.println("Spektrometer Farbsensor Verdrahtung prüfen! Programm Ende.");
        while (1);
    }
    if (farbsensor.getVersion() != SENSORTYPE_AS7262) {
        delay(10000); // damit wir Zeit haben den Serial Monitor zu öffnen nach dem Upload
        Serial.println("Falscher Sensor! Muss AS7262 sein!");
        while (1);
    }
    farbsensor.setMeasurementMode(CONTINUOUS_READ_ALL); // wann Soll der Sensor welche Farben messen? Brauchen wir wirklich alle Farben?
    farbsensor.setIntegrationTime(1); // groß (z.B. 50): genau, aber langsam | klein (z.B. 1): verrauscht, aber schnell
    // Die Lampe verbraucht viel Strom! Auf 12.5mA begrenzen, da sonst 3V3 einbricht und kein Sensor mehr funktioniert.
    farbsensor.setBulbCurrent(0);
    farbsensor.enableBulb(); // Lampe AN
    // farbsensor.disableBulb(); // Lampe AUS

    Serial.println("Initialisierung abgeschlossen");
}

// hier speichern wir die Sensorwerte ab:
float violett, blau, gruen, gelb, orange, rot;

void loop() {
    readColors();

    switch (modus) {

        case WERTE_LOGGEN:
            werteLoggen();
            break;

        case IM_SERIAL_PLOTTER_ZEIGEN:
            fuerPlotterLoggen();
            delay(100); // damit man im Plotter auch was erkennt und nicht alles so schnell vorbei fliegt
            break;

    }
}

const uint16_t VERBINDUNG_VERLOREN = 0;
void readColors() {
    if (farbsensor.dataAvailable()) {
        // Bei Verbindungsverlust werden alle Werte automatisch 0 (VERBINDUNG_VERLOREN):
        violett = farbsensor.getViolet();
        blau = farbsensor.getBlue();
        gruen = farbsensor.getGreen();
        gelb = farbsensor.getYellow();
        orange = farbsensor.getOrange();
        rot = farbsensor.getRed();

        if (violett == VERBINDUNG_VERLOREN && blau == VERBINDUNG_VERLOREN &&
            gruen == VERBINDUNG_VERLOREN && gelb == VERBINDUNG_VERLOREN &&
            orange == VERBINDUNG_VERLOREN && rot == VERBINDUNG_VERLOREN) {
            // Fehler:
            Serial.println("Spektrometer Verdrahtung prüfen!");
        }
    }
}

void werteLoggen() {
    Serial.println(
        "violett=" + String(violett) + " blau=" + String(blau) + " grün=" + String(gruen)
        + " gelb=" + String(gelb) + " orange=" + String(orange) + " rot=" + String(rot));
}

void fuerPlotterLoggen() {
    Serial.println(
        String(blau) + "," + String(rot) + "," + String(gruen) + ","
        + String(gelb) + "," + String(violett) + "," + String(orange));
}
