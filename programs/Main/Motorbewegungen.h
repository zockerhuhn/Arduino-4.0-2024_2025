void straight()
{
  motors.flipLeftMotor(false);
  motors.flipRightMotor(true);
  motors.setSpeeds(60, 48);
}

void left()
{
  motors.flipLeftMotor(true);
  motors.flipRightMotor(true);
  motors.setSpeeds(60, 48);
}

void right()
{
  motors.flipLeftMotor(false);
  motors.flipRightMotor(false);
  motors.setSpeeds(60, 48);
}

void turn()
{
  delay(1); // placeholder
}
