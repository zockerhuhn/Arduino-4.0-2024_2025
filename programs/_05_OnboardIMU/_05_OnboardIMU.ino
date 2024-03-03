/** importiert Arduino automatisch, muss man also hier nicht unbedingt auch noch mal importieren: */
#include <Arduino.h>

/**
 * !!! Immer darauf achten, dass unten in der Statusleiste...
 *     ... das richtige Arduino Board eingestellt ist
 *     ... das richtige "Sketch File" ausgewählt ist (das ändert sich nämlich nicht automatisch)
 *     ... die richtige C/C++ Konfiguration eingestellt ist (sonst gibt es noch mehr "rote swiggels")
 * 
 * :: Onboard-IMU Sensor auslesen (3-Achsen Beschleunigung + 3-Achsen Gyro) ::
 * :: Serial Plotter ausprobieren ::
 * :: Was ist Sensor-Fusion? ::
 * :: Hauptprogramm-Schleife in Zustände unterteilen ::
 * :: Programm in Funktionen unterteilen ::
 * 
 * Hardware-Aufbau:
 * Arduino Nano RP2040 Connect (!! Arduino Due geht nicht !!)
 * <nichts weiter>
 * Der LSM6DSOX ist immer an Bus I2C0 ("Wire") verbunden. Das kann nicht geändert werden.
 * 
 * Der LSM6DSOX Sensor hängt am I2C Bus (SDA/SCL) mit 7-Bit Adresse 0x6A.
 * Die Adresse kann nicht verändert werden. Man kann also keinen anderen Sensor mit derselben Adresse
 * an den selben Bus anschließen.
*/

/** der I2C Bus */
#include <Wire.h>
/** I2C Adresse: 0x6A (7-bit) (unveränderlich) */
#include <Arduino_LSM6DSOX.h>

#include <imuFilter.h>
imuFilter Fusion = imuFilter(); // Objekt, dass Sensordaten fusionieren kann

#include <WiFiNINA.h>

enum Modus {
    /* Werte im Serial Monitor anzeigen. */
    WERTE_LOGGEN,
    /* Geht nur in der offiziellen Arduino IDE: Komma-Separierte Liste für den Serial Plotter. */
    IM_SERIAL_PLOTTER_ZEIGEN,
    /* Accelerometer- und Gyro-Werte fusionieren, um Roll-/Nick- und Gierwinkel zu errechnen. */
    SENSOR_FUSION
};

/** hier einstellen, was das Programm mit den Sensorwerten anfangen soll: */
Modus modus = WERTE_LOGGEN;

// :: Kalibierung:

/* Liegt der Arduino gerade auf dem Tisch und wird nicht bewegt, muss die Z-Achse 1G (Erdbeschleunigung) ausgeben und der Rest muss 0 sein.
   Das ist nicht wirklich so und wird mit diesen Werten kalibriert: */
const float ACC_X_OFFSET = +0.0231;
const float ACC_Y_OFFSET = +0.0105;
const float ACC_Z_OFFSET = -0.0102;

/* Wird der Arduino nicht rotiert, müssen alle Winkelbeschleunigungen 0 sein.
   Das ist nicht wirklich so und wird mit diesen Werten kalibriert: */
const float GYRO_X_OFFSET = -0.43;
const float GYRO_Y_OFFSET = +0.67;
const float GYRO_Z_OFFSET = +0.31;

int rgbValues[] = { 255, 0, 0 }; // 0=Red, 1=Green and 2=Blue

void setup() {
    Serial.begin(115200);

    // I2C Bus 1x für alle Bus-Teilnehmer initialisieren (sonst crasht das Betriebssystem):
    Wire.begin();
    Wire.setClock(1000000); // 1MHz Kommunikationsgeschwindigkeit

    if (!IMU.begin()) {
        delay(10000); // damit wir Zeit haben den Serial Monitor zu öffnen nach dem Upload
        Serial.println("IMU Verdrahtung prüfen! Programm Ende.");
        while (1);
    }

    Serial.println("Onboard_Sensoren Initialisierung abgeschlossen");
    
    pinMode(LEDR, OUTPUT);
    pinMode(LEDG, OUTPUT);
    pinMode(LEDB, OUTPUT);

    Serial.println("LED Initialisierung abgeschlossen");
}

