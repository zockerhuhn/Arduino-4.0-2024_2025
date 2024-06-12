void readColor() //liest Farbsensor 1 aus und printed Ergebnis
{
  rgbSensor.getRawData(&rot, &gruen, &blau, &helligkeit);
  Serial.println("R:" + String(rot) + " G:" + String(gruen) + " B:" + String(blau) + " C:" + String(helligkeit));
  /** Dieser Mechanismus hier ist gefährlich, wenn es passieren kann, dass die Sensoren lange Zeit das selbe sehen:
   *  In meinen Versuchen habe ich oben den Gain von 4x auf 16x gestellt, um mehr Rauschen zu bekommen.
   *  Mit Timeout 5s sehe ich keine False-Negatives mehr: */
}

void readColor2() //liest Farbsensor 2 aus und printed Ergebnis
{
  rgbSensor2.getRawData(&rot2, &gruen2, &blau2, &helligkeit2);
  Serial.println("R:" + String(rot2) + " G:" + String(gruen2) + " B:" + String(blau2) + " C:" + String(helligkeit2));
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
      Serial.println("right:" + String(gruen) + " " + String(blau) + " " + String(rot) + " " + String((gruen-blueGreenThreshold) >= blau) + " " + String((gruen-redGreenThreshold) >= rot) + " " + String(helligkeit <= colorBrightMaxThreshold)  + " " + String(helligkeit >= colorBrightMinThreshold));
      return false;
    }
  }
}

boolean calculateColor2() //nimmt letzten ausgelesen Wert von Farbsensor 2 und returnt ob grün erkannt wurde als bool
{
  {
    if ((gruen2-blueGreenThreshold) >= blau2 && (gruen2-redGreenThreshold) >= rot2 && helligkeit2 <= colorBrightMaxThreshold && helligkeit2 >= colorBrightMinThreshold)
    {
      return true;
    }
    else
    {
      Serial.println("left:" + String(gruen2) + " " + String(blau2) + " " + String(rot2) + " " + String((gruen2-blueGreenThreshold) >= blau2) + " " + String((gruen2-redGreenThreshold) >= rot2) + " " + String(helligkeit2 <= colorBrightMaxThreshold) + " " + String(helligkeit2 >= colorBrightMinThreshold));
      return false;
    }
  }
}
