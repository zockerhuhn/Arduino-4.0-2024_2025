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
  
    while (abstandsWert > 100) {
      straight();
      if (digitalRead(motorPin)) {
      stop();
      return;
    }
      delay(2000);
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