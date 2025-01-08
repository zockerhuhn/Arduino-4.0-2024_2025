void read_reflectionandprint() //read reflectionsensor and print result
{
  sensorLeiste.read(helligkeiten);
  for (int i = 0; i < SENSOR_LEISTE_ANZAHL_SENSOREN; i++)
  {
    Serial.print(String(helligkeiten[i]) + '\t'); // alles in eine Zeile
  }
  Serial.println(); // neue Zeile beginnen
}

void read_reflection() //read reflectionsensor
{
  sensorLeiste.read(helligkeiten);
}
 

String calculateReflection() //read reflection and return processed result 
// !!! Green also counts as black !!!
{
  read_reflection();
  // read_reflectionandprint();
  if ((helligkeiten[0] >= reflectionBlackThreshold) && (helligkeiten[5] >= reflectionBlackThreshold))
  {
    return "frontalLine";
  }
  else if ((helligkeiten[0] >= reflectionBlackThreshold && helligkeiten[1] >= reflectionBlackThreshold && helligkeiten[2] >= reflectionBlackThreshold))
  {
    return "sideRightLine";
  }
  else if ((helligkeiten[3] >= reflectionBlackThreshold && helligkeiten[4] >= reflectionBlackThreshold && helligkeiten[5] >= reflectionBlackThreshold)) {
    return "sideLeftLine";
  }
  else if (helligkeiten[5] >= reflectionBlackThreshold)
  {
    return "hardleftLine";
  }
  else if (helligkeiten[0] >= reflectionBlackThreshold)
  {
    return "hardrightLine";
  }
  else if (helligkeiten[4] >= reflectionBlackThreshold)
  {
    return "leftLine";
  }
  else if (helligkeiten[1] >= reflectionBlackThreshold)
  {
    return "rightLine";
  }
  else if ((helligkeiten[2] >= reflectionBlackThreshold) || (helligkeiten[3] >= reflectionBlackThreshold))
  {
    return "normalLine";
  }
  else
  {
    return "noLine";
  }
}

void ReadDirection() {
  Wire.beginTransmission(CMPS12);
  Wire.write(0X02);
  
  if (Wire.endTransmission(false)) {
    direction = -1; // set an invalid value when the transmission had an error
  } else {
    Wire.requestFrom(CMPS12, 2);
    direction = (int16_t)(Wire.read()<<8|Wire.read())/10.00; //Two bytes Yaw in range of (0 to 359 degrees)
  }
}