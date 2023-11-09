#include "ZumoMotors.h"

ZumoMotors motoren;
int jabadabadu = 30;

void setup() {
  // kann benutzt werden, um überall im Programm die Drehrichtung der Motoren zu ändern.
  // normalerweise nicht notwendig; kann weggelassen werden:
  motoren.flipLeftMotor(true);   // Drehrichtung linker Motor nicht umdrehen
  motoren.flipRightMotor(true);  // Drehrichtung rechter Motor nicht umdrehen
}

void loop() {
   // vorwärts
   // anscheinend sind -400 bis +400 möglich?
   // Motorgeschwindigkeiten können einzeln gesetzt werden...

   // rückwärts
   motoren.setSpeeds(150,150);     
}
