#pragma once
//#include "variables.h"
//#include "Kalibrierung.h"

void readColor() //liest Farbsensor 1 aus und printed Ergebnis
{
  old_colour[0] = rot;old_colour[1] = gruen;old_colour[2] = blau;old_colour[3] = helligkeit;
  rgbSensor.getRawData(&rot, &gruen, &blau, &helligkeit);
  //Serial.println("R:" + String(rot) + " G:" + String(gruen) + " B:" + String(blau) + " C:" + String(helligkeit));
  /** Dieser Mechanismus hier ist gefährlich, wenn es passieren kann, dass die Sensoren lange Zeit das selbe sehen:
   *  In meinen Versuchen habe ich oben den Gain von 4x auf 16x gestellt, um mehr Rauschen zu bekommen.
   *  Mit Timeout 5s sehe ich keine False-Negatives mehr: */
}

void readColor2() //liest Farbsensor 2 aus und printed Ergebnis
{
  old_colour2[0] = rot2;old_colour2[1] = gruen2;old_colour2[2] = blau2;old_colour2[3] = helligkeit2;
  rgbSensor2.getRawData(&rot2, &gruen2, &blau2, &helligkeit2);
  //Serial.println("R:" + String(rot2) + " G:" + String(gruen2) + " B:" + String(blau2) + " C:" + String(helligkeit2));
  /** Dieser Mechanismus hier ist gefährlich, wenn es passieren kann, dass die Sensoren lange Zeit das selbe sehen:
   *  In meinen Versuchen habe ich oben den Gain von 4x auf 16x gestellt, um mehr Rauschen zu bekommen.
   *  Mit Timeout 5s sehe ich keine False-Negatives mehr: */
}

boolean calculateColor() //nimmt letzten ausgelesen Wert von Farbsensor 1 und return ob grün erkannt wurde als bool 
{
  {
    if ((gruen-blueGreenThreshold) >= blau && (gruen-redGreenThreshold) >= rot && helligkeit <= colorBrightMaxThreshold && helligkeit >= colorBrightMinThreshold)
    {
      return true;
    }
    else
    {
      //Serial.println("right:" + String(gruen) + " " + String(blau) + " " + String(rot) + " " + String((gruen-blueGreenThreshold) >= blau) + " " + String((gruen-redGreenThreshold) >= rot) + " " + String(helligkeit <= colorBrightMaxThreshold)  + " " + String(helligkeit >= colorBrightMinThreshold));
      return false;
    }
  }
}

boolean calculateColor2() //nimmt letzten ausgelesen Wert von Farbsensor 2 und returnt ob grün erkannt wurde als bool
{
  {

    if ((gruen2-blueGreenThreshold2) >= blau2 && (gruen2-redGreenThreshold2) >= rot2 && helligkeit2 <= colorBrightMaxThreshold && helligkeit2 >= colorBrightMinThreshold)
    {
      return true;
    }
    else
    {
      //Serial.println("left:" + String(gruen2) + " " + String(blau2) + " " + String(rot2) + " " + String((gruen2-blueGreenThreshold) >= blau2) + " " + String((gruen2-redGreenThreshold) >= rot2) + " " + String(helligkeit2 <= colorBrightMaxThreshold) + " " + String(helligkeit2 >= colorBrightMinThreshold));
      return false;
    }
  }
}

bool valid_red() {
  if ((1.3 * (blau + gruen) <= rot + 300) || (1.3 * (old_colour[2] + old_colour[1]) <= old_colour[0] + 300)) {
    return true;
  }
  return false;
}

bool valid_red2() {
  if ((1.3 * (blau2 + gruen2) <= rot2 + 300) || (1.3 * (old_colour2[2] + old_colour2[1]) <= old_colour2[0] + 300)) {
    return true;
  }
  return false;
}

void is_red() {
  readColor();
  readColor2();
  if ((valid_red() && valid_red2()) && (helligkeit <= colorBrightMaxThreshold + 800 || helligkeit2 <= colorBrightMaxThreshold + 800)) {
    
    stop();
    digitalWrite(LEDR, HIGH);
    
    // Serial.println("red"); 
    // Serial.println("red vals: " + String(rot) + " " + String(gruen) + " " + String(blau) + " " + String(helligkeit) + "\t " + String(rot2) + " " + String(gruen2) + " " + String(blau2) + " " + String(helligkeit2));
    delay(8000); // More than 5 seconds

    if (digitalRead(motorPin)) {
      stop();
      return;
    }
  }
  digitalWrite(LEDR, LOW);
}
