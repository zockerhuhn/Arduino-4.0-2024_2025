//#include "Motorbewegungen.h"
//#include "Farbauslese.h"
//#include "Reflektionsauslese.h"

void kreuzung(bool bothSides) {
  if (bothSides) { // very probably a crossing where green is

    // drive forward slowly, check for greens
    digitalWrite(LED_BUILTIN, HIGH); // Activate Lamp to see when a Kreuzung is detected
    
    motors.flipLeftMotor(false);
    motors.flipRightMotor(true);
    motors.setSpeeds((int)(42 * 0.5),(int)(50 * 0.5)); // half the default speed
    // ERROR this does not suffice for double green because the kreuzung is detected earlier because "green" is seen as black by reflektion

    bool green1 = false; // right
    bool green2 = false; // left

    bool stopping = false;
    int stopping_in = -1;
    String reflection;
    
    while (!(stopping)) {
      if (stopping_in > 0) stopping_in--;
      if (stopping_in == 0) stopping = true;
      readColor2();
      readColor();

      if (calculateColor() && !green1) {
        green1 = true; 
        Serial.print("Found green 1 (right)\t");
        stopping_in = 3;
        digitalWrite(LED_BUILTIN, LOW);
        delay(20);
        digitalWrite(LED_BUILTIN, HIGH);
        delay(20);
        digitalWrite(LED_BUILTIN, LOW);
      }
      if (calculateColor2() && !green2) {
        green2 = true;
        Serial.print("Found green 2 (left)\t");
        stopping_in = 3;
        digitalWrite(LED_BUILTIN, LOW);
        delay(20);
        digitalWrite(LED_BUILTIN, HIGH);
        delay(20);
        digitalWrite(LED_BUILTIN, LOW);
      }

      reflection = calculateReflection();

      if ((!(reflection == "frontalLine" || reflection == "sideLine")) && stopping_in < 0) {
        stopping_in = 3;
      }
      else {
        // straighten();
      }
      
      if (green1 && green2) {
        stopping = true;
      }

      else if (green1 || green2) {
        // Stop to indicate that green has been detected
        digitalWrite(LED_BUILTIN, LOW);
        stop();
        delay(250);
        digitalWrite(LED_BUILTIN, HIGH);
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

      Serial.println(calculateReflection());
      
    
    }

    else { // Did not find any green
      straight();
      delay(1800); // adjust that waiting time

      if (calculateReflection() == "noLine") {
        // finding line
        left(90);
        
        // going right "forever"    
        motors.flipLeftMotor(false);
        motors.flipRightMotor(false);
        motors.setSpeeds(35, 37.5); // probably accounting for motor deficiencies
        Serial.print("looping\t");
        while (calculateReflection() == "noLine") // MAYBE because it turns left at the start ignore left Lines because these would be the wrong direction (for a kreuzung for example they would be left instead of straight)
        {
          Serial.print("\nsuche...");
        }
      }
    }

  }
  else { // hit the kreuzung more from a side
      // motors.flipLeftMotor(true);
      // motors.flipRightMotor(false);
      // motors.setSpeeds(35, 37.5);
      // delay(200);
      // straighten();
  }
}
