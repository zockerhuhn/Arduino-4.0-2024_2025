void opfer() {
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);
  if (digitalRead(motorPin)) {
    stop();
    return;
  }
  stop();
  readDistance();
  //turn to a direction where distance is bigger than a set threshold to avoid facing the side wall in the beginning
  if (!StraightenByWall()) {
      // turn left or right depending where the wall is, this case is very rare if not even impossible
  }
  while (readDistance() <= obstacle_threshold) {
      straight();
      delay(1000);
      StraightenByWall();
  }
}

bool StraightenByWall() {
    int turnCounter;
    int wallScan[21];
    left(11);
    for (turnCounter = 0; turnCounter <= 20; turnCounter++) {
        stop();
        right(1);
        delay(100);
        // read distance
        wallScan[turnCounter] = distance;
    }
    // get max of wallScan if not over certain threshold bigger than average
    int biggest = currentDistance;
    for (int i = 0; i<21; i++) {
        if (0 < WallScan[i] - biggest < WallScanThreshold) {biggest = i;}
    }
    if (biggest == 0) {
        left(10);
        return false;
    }
    left(20);
    right(biggest);
    return true;

