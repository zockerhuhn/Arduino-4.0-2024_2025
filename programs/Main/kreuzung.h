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
  motors.setSpeeds(25,25);
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
      turn();
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
    if (green2)
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
        // finding line
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
