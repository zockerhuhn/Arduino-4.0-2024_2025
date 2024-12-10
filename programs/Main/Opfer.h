void Opfer() {
    stop();
    ReadDirection();
}

bool StraightenByWall() {
    int turnCounter;
    int wallScan[21];
    left(12);
    for (turnCounter = 0; turnCounter <= 20; turnCounter++) {
        stop();
        right(1);
        delay(100);
        // read distance
        wallScan[turnCounter] = distance;
    }
    // get max of wallScan if not over certain threshold bigger than average
    left(21);
    right(/* max index of wallScan*/);
    return true;
}