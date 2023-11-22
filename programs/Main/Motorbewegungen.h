void straight()
{
  motoren.flipLeftMotor(true);
  motoren.flipRightMotor(true);
  motoren.setSpeeds(75, 75);
}

void left()
{
  motoren.flipLeftMotor(false);
  motoren.flipRightMotor(true);
  motoren.setSpeeds(75, 75);
}

void right()
{
  motoren.flipLeftMotor(true);
  motoren.flipRightMotor(false);
  motoren.setSpeeds(75, 75);
}

void turn()
{
  delay(1); // placeholder
}
