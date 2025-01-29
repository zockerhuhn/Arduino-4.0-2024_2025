#pragma once

#include "Distance.h"

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
  stop();
  if (digitalRead(motorPin)) {
    return;
  }
  readDirection();
  int initialDirection = direction;
  motors.flipLeftMotor(true);
  motors.flipRightMotor(true);
  motors.setSpeeds(70, 75);
  if (turnBy!=0) {
    while ((((initialDirection - turnBy) + 360) % 360) != direction) {
      delay(1);
      readDirection();
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
  stop();
  if (digitalRead(motorPin)) {
    return;
  }
  readDirection();
  int initialDirection = direction;
  motors.flipLeftMotor(false);
  motors.flipRightMotor(false);
  motors.setSpeeds(70, 75);
  if (turnBy != 0) {
    while (((initialDirection + turnBy) % 360) != direction) {
      delay(1);
      readDirection();
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
  readDirection();
  int initialDirection = direction;
  motors.flipLeftMotor(true);
  motors.flipRightMotor(true);
  motors.setSpeeds(70, 75);
  int turnBy = 330;
  while ((calculatedReflection = calculateReflection()) == "noLine") {
    delay(10);
    readDirection();
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
  readDirection();
  int initialDirection = direction;
  right();
  while ((calculatedReflection = calculateReflection()) == "noLine") {
    delay(10);
    readDirection();
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
  digitalWrite(LED_BUILTIN, HIGH);
  if (digitalRead(motorPin)) {
    stop();
    for (int i = 0; i < 5; i++) distance_array[i] = 65535;
    return;
    }

  straight(-1);
  while (distance_val < 90) {
    readDistance();
    delay(10);
  }
  stop();


  right();
  if (digitalRead(motorPin)) {
    stop();
    for (int i = 0; i < 5; i++) distance_array[i] = 65535;
    return;
  }
  delay(2000);

  straight();
  if (digitalRead(motorPin)) {
    stop();
    for (int i = 0; i < 5; i++) distance_array[i] = 65535;
    return;
  }
  delay(4000);
  

  left();
  if (digitalRead(motorPin)) {
    stop();
    for (int i = 0; i < 5; i++) distance_array[i] = 65535;
    return;
  }
  delay(2000);

  straight();

  if (digitalRead(motorPin)) {
    stop();
    for (int i = 0; i < 5; i++) distance_array[i] = 65535;
    return;
  }
  delay(6500);
  

  left();
  if (digitalRead(motorPin)) {
    stop();
    for (int i = 0; i < 5; i++) distance_array[i] = 65535;
    return;
  }
  delay(2000);

  straight();

  if (digitalRead(motorPin)) {
    stop();
    for (int i = 0; i < 5; i++) distance_array[i] = 65535;
    return;
  }

  delay(1200);
  while ((calculatedReflection = calculateReflection()) == "noLine") {
    if (digitalRead(motorPin)) {
      stop();
      for (int i = 0; i < 5; i++) distance_array[i] = 65535;
      return;
    }
  }

  if (digitalRead(motorPin)) {
    stop();
    for (int i = 0; i < 5; i++) distance_array[i] = 65535;
    return;
  }
  delay(1500);

  if (digitalRead(motorPin)) {
    stop();
    for (int i = 0; i < 5; i++) distance_array[i] = 65535;
    return;
  }
  right_to_line(180);

  for (int i = 0; i < 5; i++) distance_array[i] = 65535;
  digitalWrite(LED_BUILTIN, LOW);
}