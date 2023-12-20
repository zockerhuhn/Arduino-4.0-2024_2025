void straight()
{
  motors.flipLeftMotor(false);
  motors.flipRightMotor(true);
  motors.setSpeeds(45, 30);
}

void left()
{
  motors.flipLeftMotor(true);
  motors.flipRightMotor(true);
  motors.setSpeeds(45, 30);
}

void right()
{
  motors.flipLeftMotor(false);
  motors.flipRightMotor(false);
  motors.setSpeeds(45, 30);
}

void turn()
{
  delay(1); // placeholder
}

void straight_left()
{
  motors.flipLeftMotor(false);
  motors.flipRightMotor(true);
  motors.setSpeeds(10, 30);
}

void straight_right()
{
  motors.flipLeftMotor(false);
  motors.flipRightMotor(true);
  motors.setSpeeds(45, 10);
}