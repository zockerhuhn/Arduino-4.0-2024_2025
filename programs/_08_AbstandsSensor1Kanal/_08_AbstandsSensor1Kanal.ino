/** importiert Arduino automatisch, muss man also hier nicht unbedingt auch noch mal importieren: */
#include <Arduino.h>

/**
 * !!! Immer darauf achten, dass unten in der Statusleiste...
 *     ... das richtige Arduino Board eingestellt ist
 *     ... das richtige "Sketch File" ausgewählt ist (das ändert sich nämlich nicht automatisch)
 *     ... die richtige C/C++ Konfiguration eingestellt ist (sonst gibt es noch mehr "rote swiggels")
 * 
 * :: Externen 1-Kanal Time of Flight Abstandssensor auslesen ::
 * :: Serial Plotter ausprobieren ::
 * :: Hauptprogramm-Schleife in Zustände unterteilen ::
 * :: Programm in Funktionen unterteilen ::
 * 
 * Hardware-Aufbau:
 * VL53L0X:    Arduino Due / Arduino Nano RP2040 Connect:
 *    SDA  <-> SDA
 *    SCL  <-- SDA
 *   [SHDN <-- (kann man optional an irgendeinen Digital-Pin anschließen,
 *              um den Sensor programmatisch abzuschalten,
 *              während man die I2C Adresse eines anderen Sensors auf demselben I2C Bus ändert.)]
 *   [GPIO --> (kann man optional an irgendeinen Digital-Pin anschließen,
 *              wenn man sich vom Sensor benachrichtigen lassen möchten,
 *              z.B. wenn der Abstand unter eine bestimme Schwelle absinkt)]
 *    GND  <-- GND
 *   [2v8  --> (2,8V Spannungsversorgung, falls man sowas irgendwo benötigt)]
 *    VIN  <-- 3V3
 * 
 * Der VL53L0X ist ein externer Sensor, kann also entweder
 * - an   Bus I2C0 ("Wire")
 * - oder Bus I2C1 ("Wire1") verbunden werden.
*/

/** der I2C Bus */
#include <Wire.h>
/** I2C Adresse: 0x29 (7-bit) (Das Beispiel programmiert die Adresse auf 0x30 um) */
#include <VL53L0X.h>
VL53L0X abstandsSensor = VL53L0X();
const uint8_t NEUE_ADDRESSE = 0x30;

/** optional: Stoppuhr, um zu Verbindungsverluste zu erkennen */
#include <Chrono.h> 
Chrono abstandStatischStoppuhr = Chrono(Chrono::MILLIS, false); // noch nicht gestartet

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
    //Wire1.begin();  // Bus I2C1

    // hier den zu nutzenden I2C Bus und die zu nutzende I2C Adresse eintragen:
    abstandsSensor.setBus(&Wire);
    abstandsSensor.setAddress(NEUE_ADDRESSE);
    if (!abstandsSensor.init()) {
        delay(10000); // damit wir Zeit haben den Serial Monitor zu öffnen nach dem Upload
        Serial.println("ToF Verdrahtung prüfen! Roboter aus- und einschalten! Programm Ende.");
        while (1);
    }
    // Einstellung: Fehler, wenn der Sensor länger als 500ms lang nicht reagiert
    abstandsSensor.setTimeout(500);
    // Reichweiter vergrößern (macht den Sensor ungenauer)
    abstandsSensor.setSignalRateLimit(0.1);
    abstandsSensor.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
    abstandsSensor.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
    // lasse Sensor die ganze Zeit an
    abstandsSensor.startContinuous();

    abstandStatischStoppuhr.start();
    Serial.println("Initialisierung abgeschlossen");
}

// hier speichern wir die 6 Sensorwerte ab:
uint16_t abstand = 0;

void loop() {
    readDistance();

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
uint16_t vorheriger_abstand = VERBINDUNG_VERLOREN;
void readDistance() {
    if (!abstandsSensor.timeoutOccurred()) {
        abstand = abstandsSensor.readRangeContinuousMillimeters();
        // statt 65535 kann es auch passieren, dass sich der Wert einfach nicht mehr ändert
        if (abstand != 65535 && !abstandStatischStoppuhr.hasPassed(1000)) {
            // alles OK
            if (vorheriger_abstand != abstand) {
                // merken: der Wert hat sich verändert
                vorheriger_abstand = abstand;
                abstandStatischStoppuhr.restart();
            }
            return; // rausgehen aus der Funktion, damit wir nicht zum Fehler kommen
        }
    }

    // Fehler:
    abstand = VERBINDUNG_VERLOREN;
    abstandStatischStoppuhr.restart(); // um bei Wackelkontakt Wiederverbindung zu erlauben
    Serial.println("ToF Verdrahtung prüfen! Roboter aus- und einschalten!");
}

void werteLoggen() {
    Serial.println("Abstand: " + String(abstand));
}

void fuerPlotterLoggen() {
    Serial.println(abstand);
}
