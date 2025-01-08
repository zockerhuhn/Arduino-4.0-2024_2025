//#include "Motorbewegungen.h"
//#include "Farbauslese.h"
//#include "Reflektionsauslese.h"

void kreuzung(bool bothSides, int sides /*- 1 is left, 0 is none, 1 is right*/) {
  if (!(digitalRead(calibrationPin))) {
    if (bothSides) { // very probably a crossing where green is

    // drive forward slowly, check for greens
    digitalWrite(LED_BUILTIN, HIGH); // Activate Lamp to see when a Kreuzung is detected

    int green1 = 0; // right
    int green2 = 0; // left

    bool stopping = false;
    int stopping_in = -1;
    straight(0.5);
    while (!(stopping)) {
      if (stopping_in > 0) stopping_in--;
      if (stopping_in == 0) stopping = true;
      readColor2();
      readColor();

      if (calculateColor()) {
        green1 += 1; 
        Serial.print("Found green 1 (right)\t");
        stopping_in = 7;
        digitalWrite(LEDG, LOW);
        delay(50);
        if (green2 >= 4) {
          digitalWrite(LEDR, HIGH);
          digitalWrite(LEDB, LOW);
        }
        else digitalWrite(LEDG, HIGH);
      }
      if (calculateColor2()) {
        green2 += 1;
        Serial.print("Found green 2 (left)\t");
        stopping_in = 7;
        digitalWrite(LEDB, LOW);
        delay(50);
        if (green1 >= 4) {
          digitalWrite(LEDR, HIGH);
          digitalWrite(LEDG, LOW);
        }
        else digitalWrite(LEDB, HIGH);
      }

      calculatedReflection = calculateReflection();

      if (!(calculatedReflection == "frontalLine" || calculatedReflection == "sideLine") && stopping_in < 0) {
        stopping_in = 6;
      }
      
      if (green1 >= 4 && green2 >= 4) {
        stopping = true;
      }

      else if (green1 >= 1 || green2 >= 1) {
        // Stop to indicate that green has been detected
        stop();
        delay(250);
        straight(0.5);
      }

      delay(10);
    }

    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(LEDR, LOW);
    digitalWrite(LEDG, LOW);
    digitalWrite(LEDB, LOW);


    // Handle the recorded greens
    if (green1 >= 4 && green2 >= 4) {
      // Turn
      Serial.print("turn\t");
      right(180);
      delay(600);
    }
    else if (green1 >= 4) {
      Serial.print("right\t");

      // Drive forward for some time to position the geometric centre above the crossing
      straight();
      delay(250);
      right(90);
      delay(300);
      straight(); // then go straight a bit to avoid seeing a crossing again
      delay(800);     
    }
    else if (green2 >= 4) {
      Serial.print("left\t");
      straight();
      delay(250);
      left(90);
      delay(300);
      straight();
      delay(800);
    }
    else { // Did not find any green
      straight();
      delay(500); // adjust that waiting time

      if (calculateReflection() == "noLine") {
        if (sides == -1 || sides == 0) {
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
        else if (sides == 1) {
          // finding line
          right(90);
          
          // going right "forever"    
          motors.flipLeftMotor(true);
          motors.flipRightMotor(true);
          motors.setSpeeds(35, 37.5); // probably accounting for motor deficiencies
          Serial.print("looping\t");
          while (calculateReflection() == "noLine") // MAYBE because it turns left at the start ignore left Lines because these would be the wrong direction (for a kreuzung for example they would be left instead of straight)
          {
            Serial.print("\nsuche...");
          }
        }
        
      }
    }
  }
    else { // hit the kreuzung more from a side
        straight(-1);
        delay(600);
        stop();
        delay(100);
        straight(1);
        delay(1200);
        kreuzung(true, sides);
    }
  }
}
