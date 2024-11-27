void stop()
{
  motors.setSpeeds(0, 0);
}

void straight(int factor = 1) //drive straight
{
  if (digitalRead(calibrationPin)) {
    return;
  }
  motors.flipLeftMotor(false);
  motors.flipRightMotor(true);
  motors.setSpeeds((int)(42 * factor),(int)(50 * factor)); //prevent motor drifting
}

void left(int turnBy=0, boolean turnToExact=false) //turn left
{
  if (digitalRead(calibrationPin)) {
    return;
  }
  motors.flipLeftMotor(true);
  motors.flipRightMotor(true);
  motors.setSpeeds(70, 75);
  if (!turnToExact && turnBy!=0) {
    ReadDirection();
    int initialDirection = direction;
    while ((((initialDirection - turnBy) + 360) % 360) != direction) {
      delay(10);
      ReadDirection();
    }
    stop();
  } else if (turnToExact) {
    while (direction!=turnBy) {
      ReadDirection();
      Serial.println(direction);
    }
    stop();
  }
}

void right(int turnBy=0, boolean turnToExact=false) //turn right
{
  if (digitalRead(calibrationPin)) {
    return;
  }
  motors.flipLeftMotor(false);
  motors.flipRightMotor(false);
  motors.setSpeeds(70, 75);
  if (!turnToExact && turnBy != 0) {
    ReadDirection();
    int initialDirection = direction;
    while (((initialDirection + turnBy) % 360) != direction) {
      delay(10);
      ReadDirection();
    }
    stop();
  } else if (turnToExact) {
    while (direction!=turnBy) {
      ReadDirection();
      Serial.println(direction);
    }
    stop();
  }
}

void turn() //turn around
{
  if (digitalRead(calibrationPin)) return;
  motors.flipLeftMotor(false);
  motors.flipRightMotor(false);
  motors.setSpeeds(63, 75);
  delay(4000);
}

void straight_left() //drive straight but pull left
{
  if (digitalRead(calibrationPin)) {
    return;
  }
  motors.flipLeftMotor(false);
  motors.flipRightMotor(true);
  motors.setSpeeds(30, 80);
}

void straight_right() //drive straight but pull right
{
  if (digitalRead(calibrationPin)) {
    return;
  }
  motors.flipLeftMotor(false);
  motors.flipRightMotor(true);
  motors.setSpeeds(80, 30);
}