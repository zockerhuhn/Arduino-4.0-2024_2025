void stop()
{
  motors.setSpeeds(0, 0);
}

void straight(float factor = 1) //drive straight
{
  if (digitalRead(motorPin)) {
    stop();
    return;
  }
  motors.flipLeftMotor(false);
  motors.flipRightMotor(true);
  motors.setSpeeds((int)(42 * factor),(int)(50 * factor)); //prevent motor drifting
}

void left(int turnBy=0) //turn left
{
  if (digitalRead(motorPin)) {
    stop();
    return;
  }
  ReadDirection();
  int initialDirection = direction;
  motors.flipLeftMotor(true);
  motors.flipRightMotor(true);
  motors.setSpeeds(70, 75);
  if (turnBy!=0) {
    while ((((initialDirection - turnBy) + 360) % 360) != direction) {
      delay(1);
      ReadDirection();
      if (digitalRead(motorPin)) {
        stop();
        return;
      }
    }
    stop();
  }
}

void right(int turnBy=0) //turn right
{
  if (digitalRead(motorPin)) {
    stop();
    return;
  }
  ReadDirection();
  int initialDirection = direction;
  motors.flipLeftMotor(false);
  motors.flipRightMotor(false);
  motors.setSpeeds(70, 75);
  if (turnBy != 0) {
    while (((initialDirection + turnBy) % 360) != direction) {
      delay(1);
      ReadDirection();
      if (digitalRead(motorPin)) {
        stop();
        return;
      }
    }
    stop();
  }
}

void straight_left() //drive straight but pull left
{
  if (digitalRead(motorPin)) {
    stop();
    return;
  }
  motors.flipLeftMotor(true);
  motors.flipRightMotor(true);
  motors.setSpeeds(30, 80);
}

void straight_right() //drive straight but pull right
{
  if (digitalRead(motorPin)) {
    stop();
    return;
  }
  motors.flipLeftMotor(false);
  motors.flipRightMotor(false);
  motors.setSpeeds(80, 30);
}


void left_to_line() {
  // going left until it finds a line  
  if (digitalRead(motorPin)) {
    stop();
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
    if (((initialDirection + turnBy) % 360) == direction) {
      break;
    }

    if (digitalRead(motorPin)) {
      stop();
      return;
    }
  }
}

void right_to_line(int turnBy = 330) {
  // going right until it finds a line  
  if (digitalRead(motorPin)) {
    stop();
    return;
  }
  ReadDirection();
  int initialDirection = direction;
  right();
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
    if (((initialDirection + turnBy) % 360) == direction) {
      break;
    }

    if (digitalRead(motorPin)) {
      stop();
      return;
    }
  }
}

void abstand_umfahren() {
  straight(-1);
  delay(500);
  right(90);
  straight();
  delay(3200);
  left(86);
  straight();
  delay(7000);
  left(65);
  straight();
  delay(3700);
  right_to_line(180);
  for (int i = 0; i < 10; i++) abstandsWerte[i] = 65535;
}