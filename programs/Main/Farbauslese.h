void readColor()
{
  rgbSensor.getRawData(&rot, &gruen, &blau, &helligkeit);
  Serial.println("R:" + String(rot) + " G:" + String(gruen) + " B:" + String(blau) + " C:" + String(helligkeit));
  /** Dieser Mechanismus hier ist gefährlich, wenn es passieren kann, dass die Sensoren lange Zeit das selbe sehen:
   *  In meinen Versuchen habe ich oben den Gain von 4x auf 16x gestellt, um mehr Rauschen zu bekommen.
   *  Mit Timeout 5s sehe ich keine False-Negatives mehr: */
}

void readColor2()
{
  rgbSensor2.getRawData(&rot2, &gruen2, &blau2, &helligkeit2);
  Serial.println("R:" + String(rot2) + " G:" + String(gruen2) + " B:" + String(blau2) + " C:" + String(helligkeit2));
  /** Dieser Mechanismus hier ist gefährlich, wenn es passieren kann, dass die Sensoren lange Zeit das selbe sehen:
   *  In meinen Versuchen habe ich oben den Gain von 4x auf 16x gestellt, um mehr Rauschen zu bekommen.
   *  Mit Timeout 5s sehe ich keine False-Negatives mehr: */
}

boolean calculateColor()
{
  {
    if (gruen >= colorMaxThreshold && helligkeit >= colorBrightMaxThreshold)
    {
      return true;
    }
    else
    {
      return false;
    }
  }
}

boolean calculateColor2()
{
  {
    if (gruen2 >= colorMaxThreshold && helligkeit2 >= colorBrightMaxThreshold)
    {
      return true;
    }
    else
    {
      return false;
    }
  }
}