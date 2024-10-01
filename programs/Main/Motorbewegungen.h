void stop()
{
  motors.setSpeeds(0, 0);
}

void straight() //drive straight
{
  if (digitalRead(motorpin)) {
    return;
  }
  motors.flipLeftMotor(false);
  motors.flipRightMotor(true);
  motors.setSpeeds(42, 50); //prevent motor drifting
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
    while (((initialDirection+turnBy)%360)!=direction) {
      delay(10);
      ReadDirection();
    }
    stop();
  } else if (turnBy!=0) {
    while (direction!=turnBy) {
      delay(10)
      ReadDirection()
    }
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
  if (!turnToExact && turnBy!= 0) {
    ReadDirection();
    int initialDirection = direction;
    while (((initialDirection-turnBy)%360)!=direction) {
      delay(10);
      ReadDirection();
    }
    stop();
  } else if (turnBy!=0){
    while (direction!=turnBy) {
      delay(10)
      ReadDirection()
    }
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

#include <iostream>
#include <cmath>
using namespace std;

// Function to calculate the minimum angular distance between two angles
int circularDistance(int a, int b) {
    int diff = abs(a - b);
    return min(diff, 360 - diff);  // Account for the circular nature
}

// Function to find the closest number in the array to the target number
int findClosestNumber(int arr[], int size, int target) {
    int closest = arr[0];
    int minDistance = circularDistance(arr[0], target);
    
    for (int i = 1; i < size; ++i) {
        int dist = circularDistance(arr[i], target);
        if (dist < minDistance) {
            minDistance = dist;
            closest = arr[i];
        }
    }
    
    return closest;
}

int main() {
    int arr[] = {0, 90, 180, 270};  // The array of numbers
    int target;
    
    cout << "Enter a number between 0 and 360: ";
    cin >> target;

    // Ensure target is between 0 and 359
    target = target % 360;

    int closest = findClosestNumber(arr, 4, target);
    cout << "The closest number in the array is: " << closest << endl;
    
    return 0;
}


void straighten()
{
  ReadDirection();
  uint16_t closest = findClosest(calibrateddirection, 4, direction);
  if 
}