bool imuFusionInitialisiert = false;

double accxalt;
double accyalt;
double acczalt;

// hier speichern wir die Sensorwerte ab:
vec3_t acc = vec3_t(0, 0, 0); // (x, y, z)
vec3_t gyro = vec3_t(0, 0, 0); // (x, y, z)

void loop() {
    accxalt = acc.x;   
    accyalt = acc.y;
    acczalt = acc.z;
    readAcc();
    ReadGyro();

    // Serial.println(fabs  (acc.x - accxalt));
    // Serial.println(fabs  (acc.y - accyalt));
    // Serial.println(acczalt);
    // Serial.println(fabs  (acc.z - acczalt));
    
    // if ((fabs (acc.x - accxalt) >= 0.06) || (fabs (acc.y - accyalt) >= 0.06) || (fabs (acc.z - acczalt) >= 0.06))
    // {
    //     analogWrite(LEDR, 0);
    //     analogWrite(LEDG, 255);
    //     analogWrite(LEDB, 0);
    // }
    // else {
    //     analogWrite(LEDR, 255);
    //     analogWrite(LEDG, 0);
    //     analogWrite(LEDB, 0);
    // }
    Serial.println(String(gyro.x) + "," + String(gyro.y) + "," + String(gyro.z)) + "\n";
    switch (modus) {
        case WERTE_LOGGEN:
            werteLoggen();
            break;

        case IM_SERIAL_PLOTTER_ZEIGEN:
            fuerPlotterLoggen();
            delay(100); // damit man im Plotter auch was erkennt und nicht alles so schnell vorbei fliegt
            break;

        case SENSOR_FUSION:
            if (!imuFusionInitialisiert) {
                fusionInitialisieren();
                imuFusionInitialisiert = true;
            } else {
                fusionLoggen();
                /** Da uns allerdings noch ein Magnetsensor fehlt, funktioniert die Sensorfusion nicht so wirklich.
                 *  Vielleicht findest du ja eine bessere Bibliothek dafür.
                 *  Alternativ könnt ihr auch einen externen 9-Achsen Sensor bekommen,
                 *  der die Sensorfusion on-board macht und besser funktionieren sollte,
                 *  aber mehr Platz verbraucht. */
            }
            break;
    }
   // delay(1000);
}

void readAcc() {
    if (IMU.accelerationAvailable()) {
        IMU.readAcceleration(acc.x, acc.y, acc.z);
        acc.x += ACC_X_OFFSET;
        acc.y += ACC_Y_OFFSET;
        acc.z += ACC_Z_OFFSET;
    } // else: nicht jedes Mal, wenn dir den Sensor fragen, hat er auch neue Werte
}

void ReadGyro() {
    if (IMU.gyroscopeAvailable()) {
        IMU.readGyroscope(gyro.x, gyro.y, gyro.z);
        gyro.x += GYRO_X_OFFSET;
        gyro.y += GYRO_Y_OFFSET;
        gyro.z += GYRO_Z_OFFSET;
    } // else: nicht jedes Mal, wenn dir den Sensor fragen, hat er auch neue Werte
}

void werteLoggen() {
    // Serial.println(
    //     "Beschleunigung x=" + String(acc.x) + " y=" + String(acc.y) + " z=" + String(acc.z)
    //     + " Rotationsgeschwindigkeit x=" + String(gyro.x) + " y=" + String(gyro.y) + " z=" + String(gyro.z));
}

void fuerPlotterLoggen() {
    Serial.println(
        String(acc.x) + "," + String(acc.y) + "," + String(acc.z));
    // Serial.println(
    //     String(gyro.x) + "," + String(gyro.y) + "," + String(gyro.z));
}

void fusionInitialisieren() {
    Serial.println("Der Arduino muss jetzt gerade liegen und darf nicht bewegt werden!");
    Fusion.setup(acc);
}

void fusionLoggen() {
    Fusion.update(gyro, acc);
    /* Wenn man aus Sicht des Arduino Richting USB-Stecker schaut: */
    Serial.println(
        "Gierwinkel (links<->rechts schauen)=" + String(Fusion.yaw())
        + " Nickwinkel (hoch<->runten schauen)=" + String(Fusion.pitch())
        + " Rollwinkel (links<->rechts kippen)=" + String(Fusion.roll()));
}
