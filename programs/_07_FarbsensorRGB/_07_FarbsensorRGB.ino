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

/** optional: Stoppuhr, um zu Verbindungsverluste zu erkennen */
#include <Chrono.h> 
Chrono helligkeitStatischStoppuhr = Chrono(Chrono::MILLIS, false); // noch nicht gestartet

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

    // hier den zu nutzenden I2C Bus einstellen:
    if (!rgbSensor.begin(TCS34725_ADDRESS, &Wire)) {
        delay(10000); // damit wir Zeit haben den Serial Monitor zu öffnen nach dem Upload
        Serial.println("RGB Farbsensor Verdrahtung prüfen! Programm Ende.");
        while (1);
    }

    helligkeitStatischStoppuhr.start();
    Serial.println("Initialisierung abgeschlossen");
}

// hier speichern wir die Sensorwerte ab:
// Roh-Werte (Es gibt auch kalibierte Werte, aber die sind sehr langsam auszulesen):
uint16_t rot, gruen, blau, helligkeit;

void loop() {
    readColor();

    switch (modus) {
        case WERTE_LOGGEN:
            werteLoggen();
            calculatecolor();
            break;

        case IM_SERIAL_PLOTTER_ZEIGEN:
            fuerPlotterLoggen();
            delay(100); // damit man im Plotter auch was erkennt und nicht alles so schnell vorbei fliegt
            break;

    }
}

const uint16_t VERBINDUNG_VERLOREN = 0;
uint16_t vorheriges_rot, vorheriges_gruen, vorheriges_blau, vorherige_helligkeit = VERBINDUNG_VERLOREN;

void readColor() {
    rgbSensor.getRawData(&rot, &gruen, &blau, &helligkeit);
    /** Dieser Mechanismus hier ist gefährlich, wenn es passieren kann, dass die Sensoren lange Zeit das selbe sehen:
     *  In meinen Versuchen habe ich oben den Gain von 4x auf 16x gestellt, um mehr Rauschen zu bekommen.
     *  Mit Timeout 5s sehe ich keine False-Negatives mehr: */
    if (!helligkeitStatischStoppuhr.hasPassed(5000)) {
        // alles OK
        if (vorheriges_rot != rot || vorheriges_gruen != gruen ||
            vorheriges_blau != blau || vorherige_helligkeit != helligkeit) {
            // merken: der Wert hat sich verändert
            vorheriges_rot = rot;
            vorheriges_gruen = gruen;
            vorheriges_blau = blau;
            vorherige_helligkeit = helligkeit;
            helligkeitStatischStoppuhr.restart();
        }
        return; // rausgehen aus der Funktion, damit wir nicht zum Fehler kommen
    }

    // Fehler:
    rot = VERBINDUNG_VERLOREN;
    gruen = VERBINDUNG_VERLOREN;
    blau = VERBINDUNG_VERLOREN;
    helligkeit = VERBINDUNG_VERLOREN;
    helligkeitStatischStoppuhr.restart(); // um bei Wackelkontakt Wiederverbindung zu erlauben
    Serial.println("RGB Sensor Verdrahtung prüfen!");
}

void werteLoggen() {
    Serial.println("R:" + String(rot) + " G:" + String(gruen) + " B:" + String(blau) + " C:" + String(helligkeit));
}

void fuerPlotterLoggen() {
    Serial.println(String(blau) + "," + String(rot) + "," + String(gruen) + "," +
                   String(helligkeit / 10)); // damit der Graph nicht die ganze Zeit raus und rein zoomt
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