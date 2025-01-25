#include "Abstand.h"

void opfer() {
  // digitalWrite(LEDR, HIGH);
  // digitalWrite(LEDG, HIGH);
  // digitalWrite(LEDB, HIGH);
  for (int j = 0; j < 4; j++) {
    for (int i = 0; i < 5; i++) readDistance();
  
    while (abstandsWert > 100) {
      straight();
      delay(3000);
      left(90);
      for (int i = 0; i < 5; i++) readDistance();
      if (abstandsWert > 500) return;
      right(90);
      for (int i = 0; i < 5; i++) readDistance();
    }

    for (int i = 0; i < 5; i++) readDistance();
    if (abstandsWert > 500) return;
    right(90);
  }

  return;
}