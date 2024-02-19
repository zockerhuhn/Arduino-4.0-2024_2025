void straight()
{
  // if (digitalRead(motorpin)) {
  //   return;
  // }
  motors.flipLeftMotor(false);
  motors.flipRightMotor(true);
  motors.setSpeeds(50, 55);
}

void left()
{
  // if (digitalRead(motorpin)) {
  //   return;
  // }
  motors.flipLeftMotor(true);
  motors.flipRightMotor(true);
  motors.setSpeeds(70, 75);
}

void right()
{
  // if (digitalRead(motorpin)) {
  //   return;
  // }
  motors.flipLeftMotor(false);
  motors.flipRightMotor(false);
  motors.setSpeeds(70, 75);
}

void turn()
{
  delay(1); // placeholder
}

void straight_left()
{
  // if (digitalRead(motorpin)) {
  //   return;
  // }
  motors.flipLeftMotor(false);
  motors.flipRightMotor(true);
  motors.setSpeeds(30, 70);
}

void straight_right()
{
  // if (digitalRead(motorpin)) {
  //   return;
  // }
  motors.flipLeftMotor(false);
  motors.flipRightMotor(true);
  motors.setSpeeds(70, 30);
}
