/** importiert Arduino automatisch, muss man also hier nicht unbedingt auch noch mal importieren: */
#include <Arduino.h>

/**
 * !!! Immer darauf achten, dass unten in der Statusleiste...
 *     ... das richtige Arduino Board eingestellt ist
 *     ... das richtige "Sketch File" ausgewählt ist (das ändert sich nämlich nicht automatisch)
 *     ... die richtige C/C++ Konfiguration eingestellt ist (sonst gibt es noch mehr "rote swiggels")
 * 
 * :: Regenbogen-Animation mit der Onboard RGB LED. ::
 * 
 * Hardware-Aufbau:
 * Arduino Nano RP2040 Connect (!! Arduino Due geht nicht !!)
 * <nichts weiter>
 * 
 * Pins LEDR (rot), LEDG (grün), LEDB (blau) sind die 3 Farbkanäle der RGB LED.
 * Diese Pins sind nicht direkt an den RP2040 Chip (Haupt-Prozessor) angebunden,
 * sondern an den ESP32, der für WiFi und Bluetooth zuständig ist. Es muss also
 * mit dem ESP32 kommuniziert werden. Dafür wird eine von 2 Libraries verwendet:
 * 
 * (a) #include <WiFiNINA.h>
 *     RP2040 --SPI--> ESP32 --GPIOs--> RGB LED
 * (b) #include <ArduinoBLE.h>
 *     RP2040 --I2C--> ESP32 (GPIOs stehen nicht zur Verfügung)
 * 
 * !!! Es können nicht gleichzeitig beide Libraries verwendet werden !!!
*/

/** Diese Pins hängen nicht am Hauptprozessor, sondern am WiFi/Bluetooth Prozessor:
 *  LEDR, LEDG, LEDB, A6, A7
 *  Es muss also die WiFiNINA Library importiert werden, um sie zu nutzen. */
#include <WiFiNINA.h>

void setup() {
    Serial.begin(115200);
    pinMode(LEDR, OUTPUT);
    pinMode(LEDG, OUTPUT);
    pinMode(LEDB, OUTPUT);

    Serial.println("Initialisierung abgeschlossen");
}

/**
 * rot:  rgbValues[0]
 * grün: rgbValues[1]
 * blau: rgbValues[2]
*/
int rgbValues[] = { 255, 0, 0 }; // 0=Red, 1=Green and 2=Blue
int upIndex = 0, downIndex = 1;

/**
 * Da dies bereits eine Schleife ist, müssen wir nur
 * upIndex hochzählen und downIndex herunterzählen:
*/
void loop() {
    Serial.println("Hauptprogrammschleife läuft...");

    rgbValues[upIndex] += 1;
    rgbValues[downIndex] -= 1;

    if (rgbValues[upIndex] > 255) {
        rgbValues[upIndex] = 255;
        upIndex = upIndex + 1;
        if (upIndex > 2) {
            upIndex = 0;
        }
    }

    if (rgbValues[downIndex] < 0) {
        rgbValues[downIndex] = 0;
        downIndex = downIndex + 1;
        if(downIndex > 2){
            downIndex = 0;
        }
    }

    analogWrite(LEDR, rgbValues[0]);
    analogWrite(LEDG, rgbValues[1]);
    analogWrite(LEDB, rgbValues[2]);

    delay(5);
}
