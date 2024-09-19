void stop()
{
  motors.setSpeeds(0, 0);
}

void straight() //drive straight
{
  if (digitalRead(motorpin)) {
    return;
  }
  motors.flipLeftMotor(false);
  motors.flipRightMotor(true);
  motors.setSpeeds(42, 50); //prevent motor drifting
}

void left(int turnBy=0) //turn left
{
  if (digitalRead(motorpin)) {
    return;
  }
  motors.flipLeftMotor(true);
  motors.flipRightMotor(true);
  motors.setSpeeds(70, 75);
  if (turnBy!=0) {
    ReadDirection();
    int initialDirection = direction;
    while (((initialDirection+turnBy)%360)!=direction) {
      delay(10);
      ReadDirection();
    }
    stop();
  }
}

void right(int turnBy=0) //turn right
{
  if (digitalRead(motorpin)) {
    return;
  }
  motors.flipLeftMotor(false);
  motors.flipRightMotor(false);
  motors.setSpeeds(70, 75);
  if (turnBy!=0) {
    ReadDirection();
    int initialDirection = direction;
    while (((initialDirection-turnBy)%360)!=direction) {
      delay(10);
      ReadDirection();
    }
    stop();
  }
}

void turn() //turn around
{
  if (digitalRead(motorpin)) return;
  motors.flipLeftMotor(false);
  motors.flipRightMotor(false);
  motors.setSpeeds(63, 75);
  delay(4000);
}

void straight_left() //drive straight but pull left
{
  if (digitalRead(motorpin)) {
    return;
  }
  motors.flipLeftMotor(false);
  motors.flipRightMotor(true);
  motors.setSpeeds(30, 80);
}

void straight_right() //drive straight but pull right
{
  if (digitalRead(motorpin)) {
    return;
  }
  motors.flipLeftMotor(false);
  motors.flipRightMotor(true);
  motors.setSpeeds(80, 30);
}

int findClosest(uint16_t arr[], int size, uint16_t target) {
  uint16_t closest = arr[0];
  uint16_t minDiff = std::abs(target - closest);

  for (int i = 0; i < size; i++) {
    uint16_t diff = std::abs(target - closest);

    if (diff < minDiff) {
      minDiff = diff;
      closest = arr[i];
    }
  }
  return closest;
}

void straighten()
{
  ReadDirection();
  uint16_t closest = findClosest(calibrateddirection, 4, direction);
  if (0 <= closest - direction <= 45) {
    left(closest-direction);
  }
  else {
    if (0 >= closest - direction >= -45 ) {
      right(abs((closest-direction)));
    } else if (closest - direction < -45) {

    }
  }
}