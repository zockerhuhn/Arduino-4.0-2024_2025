void straight() //drive straight
{
  if (digitalRead(motorpin)) {
    return;
  }
  motors.flipLeftMotor(false);
  motors.flipRightMotor(true);
  motors.setSpeeds(50, 55); //prevent motor drifting
}

void left() //turn left
{
  if (digitalRead(motorpin)) {
    return;
  }
  motors.flipLeftMotor(true);
  motors.flipRightMotor(true);
  motors.setSpeeds(70, 75);
}

void right() //turn right
{
  if (digitalRead(motorpin)) {
    return;
  }
  motors.flipLeftMotor(false);
  motors.flipRightMotor(false);
  motors.setSpeeds(70, 75);
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
  motors.setSpeeds(30, 70);
}

void straight_right() //drive straight but pull right
{
  if (digitalRead(motorpin)) {
    return;
  }
  motors.flipLeftMotor(false);
  motors.flipRightMotor(true);
  motors.setSpeeds(70, 30);
}
