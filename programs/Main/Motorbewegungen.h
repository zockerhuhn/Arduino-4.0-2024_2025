void straight()
{
  motors.flipLeftMotor(false);
  motors.flipRightMotor(true);
  motors.setSpeeds(70, 60);
}

void left()
{
  motors.flipLeftMotor(true);
  motors.flipRightMotor(true);
  motors.setSpeeds(70, 60);
}

void right()
{
  motors.flipLeftMotor(false);
  motors.flipRightMotor(false);
  motors.setSpeeds(70, 60);
}

void turn()
{
  delay(1); // placeholder
}

void straight_left()
{
  motors.flipLeftMotor(true);
  motors.flipRightMotor(false);
  motors.setSpeeds(70, 30);
}

void straight_right()
{
  motors.flipLeftMotor(true);
  motors.flipRightMotor(false);
  motors.setSpeeds(30, 70);
}