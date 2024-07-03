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
  motors.flipLeftMotor(false);
  motors.flipRightMotor(false);
  motors.setSpeeds(70, 75);
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
