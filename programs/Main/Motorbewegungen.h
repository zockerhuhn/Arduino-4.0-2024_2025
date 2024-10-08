void stop()
{
  motors.setSpeeds(0, 0);
}

void straight(int factor = 1) //drive straight
{
  if (digitalRead(motorpin)) {
    return;
  }
  motors.flipLeftMotor(false);
  motors.flipRightMotor(true);
  motors.setSpeeds((int)(42 * factor),(int)(50 * factor)); //prevent motor drifting
}

void left(int turnBy=0, boolean turnToExact=false) //turn left
{
  if (digitalRead(motorpin)) {
    return;
  }
  motors.flipLeftMotor(true);
  motors.flipRightMotor(true);
  motors.setSpeeds(70, 75);
  if (!turnToExact && turnBy!=0) {
    ReadDirection();
    int initialDirection = direction;
    while ((((initialDirection - turnBy) + 360) % 360) != direction) {
      delay(10);
      ReadDirection();
    }
    stop();
  } else if (turnToExact) {
    while (direction!=turnBy) {
      ReadDirection();
      Serial.println(direction);
    }
    stop();
  }
}

void right(int turnBy=0, boolean turnToExact=false) //turn right
{
  if (digitalRead(motorpin)) {
    return;
  }
  motors.flipLeftMotor(false);
  motors.flipRightMotor(false);
  motors.setSpeeds(70, 75);
  if (!turnToExact && turnBy != 0) {
    ReadDirection();
    int initialDirection = direction;
    while (((initialDirection + turnBy) % 360) != direction) {
      delay(10);
      ReadDirection();
    }
    stop();
  } else if (turnToExact) {
    while (direction!=turnBy) {
      ReadDirection();
      Serial.println(direction);
    }
    stop();
  }
}

void turn() //turn around
{
  if (digitalRead(motorpin)) return;
  motors.flipLeftMotor(false);
  motors.flipRightMotor(false);
  motors.setSpeeds(63, 75);
  delay(4000);
}

void straight_left() //drive straight but pull left
{
  if (digitalRead(motorpin)) {
    return;
  }
  motors.flipLeftMotor(false);
  motors.flipRightMotor(true);
  motors.setSpeeds(30, 80);
}

void straight_right() //drive straight but pull right
{
  if (digitalRead(motorpin)) {
    return;
  }
  motors.flipLeftMotor(false);
  motors.flipRightMotor(true);
  motors.setSpeeds(80, 30);
}

// #include <iostream>
// #include <cmath>
// using namespace std;

// Function to calculate the minimum angular distance between two angles
uint16_t circularDistance(uint16_t a, uint16_t b) {
    uint16_t diff = abs(a - b);
    return min(diff, 360 - diff);  // Account for the circular nature
}

// Function to find the closest number in the array to the target number
uint16_t findClosestNumber(uint16_t arr[], uint16_t size, uint16_t target) {
    uint16_t closest2 = arr[0];
    uint16_t minDistance = circularDistance(arr[0], target);
    
    for (uint16_t i = 1; i < size; ++i) {
        uint16_t dist = circularDistance(arr[i], target);
        if (dist < minDistance) {
            minDistance = dist;
            closest2 = arr[i];
        }
    }
    
    return closest2;
}

void straighten()
{
  ReadDirection();
  uint16_t closest = findClosestNumber(calibrateddirection, 4, direction);
  Serial.println("\nStraighten: " + String(direction) + " " + String(closest) + " " + String(direction - closest));
  if (direction - closest < -45 || (0 < direction - closest && direction - closest < 45)) {
    left(closest,true);
  } else {
    right(closest,true);
  }
}
