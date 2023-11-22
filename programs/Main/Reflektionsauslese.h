void read_reflectionandprint()
{
  sensorLeiste.read(helligkeiten);
  for (int i = 0; i < SENSOR_LEISTE_ANZAHL_SENSOREN; i++)
  {
    Serial.print(String(helligkeiten[i]) + '\t'); // alles in eine Zeile
  }
  Serial.println(); // neue Zeile beginnen
}

String calculateReflection()
{
  read_reflectionandprint();
  if ((helligkeiten[0] >= reflectionBlackThreshold) && (helligkeiten[5] >= reflectionBlackThreshold))
  {
    return "frontalLine";
  }
  else if ((helligkeiten[2] >= reflectionBlackThreshold || helligkeiten[3] >= reflectionBlackThreshold) && (helligkeiten[0] <= reflectionBlackThreshold && helligkeiten[1] <= reflectionBlackThreshold && helligkeiten[4] <= reflectionBlackThreshold && helligkeiten[5] <= reflectionBlackThreshold))
  {
    return "normalLine";
  }
  else if (helligkeiten[0] >= reflectionBlackThreshold || helligkeiten[1] >= reflectionBlackThreshold)
  {
    return "leftLine";
  }
  else if (helligkeiten[4] >= reflectionBlackThreshold || helligkeiten[5] >= reflectionBlackThreshold)
  {
    return "rightLine";
  }
  else
  {
    return "noLine";
  }
}