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
      Serial.print("Found green 1 (right)");
    }
    if (calculateColor2()) {
      green2 = true;
      Serial.print("Found green 2 (left)");
    }
    if (green1 || green2) motors.setSpeeds(15,15);
    if (green1 && green2) break;
    // (problem) maybe this moves too far
  } // TODO set motor speeds according to the weird deficiencies they have
  

  if (green1)
  {
    Serial.print("rechts");
    if (green2)
    {
      Serial.print("links");
      right(180);
      delay(150); 
      while (calculateReflection() == "noLine")
      {
        delay(10);
      }
    }
    else
    {
      straight();
      delay(600);
      right(90);
      while (calculateReflection() == "noLine")
      {
        delay(1);
      }
    }
  }
  else
  {
    if (green2)
    {
      Serial.print("links");
      straight();
      delay(600);
      left(90);
    }
    else
    {
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
}
