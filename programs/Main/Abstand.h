#pragma once

// void readDistances() {
//     if (abstandsSensor.isDataReady()) {
//         // diese Zeile speicher bereits die Daten ab:
//         if (abstandsSensor.getRangingData(&messDaten)) {
//             // alles OK
//             if(debT){keineNeuenDatenStoppuhr.restart();}
//             return;  // rausgehen aus der Funktion, damit wir nicht zum Fehler kommen
//         } // else: Fehler
//     } // else: wenn es keine neuen Daten gibt, müssen wir sie auch nicht lesen

//     /* können wir bei Fehlern nutzen, sonst isses unnötig */
//     // Fehler:
//     if(debT){
//         if (keineNeuenDatenStoppuhr.hasPassed(1000)) { // eingerückt, weil sonst
//             Serial.println("abstandsWert: ToF64 Verdrahtung prüfen! Roboter aus- und einschalten!");
//         }
//     } 
// }


void readDistance() {
    if (!abstandsSensor.timeoutOccurred()) {
        abstandsWert = abstandsSensor.readRangeContinuousMillimeters();
        // statt 65535 kann es auch passieren, dass sich der Wert einfach nicht mehr ändert
        if (abstandsWert != 65535) {
            // alles OK
            if (vorheriger_abstand != abstandsWert) {
                // merken: der Wert hat sich verändert
                vorheriger_abstand = abstandsWert;
            }
            return; // rausgehen aus der Funktion, damit wir nicht zum Fehler kommen
        }
    }
    // Fehler:
    abstandsWert = VERBINDUNG_VERLOREN;
    Serial.println("ToF Verdrahtung prüfen! Roboter aus- und einschalten! " + String(abstandsSensor.readRangeContinuousMillimeters()));
}

int tenAverage(int *array) {
  int avg = 0;
  for (int i = 0; i < 10; i++) avg += array[i];
  return avg / 10;
}

void werteLoggen() {
    Serial.println("abstandsWert: " + String(abstandsWert));
}
