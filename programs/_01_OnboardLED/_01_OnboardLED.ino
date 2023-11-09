/** importiert Arduino automatisch, muss man also hier nicht unbedingt auch noch mal importieren: */
#include <Arduino.h>

/**
 * !!! Immer darauf achten, dass unten in der Statusleiste...
 *     ... das richtige Arduino Board eingestellt ist
 *     ... das richtige "Sketch File" ausgewählt ist (das ändert sich nämlich nicht automatisch)
 *     ... die richtige C/C++ Konfiguration eingestellt ist (sonst gibt es noch mehr "rote swiggels")
 * 
 * :: Onboard-LED blinken lassen ::
 * 
 * Hardware-Aufbau:
 * Arduino Nano RP2040 Connect / Arduino Due
 * <nichts weiter>
 * 
 * Pin 13 (LED_BUILTIN) sollte lieber nicht für andere Dinge verwendet werden, da die LED am selben
 * Netz hängt und potentiell stören kann.
*/

void setup() {
    Serial.begin(115200);

    pinMode(LED_BUILTIN, OUTPUT); // Pin D13

    Serial.println("Initialisierung abgeschlossen");
}

/**
 * Nicht vergessen: Dies hier ist eine Endlos-Schleife!
*/
void loop() {
    Serial.println("Hauptprogrammschleife läuft...");

    // 5x blinken (AN/AUS):
    for (int i = 0; i < 5; i++) {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(250); // ms
        digitalWrite(LED_BUILTIN, LOW);
        delay(250); // ms
    }

    // in 256 Schritten alle 10 Millisekunden die LED heller machen:
    for (int i = 0; i < 255; i++) {
        analogWrite(LED_BUILTIN, i);
        delay(10); // ms
    }
}
