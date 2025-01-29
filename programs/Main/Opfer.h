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