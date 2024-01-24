void straight()
{
  motors.flipLeftMotor(false);
  motors.flipRightMotor(true);
  motors.setSpeeds(70, 75);
}

void left()
{
  motors.flipLeftMotor(true);
  motors.flipRightMotor(true);
  motors.setSpeeds(70, 75);
}

void right()
{
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
  motors.flipLeftMotor(false);
  motors.flipRightMotor(true);
  motors.setSpeeds(30, 70);
}

void straight_right()
{
  motors.flipLeftMotor(false);
  motors.flipRightMotor(true);
  motors.setSpeeds(70, 30);
}
