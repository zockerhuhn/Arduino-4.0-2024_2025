void doppelschwarz()
{
  Serial.print("\n");
  Serial.print("alles-schwarz");
  motoren.setSpeeds(0, 0);
  delay(1000);
  readColor2();
  readColor();
  if (calculateColor())
  {
    if (calculateColor2())
    {
      turn();
    }
    else
    {
      straight();
      delay(1250);
      right();
      delay(1000);
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
      straight();
      delay(1250);
      left();
      delay(1000);
      while (calculateReflection() == "noLine")
      {
        delay(1);
      }
    }
    else
    {
      straight();
      delay(1200);
      motoren.setSpeeds(0, 0);
      if (!(calculateReflection() == "noLine"))
      {
        // not else lol
      }
      else
      {
        left();
        delay(2500);
        right();
        while (calculateReflection() == "noLine")
        {
          Serial.print("\n");
          Serial.print("suche...");
        }
      }
    }
  }
}