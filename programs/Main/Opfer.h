
void StraightenByWall() {
  digitalWrite(LEDR, LOW);
  left(30);
  delay(1000);
  right();
  readDistance();
  int smallest = INT_MAX;
  while ((readDistance() - smallest) < 20) {
    if ((distance_val - smallest) > 20) {
      Serial.println("hmmmmmmmmmmmm");
      break;
    }
    smallest = readDistance();
    delay(100);
  }
  stop();
  digitalWrite(LEDR, HIGH);
}

void opfer() {
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);
  if (digitalRead(motorPin)) {
    stop();
    return;
  }
  stop();
  
  for (int i = 0; i < 5; i++) readDistance();
  
  while (distance_val >= opfer_wall_threshold) {
    StraightenByWall();
    straight(1.8);
    for (int i = 0; i < 5; i++) readDistance();
  }
}
