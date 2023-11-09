/** importiert Arduino automatisch, muss man also hier nicht unbedingt auch noch mal importieren: */
#include <Arduino.h>

/**
 * !!! Immer darauf achten, dass unten in der Statusleiste...
 *     ... das richtige Arduino Board eingestellt ist
 *     ... das richtige "Sketch File" ausgewählt ist (das ändert sich nämlich nicht automatisch)
 *     ... die richtige C/C++ Konfiguration eingestellt ist (sonst gibt es noch mehr "rote swiggels")
 * 
 * :: Zahlen und Text im Serial Monitor anzeigen ::
 * :: Haupt-Programm-Schleife in Zustände unterteilen ::
 * :: Programm interaktiv durchschreiten (zum Debugen). ::
 * :: Nebenbei Zusammenfassung der Datentypen ::
 * 
 * Hardware-Aufbau:
 * Arduino Nano RP2040 Connect
 * <nichts weiter>
*/

void setup() {
    /** Serielle Konsole (via USB) mit 115200 Bits/Sekunde (Baudrate) initialisieren.
     *  -> schnellere Baudrate: hängt das Programm nicht so lange auf
     *  -> langsamere Baudrate: stabilere Kommunikation
     * Für mögliche Werte: F1 drücken, "Open Serial Monitor" eingeben
     * -> das Menü zeigt alle möglichen Werte. */
    Serial.begin(115200);

    delay(5000); // 5 Sekunden blockieren, damit wir Zeit haben den seriellen Monitor zu öffnen
    warteAufEnter();

    /** 8-bit (signed) Wertebereich: -128..127 */
    int8_t kleineZahlMitVorzeichen = -128;
    /** 8-bit (unsigned) Wertebereich: 0..255 */
    uint8_t kleineZahl = 255;
    /** Basis-Variante: Jedes Mal, wenn sich der Datentyp ändert,
     *  muss man 1x print() aufrufen. */
    Serial.print("kleine Zahl mit Vorzeichen: ");
    Serial.print(kleineZahlMitVorzeichen);
    Serial.print(", kleine Zahl: ");
    Serial.println(kleineZahl); // Zeilenumbruch am Ende
    warteAufEnter();

    /** 16-bit (signed) Wertebereich: -32768..32767 */
    int16_t groessereZahlMitVorzeichen = -32768;
    /** 16-bit (unsigned) Wertebereich: 0..65535 */
    uint16_t groessereZahl = 65535;
    /** Variante: alles in einer Programm-Zeile (obwohl ich sie ungebrochen habe),
     *  indem alles in Strings umgewandelt und diese miteinander verkettet werden: */
    Serial.println("größere Zahl mit Vorzeichen: " + String(groessereZahlMitVorzeichen) +
                   ", größere Zahl: " + String(groessereZahl));
    warteAufEnter();

    /** 32-bit (signed) Wertebereich: -2147483648..2147483647 */
    int32_t riesengrosseZahlMitVorzeichen = -2147483648;
    /** 32-bit (unsigned) Wertebereich: 0..4294967295 */
    uint32_t riesengrosseZahl = 4294967295;
    Serial.println("große Zahl mit Vorzeichen: " + String(riesengrosseZahlMitVorzeichen) +
                   ", große Zahl: " + String(riesengrosseZahl));
    warteAufEnter();

    /** keine Ahnung wie viel Bit diese Datentypen auf dem Cortex M0+ haben... */
    float kleineGleitkommazahl = 0.123456789;
    double grosseGleitkommazahl = -0.123456789;
    // du wirst merken: es werde nicht alle Nachkommastellen angezeigt:
    Serial.println("kleine Gleitkommazahl: " + String(kleineGleitkommazahl) +
                   ", große Gleitkommazahl: " + String(grosseGleitkommazahl));
    warteAufEnter();

    /** Wertebereich: {true,false} */
    bool ja_oder_nein = true;
    // angezeigt als: true=1 und false=0:
    Serial.println("boolean: " + String(ja_oder_nein));
    warteAufEnter();

    char einZeichen = 'a';
    /** Muss trotzdem in einen String konvertiert werden,
     *  da sonst die Adresse der (Pointer zur) Variable ausgegeben wird, nicht der Wert. */
    Serial.println("ein Zeichen: " + String(einZeichen));
    warteAufEnter();

    String mehrereZeichen = "Bla Keks";
    /** Die Variable mehrereZeichen ist bereits vom Typ String und muss nicht konvertiert werden: */
    Serial.println("mehrere Zeichen: " + mehrereZeichen);
    warteAufEnter();
}

enum Zustand {
    ZUSTAND_A, // 1 Sekunde delay
    ZUSTAND_B  // warte auf <ENTER>
};

// Start-Zustand
Zustand zustand = ZUSTAND_A;

void loop() {
    Serial.println("Hauptprogrammschleife läuft... (Zustand=" + String(zustand) + ")");

    switch (zustand) {
        case ZUSTAND_A:
            delay(1000);
            zustand = ZUSTAND_B;
            break;
        case ZUSTAND_B:
            warteAufEnter();
            zustand = ZUSTAND_A;
            break;
    }
}

/**
 * Es muss mindestens ein Zeichen eingegeben werden,
 * oder man stellt den Serial Monitor auf "Terminal Mode" und gibt ENTER ein.
*/
void warteAufEnter() {
    Serial.println("Weiter mit <ENTER>...");
    while (!Serial.available()) {
        // noch kein Zeichen (z.B. ENTER) empfangen
    }
    while (Serial.available()) {
        // Alle Zeichen lesen und damit den Zeichen-Puffer leeren.
        Serial.read();
    }
}
