#pragma once
/** optional: Stoppuhr, um zu Verbindungsverluste zu erkennen */
if(debT){Chrono keineNeuenDatenStoppuhr = Chrono(Chrono::MILLIS, false)}; // noch nicht gestartet 
// !TODO: Setup und Loop löschen und das ganze in eine wirkliche library umwandeln
void setup() {

    Serial.println("Initialisierung des 64-Kanal ToF kann bis zu 10 Sekunden dauern...");
    

    abstandsSensor.startRanging();
    if(debT){keineNeuenDatenStoppuhr.start()};
    Serial.println("Initialisierung abgeschlossen");
}

const uint16_t VERBINDUNG_VERLOREN = 0;
uint16_t vorheriger_abstand = VERBINDUNG_VERLOREN;
void readDistances() {
    if (abstandsSensor.isDataReady()) {
        // diese Zeile speicher bereits die Daten ab:
        if (abstandsSensor.getRangingData(&messDaten)) {
            // alles OK
            if(debT){keineNeuenDatenStoppuhr.restart()};
            return;  // rausgehen aus der Funktion, damit wir nicht zum Fehler kommen
        } // else: Fehler
    } // else: wenn es keine neuen Daten gibt, müssen wir sie auch nicht lesen

    /* können wir bei Fehlern nutzen, sonst isses unnötig */
    // Fehler:
    if(debT){
        if (keineNeuenDatenStoppuhr.hasPassed(1000)) {
            Serial.println("ToF64 Verdrahtung prüfen! Roboter aus- und einschalten!");
        }
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
