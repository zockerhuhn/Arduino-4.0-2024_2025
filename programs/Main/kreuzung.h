//#include "Motorbewegungen.h"
//#include "Farbauslese.h"
//#include "Reflektionsauslese.h"

void kreuzung(bool bothSides) {
  if (bothSides) { // very probably a crossing where green is

    // drive forward slowly, check for greens
    digitalWrite(LED_BUILTIN, HIGH); // Activate Lamp to see when a Kreuzung is detected
    straight();
    bool green1 = false; // right
    bool green2 = false; // left

    for (int i = 0; i < 20 /*adjust that value*/; i++) {
      // TODO find out how much distance a kreuzung is and how much time this corresponds to for the motors
      readColor2();
      readColor();

      if (calculateColor() && !green1) {
        green1 = true; 
        Serial.print("Found green 1 (right)\t");
        i = 18; // Let the robot check for the other green just one more time
      }
      if (calculateColor2() && !green2) {
        green2 = true;
        Serial.print("Found green 2 (left)\t");
        i = 18;
      }
      if (green1 && green2) {
        break;
      }
      else if (green1 || green2) {
        // Stop to indicate that green has been detected
        digitalWrite(LED_BUILTIN, LOW);
        stop();
        delay(250);
        digitalWrite(LED_BUILTIN, HIGH);
        straight(0.8); // drive slower to try to detect a potential other green
      }

      delay(10);
    }
    digitalWrite(LED_BUILTIN, LOW);


    // Handle the recorded greens
    if (green1 && green2) {
      // Turn
      Serial.print("turn\t");
      right(180);

      // TODO turn to the nearest direction that is indicated as straight by the compass
    }
    else if (green1) {
      Serial.print("right\t");

      // Drive forward for some time to position the geometric centre above the crossing
      straight();
      delay(600);
      right(90);

    }
    else if (green2) {
      Serial.print("left\t");
      straight();
      delay(600);
      left(90);
    }

    else { // Did not find any green
      // finding line
      left(90);
      
      // going right "forever"    
      motors.flipLeftMotor(false);
      motors.flipRightMotor(false);
      motors.setSpeeds(35, 37.5); // probably accounting for motor deficiencies
      Serial.print("looping\t");
      while (calculateReflection() == "noLine")
      {
        Serial.print("\nsuche...");
      }
    }

  }
  else { // should usually not occur
      // IDEA turn towards the nearest possible direction (got from compass)


  }
}
