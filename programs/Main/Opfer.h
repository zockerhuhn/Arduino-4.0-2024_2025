
bool StraightenByWall() {
    int turnCounter;
    int wallScan[21];
    left(11);
    for (turnCounter = 0; turnCounter <= 20; turnCounter++) {
        stop();
        right(1);
        delay(100);
        // read distance
        wallScan[turnCounter] = readDistance();
    }
    // get max of wallScan if not over certain threshold bigger than average
    int biggest = 0;
    for (int i = 0; i<21; i++) {
        if (0 < wallScan[i] - wallScan[biggest] && wallScan[i] - wallScan[biggest] < wallscan_threshold) {biggest = i;}
    }
    if (biggest == 0) {
        left(10);
        return false;
    }
    left(20);
    right(biggest);
    return true;
}

void opfer() {
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);
  if (digitalRead(motorPin)) {
    stop();
    return;
  }
  stop();  //turn to a direction where distance is bigger than a set threshold to avoid facing the side wall in the beginning
  if (!StraightenByWall()) {
    digitalWrite(LEDR, LOW);
    digitalWrite(LEDG, LOW);
    digitalWrite(LEDB, LOW);
      // turn left or right depending where the wall is, this case is very rare if not even impossible
  }
  while (readDistance() <= obstacle_threshold) {
    straight();
    delay(1000);
    StraightenByWall();
  }
}
