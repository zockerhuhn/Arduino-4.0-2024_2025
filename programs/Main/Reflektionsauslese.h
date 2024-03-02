void read_reflectionandprint() //read relfectionsensor and print result
{
  sensorLeiste.read(helligkeiten);
  for (int i = 0; i < SENSOR_LEISTE_ANZAHL_SENSOREN; i++)
  {
    Serial.print(String(helligkeiten[i]) + '\t'); // alles in eine Zeile
  }
  Serial.println(); // neue Zeile beginnen
}

String calculateReflection() //read reflection and return processed result
{
  read_reflectionandprint();
  if ((helligkeiten[0] >= reflectionBlackThreshold) && (helligkeiten[5] >= reflectionBlackThreshold))
  {
    return "frontalLine";
  }
  else if ((helligkeiten[0] >= reflectionBlackThreshold && helligkeiten[1] >= reflectionBlackThreshold && helligkeiten[2] >= reflectionBlackThreshold) || (helligkeiten[3] >= reflectionBlackThreshold && helligkeiten[4] >= reflectionBlackThreshold && helligkeiten[5] >= reflectionBlackThreshold))
  {
    return "sideLine";
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
