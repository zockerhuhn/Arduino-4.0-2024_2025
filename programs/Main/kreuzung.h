//#include "Motorbewegungen.h"
//#include "Farbauslese.h"
//#include "Reflektionsauslese.h"

void kreuzung(bool bothsides)
{
  Serial.print("\n");

  
  straight();
  if (bothsides) {
    Serial.print("alles schwarz\t");
    delay(300);
  }
  else {
    Serial.print("einseitig schwarz\t");
    delay(450);
  }
  straight(0.5); // driving at half speed
  bool green1 = false;
  bool green2 = false;
  for (int i = 0; i < 15; i++) {
    readColor2();
    readColor();
    delay(10);  

    if (calculateColor()) {
      green1 = true; 
      Serial.print("Found green 1 (right)\t");
    }
    if (calculateColor2()) {
      green2 = true;
      Serial.print("Found green 2 (left)\t");
    }
    if (green1 || green2) {
      digitalWrite(LED_BUILTIN, HIGH);
      straight(1);
      }
    if (green1 && green2) {
      stop();
      break;
    }
    // ACTUAL PROBLEM often the sensor reads green1 at the end (like that: alles schwarz	Found green 2 (left)	Found green 2 (left)	Found green 2 (left)	Found green 1 (right)	both	)
    // even though it doesn't occur and maaaybe bc of turning or black/white grenze idk
    // (problem) maybe this moves too far
  }
  digitalWrite(LED_BUILTIN, LOW);
  
  if (green1 && green2) {
    Serial.print("both\t");
      right(180);
      delay(150); 
      while (calculateReflection() == "noLine")
      {
        delay(10);
      }
  }
  else if (green1) {
    straight();
      delay(600);
      right(90);
      while (calculateReflection() == "noLine")
      {
        delay(1);
      }
  }
  else if (green2) {
    Serial.print("links");
      straight();
      delay(600);
      left(90);
  }
  else {
    if (!(calculateReflection() == "noLine")/*doesnt work otherwise apparently*/)
      {
        // not else lol
      }
      else
      {
        // finding line
        left(90);
        right(); // going right "forever"    
        while (calculateReflection() == "noLine")
        {
          Serial.print("\n");
          Serial.print("suche...");
        }
      }
  }
}
