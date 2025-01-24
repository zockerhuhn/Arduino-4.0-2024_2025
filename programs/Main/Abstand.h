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

int findAverage(int *array, int size) {
  int avg = 0;
  for (int i = 0; i < size; i++) {
    if (array[i] < 8000 && array[i] > 10) {
      avg += array[i];
    }
    else {
      return max(array[i], 8190);
    }
  }
  return avg / size;
}

void werteLoggen() {
    Serial.println("abstandsWert: " + String(abstandsWert));
}

void move_arr_back(int *array, int size) {
  for (int i = 1; i < size; i++) {
    array[i-1] = array[i];
  }
}

void readDistance() {
    if (!abstandsSensor.timeoutOccurred()) {
        abstandsWert = abstandsSensor.readRangeContinuousMillimeters();
        // werteLoggen();
        // statt 65535 kann es auch passieren, dass sich der Wert einfach nicht mehr ändert
        if (abstandsWert != 65535) {
            // alles OK
            if (vorheriger_abstand != abstandsWert) {
                // merken: der Wert hat sich verändert
                vorheriger_abstand = abstandsWert;
            }
            move_arr_back(abstandsWerte, 5);
            abstandsWerte[4] = abstandsWert;

            abstandsWert = findAverage(abstandsWerte, 5);
            return; // rausgehen aus der Funktion, damit wir nicht zum Fehler kommen
        }
    }
    // Fehler:
    abstandsWert = VERBINDUNG_VERLOREN;
    Serial.println("ToF Verdrahtung prüfen! Roboter aus- und einschalten! " + String(abstandsSensor.readRangeContinuousMillimeters()));
}
