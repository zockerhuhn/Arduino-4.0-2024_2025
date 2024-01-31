void doppelschwarz()
{
  Serial.print("\n");
  Serial.print("alles-schwarz");
  motors.setSpeeds(0, 0);
  delay(1000);
  straight();
  delay(350);
  motors.setSpeeds(0,0);
  readColor2();
  readColor();
  delay(60);
  if (calculateColor())
  {
    Serial.print("rechts");
    if (calculateColor2())
    {
      Serial.print("links");
      turn();
    }
    else
    {
      straight();
      delay(500);
      right();
      delay(1500);
      while (calculateReflection() == "noLine")
      {
        delay(1);
      }
    }
  }
  else
  {
    if (calculateColor2())
    {
      Serial.print("links");
      straight();
      delay(500);
      left();
      delay(1500);
      while (calculateReflection() == "noLine")
      {
        delay(1);
      }
    }
    else
    {
      straight();
      delay(1200);
      motors.setSpeeds(0, 0);
      if (!(calculateReflection() == "noLine"))
      {
        // not else lol
      }
      else
      {
        if (calculateReflection() != "normalLine") {
        left();
        delay(2500);
        right(); }
        while (calculateReflection() == "noLine")
        {
          Serial.print("\n");
          Serial.print("suche...");
        }
      }
    }
  }
}