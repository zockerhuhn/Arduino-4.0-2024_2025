void stop()
{
  motors.setSpeeds(0, 0);
}

void straight(float factor = 1) //drive straight
{
  if (digitalRead(calibrationPin)) {
    return;
  }
  motors.flipLeftMotor(false);
  motors.flipRightMotor(true);
  motors.setSpeeds((int)(42 * factor),(int)(50 * factor)); //prevent motor drifting
}

void left(int turnBy=0) //turn left
{
  if (digitalRead(calibrationPin)) {
    return;
  }
  ReadDirection();
  int initialDirection = direction;
  motors.flipLeftMotor(true);
  motors.flipRightMotor(true);
  motors.setSpeeds(70, 75);
  if (turnBy!=0) {
    while ((((initialDirection - turnBy) + 360) % 360) != direction) {
      delay(10);
      ReadDirection();
    }
    stop();
  }
}

void right(int turnBy=0) //turn right
{
  if (digitalRead(calibrationPin)) {
    return;
  }
  ReadDirection();
  int initialDirection = direction;
  motors.flipLeftMotor(false);
  motors.flipRightMotor(false);
  motors.setSpeeds(70, 75);
  if (turnBy != 0) {
    while (((initialDirection + turnBy) % 360) != direction) {
      delay(10);
      ReadDirection();
    }
    stop();
  }
}

void straight_left() //drive straight but pull left
{
  if (digitalRead(calibrationPin)) {
    return;
  }
  motors.flipLeftMotor(true);
  motors.flipRightMotor(true);
  motors.setSpeeds(30, 80);
}

void straight_right() //drive straight but pull right
{
  if (digitalRead(calibrationPin)) {
    return;
  }
  motors.flipLeftMotor(false);
  motors.flipRightMotor(false);
  motors.setSpeeds(80, 30);
}


void left_to_line() {
  // going left until it finds a line  
  if (digitalRead(calibrationPin)) {
    return;
  }
  ReadDirection();
  int initialDirection = direction;
  motors.flipLeftMotor(true);
  motors.flipRightMotor(true);
  motors.setSpeeds(70, 75);
  int turnBy = 330;
  while ((calculatedReflection = calculateReflection()) != "normalLine") {
    delay(10);
    ReadDirection();
    if (calculatedReflection == "leftLine") {
      straight_left();
      break;
    } else if (calculatedReflection == "rightLine") {
      straight_right();
      break;
    }
    if (((initialDirection + turnBy) % 360) != direction) {
      break;
    }
  }
}

void right_to_line() {
  // going right until it finds a line  
  if (digitalRead(calibrationPin)) {
    return;
  }
  ReadDirection();
  int initialDirection = direction;
  motors.flipLeftMotor(false);
  motors.flipRightMotor(false);
  motors.setSpeeds(70, 75);
  int turnBy = 330;
  while ((calculatedReflection = calculateReflection()) != "normalLine") {
    delay(10);
    ReadDirection();
    if (calculatedReflection == "leftLine") {
      straight_left();
      break;
    } else if (calculatedReflection == "rightLine") {
      straight_right();
      break;
    }
    if (((initialDirection + turnBy) % 360) != direction) {
      break;
    }
  }
}