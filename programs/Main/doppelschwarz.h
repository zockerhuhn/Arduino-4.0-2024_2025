void doppelschwarz(bool bothsides)
{
  Serial.print("\n");
  Serial.print("alles-schwarz");
  straight();
  if (bothsides) {
    delay(550);
  }
  else {
    delay(700);
  }
  motors.setSpeeds(0,0);
  readColor2();
  readColor();
  delay(1000);  
  if (calculateColor())
  {
    Serial.print("rechts");
    if (calculateColor2())
    {
      Serial.print("links");
      turn();
      while (calculateReflection() == "noLine")
      {
        delay(10);
      }
    }
    else
    {
      straight();
      delay(600);
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
      delay(600);
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
      delay(900);
      motors.setSpeeds(0, 0);
      if (!(calculateReflection() == "noLine"))
      {
        // not else lol
      }
      else
      {
        motors.flipLeftMotor(true);
        motors.flipRightMotor(true);
        motors.setSpeeds(70, 65);
        delay(2000);
        motors.flipLeftMotor(false);
        motors.flipRightMotor(false);
        motors.setSpeeds(60, 75); }
        while (calculateReflection() == "noLine")
        {
          Serial.print("\n");
          Serial.print("suche...");
        }
    }
  }
}
