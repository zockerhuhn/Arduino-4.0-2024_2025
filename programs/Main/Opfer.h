void Opfer() {
    stop();
    currentDistance /*= read distance*/;
    //turn to a direction where distance is bigger than a set threshold to avoid facing the side wall in the beginning
    if (!StraightenByWall()) {
        // turn left or right depending where the wall is, this case is very rare if not even impossible
    }
    while ( /*distance is smaller than threshold to determine if too close to wall*/) {
        straight();
        delay(1000);
        StraightenByWall();
    }
}

bool StraightenByWall() {
    int turnCounter;
    int wallScan[21];
    left(11);
    for (turnCounter = 0; turnCounter <= 20; turnCounter++) {
        stop();
        right(1);
        delay(100);
        // read distance
        wallScan[turnCounter] = distance;
    }
    // get max of wallScan if not over certain threshold bigger than average
    int biggest = currentDistance;
    for (int i = 0; i<21; i++) {
        if (0 < WallScan[i] - biggest < WallScanThreshold) {biggest = i;}
    }
    if (biggest == 0) {
        left(10);
        return false;
    }
    left(20);
    right(biggest);
    return true;
#pragma once

#include "Distance.h"

void opfer() {
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);
  for (int j = 0; j < 4; j++) {
    if (digitalRead(motorPin)) {
      stop();
      return;
    }

    for (int i = 0; i < 5; i++) readDistance();


    if (digitalRead(motorPin)) {
      stop();
      return;
    }
  
    while (distance_val > 150) {

      straight(3);
      if (digitalRead(motorPin)) {
        stop();
        return;
      }

      // While going straight, check if something is close
      for (int i = 0; i < 5; i++) readDistance();
      for (int i = 0; i < 40 && distance_val > 200; i++, readDistance()) {
        delay(50);
        if ((calculatedReflection = calculateReflection()) != "noLine") return;
      }

      stop();

      left(90);

      for (int i = 0; i < 5; i++) readDistance();
      if (distance_val > 800) return; // if the opening is there

      straight(2.5);
      while (distance_val > 100) {
        readDistance();
      } 
      stop();
      straight(-1);
      while (distance_val < 80) {
        readDistance();
      } 
      stop();

      right();
      if (digitalRead(motorPin)) {
        stop();
        return;
      }
      delay(1880);
      stop();
      for (int i = 0; i < 5; i++) readDistance();
    }

    for (int i = 0; i < 5; i++) readDistance();
    if (distance_val > 800) return;

    straight(2.5);
    while (distance_val > 100) {
      readDistance();
    } 
    stop();
    straight(-1);
    while (distance_val < 80) {
      readDistance();
    } 
    stop();

    right();
    if (digitalRead(motorPin)) {
      stop();
      return;
    }
    delay(1880);
  }

  return;
}