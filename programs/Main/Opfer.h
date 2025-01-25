#include "Abstand.h"

void opfer() {
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);
  for (int j = 0; j < 4; j++) {
    if (digitalRead(motorPin)) {
      stop();
      return;
    }

    for (int i = 0; i < 5; i++) readDistance();


    if (digitalRead(motorPin)) {
      stop();
      return;
    }
  
    while (abstandsWert > 150) {
      straight(3);
      if (digitalRead(motorPin)) {
        stop();
        return;
      }
      delay(400);

      stop();
      if ((calculatedReflection = calculateReflection()) != "noLine") return;
      for (int i = 0; i < 5; i++) readDistance();
      if (abstandsWert < 200) break;

      left();
      if (digitalRead(motorPin)) {
        stop();
        return;
      }
      delay(2000);
      for (int i = 0; i < 5; i++) readDistance();
      if (abstandsWert > 500) return;
      right();
      if (digitalRead(motorPin)) {
        stop();
        return;
      }
      delay(2000);
      stop();
      for (int i = 0; i < 5; i++) readDistance();
    }

    for (int i = 0; i < 5; i++) readDistance();
    if (abstandsWert > 500) return;
    right();
    if (digitalRead(motorPin)) {
      stop();
      return;
    }
    delay(2000);
  }

  return;
}