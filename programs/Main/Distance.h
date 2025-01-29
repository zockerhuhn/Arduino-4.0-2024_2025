#pragma once

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

void logDistance() {
    Serial.println("distance value: " + String(distance_val));
}

void moveArrBack(int *array, int size) {
  for (int i = 1; i < size; i++) {
    array[i-1] = array[i];
  }
}

void readDistance() {
    if (!tofSensor.timeoutOccurred()) {
        distance_val = tofSensor.readRangeContinuousMillimeters();
        // logDistanceVals();
        // statt 65535 kann es auch passieren, dass sich der Wert einfach nicht mehr ändert
        if (distance_val != 65535) {
            // alles OK
            if (last_distance_val != distance_val) {
                // merken: der Wert hat sich verändert
                last_distance_val = distance_val;
            }
            moveArrBack(distance_array, 5);
            distance_array[4] = distance_val;

            distance_val = findAverage(distance_array, 5);
            return; // rausgehen aus der Funktion, damit wir nicht zum Fehler kommen
        }
    }
    // Fehler:
    distance_val = LOST_CONNECTION;
    Serial.println("ToF Verdrahtung prüfen! Roboter aus- und einschalten! " + String(tofSensor.readRangeContinuousMillimeters()));
}
