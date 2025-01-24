/** importiert Arduino automatisch, muss man also hier nicht unbedingt auch noch mal importieren: */
#include <Arduino.h>

/**
 * !!! Immer darauf achten, dass unten in der Statusleiste...
 *     ... das richtige Arduino Board eingestellt ist
 *     ... das richtige "Sketch File" ausgewählt ist (das ändert sich nämlich nicht automatisch)
 *     ... die richtige C/C++ Konfiguration eingestellt ist (sonst gibt es noch mehr "rote swiggels")
 *
 * :: Externen RGB Farbsensor auslesen ::
 * :: Serial Plotter ausprobieren ::
 * :: Hauptprogramm-Schleife in Zustände unterteilen ::
 * :: Programm in Funktionen unterteilen ::
 *
 * Hardware-Aufbau:
 * TCS34725:    Arduino Due / Arduino Nano RP2040 Connect:
 *     [LED <-- (kann man optional an irgendeinen Digital-Pin oder den INT Pin vom Sensor anschließen,
 *              um die LED abzuschalten)]
 *     [INT  --> (kann man optional an irgendeinen Digital-Pin anschließen,
 *                wenn man den Sensor effizienter auslesen will)]
 *      SDA <-> SDA
 *      SCL <-- SDA
 *     [3V3 --> (2,3V Spannungsversorgung, falls man sowas irgendwo benötigt)]
 *      GND <-- GND
 *      VIN <-- 3V3
 * Der QTR-6RC ist ein externer Sensor, der an jeden Digital Pin angeschlossen werden kann (Siehe SENSOR_LEISTE_PINS).
 * !!! Beim Arduino Nano RP2040 Connect darf der Sensor nicht an Pins A6 oder A7 angeschlossen werden,
 *     da diese nur Eingänge sind und die Bibliothek den Pin sowohl als Ein- als auch als Ausgang verwendet.
 *----------------------------------------------------------------------------------------------------------------------
 * Der VL53L0X ist ein externer Sensor, kann also entweder
 * - an   Bus I2C0 ("Wire")
 * - oder Bus I2C1 ("Wire1") verbunden werden.
------------------------------------------------------------------------------------------------------------------------
 * Hardware-Aufbau:
 * Arduino Nano RP2040 Connect (!! Arduino Due geht nicht !!)
 * <nichts weiter>
 * Der LSM6DSOX ist immer an Bus I2C0 ("Wire") verbunden. Das kann nicht geändert werden.
 *
 * Der LSM6DSOX Sensor hängt am I2C Bus (SDA/SCL) mit 7-Bit Adresse 0x6A.
 * Die Adresse kann nicht verändert werden. Man kann also keinen anderen Sensor mit derselben Adresse
 * an den selben Bus anschließen.
*/

#include "includes.h"     //all libraries
#include "variables.h"    //all declarations and variables
#include "Kalibrierung.h" //calibration values for reflectionsensor and colorsensors

void setup()
{
  delay(5000);                       // Wichtig für den Abstandssensor
  pinMode(LED_BUILTIN, OUTPUT);      // Pin D13
  pinMode(motorPin, INPUT_PULLDOWN); // define pinmode for switch on the side of the bot
  pinMode(calibrationPin, INPUT);      // define pinmode for calibration button

  // Set the color LEDS as outputs
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);

  // Turn of any "lingering" LEDs
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(LEDR, LOW);
  digitalWrite(LEDG, LOW);
  digitalWrite(LEDB, LOW);

  Serial.begin(115200);
  // I2C Bus 1x für alle Bus-Teilnehmer initialisieren (sonst crasht das Betriebssystem)
  Wire.begin();           // Bus I2C0
  Wire.setClock(1000000); // 1MHz Kommunikationsgeschwindigkeit
  Wire1.begin();          // Bus I2C1

  // REIHENFOLGE:
  /*
   - Abstandssensor
   - Farbsensoren
  */
  
  // ABSTANDSSENSOR-INITIALISIEREN
  Serial.println("Initialisierung des 1-Kanal ToF kann bis zu 10 Sekunden dauern...");
  abstandsSensor.setBus(&Wire);
  abstandsSensor.setAddress(NEUE_ABSTANDSADDRESSE);
  if (!abstandsSensor.init()) {
      delay(5000); // damit wir Zeit haben den Serial Monitor zu öffnen nach dem Upload
      Serial.println("ToF Verdrahtung prüfen! Roboter aus- und einschalten! Programm Ende.");
      while (1);
  }
  // Einstellung: Fehler, wenn der Sensor länger als 500ms lang nicht reagiert
  abstandsSensor.setTimeout(500);
  // Reichweiter vergrößern (macht den Sensor ungenauer)
  abstandsSensor.setSignalRateLimit(0.1);
  abstandsSensor.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  abstandsSensor.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
  // lasse Sensor die ganze Zeit an
  abstandsSensor.startContinuous();

  Serial.println("Initialisierung Abstandssensor abgeschlossen");


  
  if (!rgbSensor.begin(TCS34725_ADDRESS, &Wire))
  {
    digitalWrite(LEDR, HIGH);
    digitalWrite(LEDG, HIGH);
    delay(10000); // damit wir Zeit haben den Serial Monitor zu öffnen nach dem Upload
    Serial.println("RGB 1 (rechts) Farbsensor Verdrahtung prüfen!");
    while (!rgbSensor.begin(TCS34725_ADDRESS, &Wire));
    digitalWrite(LEDR, LOW);
    digitalWrite(LEDG, LOW);
  }
  Serial.println("Initialisierung Farbe 1 abgeschlossen");
  if (!rgbSensor2.begin(TCS34725_ADDRESS, &Wire1)) // test colorsensor 2
  {
    digitalWrite(LEDR, HIGH);
    digitalWrite(LEDB, HIGH);
    delay(10000); // damit wir Zeit haben den Serial Monitor zu öffnen nach dem Upload
    Serial.println("RGB 2 (links) Farbsensor Verdrahtung prüfen!");    
    while (!rgbSensor2.begin(TCS34725_ADDRESS, &Wire1));
    digitalWrite(LEDR, LOW);
    digitalWrite(LEDB, LOW);
  }
  Serial.println("Initialisierung Farbe 2 abgeschlossen");
  sensorLeiste.setTypeRC();
  sensorLeiste.setSensorPins(SENSOR_LEISTE_PINS, SENSOR_LEISTE_ANZAHL_SENSOREN);
  Serial.println("Initialisierung Reflektionssensor abgeschlossen");

  motors.initialize();
  // falls man global die Motor-Drehrichtung ändern möchte:
  motors.flipLeftMotor(false); // nur notwendig, wenn man true reinschreibt
  motors.flipRightMotor(true); // nur notwendig, wenn man true reinschreibt
}

#include "Reflektionsauslese.h" //commands for reading and processing reflectionsensor
#include "Motorbewegungen.h"    //predefined motor movements
#include "Farbauslese.h"        //commands for reading and processing colorsensors
#include "kreuzung.h"      //command for handling crosssections
#include "Opfer.h"              //Du Opfer

#include "Abstand.h"            // Abstand, noch nicht einsortiert zwischen die restlichen includes

int x = 0;
int y = 0;

void loop()
{
  if (y >= 70)
  {
    Serial.println("opfer");
    opfer();
    y = 0;
  }

  // Serial.print(digitalRead(calibrationPin));Serial.print("\t");Serial.print(digitalRead(motorPin));;Serial.print("\n");
  if (digitalRead(motorPin)) {
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(LEDR, LOW);
    digitalWrite(LEDG, LOW);
    digitalWrite(LEDB, LOW);
  }

  if (digitalRead(calibrationPin))
  {
    stop();
    for (int i = 0; i < 5; i++)
    { // 5x blinken (AN/AUS):
      digitalWrite(LED_BUILTIN, HIGH);
      delay(250);
      digitalWrite(LED_BUILTIN, LOW);
      delay(250);
    }
    // Calibrating should word by calculating an average from multiple values
    uint16_t average_r, average_g, average_b, average_c,  average_r2, average_g2, average_b2, average_c2;
    average_r = average_g = average_b = average_c = average_r2 = average_g2 = average_b2 = average_c2 = 0;
    int total_cycles = 10;
    for (int i = 0; i < total_cycles; i++) 
    {
      readColor();
      readColor2();

      average_r += rot;
      average_g += gruen;
      average_b += blau;
      average_c += helligkeit;

      average_r2 += rot2;
      average_g2 += gruen2;
      average_b2 += blau2;
      average_c2 += helligkeit2;
    }
    // calculate average values for both sensors
    average_r /= total_cycles;
    average_g /= total_cycles;
    average_b /= total_cycles;
    average_c /= total_cycles;
    average_r2 /= total_cycles;
    average_g2 /= total_cycles;
    average_b2 /= total_cycles;
    average_c2 /= total_cycles;
    
    // idea: calculate the ratio instead!
    blueGreenThreshold = average_g - average_b - 200;
    blueGreenThreshold2 = average_g2 - average_b2 - 200;
    redGreenThreshold = average_g - average_r - 200;
    redGreenThreshold2 = average_g2 - average_r2 - 200;

    colorBrightMaxThreshold = max(helligkeit, helligkeit2) + 1500;
    colorBrightMinThreshold = min(helligkeit, helligkeit2) - 300;

    // 738 886 767 2399

    Serial.println("Values: " + String(average_r) + " " + String(average_g) + " " + String(average_b)+ " " + String(average_r2) + " " + String(average_g2) + " " + String(average_b2) + " " + String(helligkeit)+ " " + String(helligkeit2));
    Serial.println("Thresholds: " + String(blueGreenThreshold) + " " + String(redGreenThreshold) + " " + String(blueGreenThreshold2) + " " + String(redGreenThreshold2) + " " + String(colorBrightMaxThreshold)+ " " + String(colorBrightMinThreshold));

    // Serial.println("red vals: " + String(rot) + " " + String(gruen) + " " + String(blau) + " " + String(helligkeit) + "\t " + String(rot2) + " " + String(gruen2) + " " + String(blau2) + " " + String(helligkeit2));
    Serial.println(String(calculateColor()) + " " + String(calculateColor2()));
    // 5x blinken (AN/AUS):
    for (int i = 0; i < 5; i++)
    {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(250);
      digitalWrite(LED_BUILTIN, LOW);
      delay(250);
    }

    // ABSTANDSWERTE LOGGEN
    modus = ABSTANDS_WERTE_LOGGEN;
    readDistance();
    werteLoggen();
  }
  
  readColor();
  readColor2();
  while ((2 * (blau + gruen) <= rot + 300 && (2 * (blau2 + gruen2) <= rot2 + 300)) && (helligkeit <= colorBrightMaxThreshold + 800 || helligkeit2 <= colorBrightMaxThreshold + 800)) {
    digitalWrite(LEDR, HIGH);
    stop();
    Serial.println("red"); 
    delay(1000);
    readColor();
    readColor2();
    Serial.print(String(2 * (blau + gruen)) + " " + String(rot + 300) + "\t" + String(2 * (blau2 + gruen2)) + " " + String(rot2 + 300) + "\t" + String(helligkeit) + " " + String(helligkeit2) + " " + String(colorBrightMaxThreshold + 800) + "\n");
    straight(-1); // backwards
    delay(400); // values of delay can be adjusted, but this works pretty good
    stop();
    delay(400);
    straight();
    red_counter++;
    if (red_counter > 3) {
      stop();
      delay(8000); // more than the 5 required seconds
      straight(-1);
      delay(800);
      break;
    }

    if (digitalRead(motorPin)) {
      stop();
      return;
    }
  }

  digitalWrite(LEDR, LOW);
  red_counter = 0;
  // readColor();readColor2();
  // Serial.println(String(calculateColor2()) + " " + String(calculateColor()) + "(" + String(rot2) + " " + String(gruen2) + " " + String(blau2) + " " + String(helligkeit2) + ", " + String(rot) + " " + String(gruen) + " " + String(blau) + " " + String(helligkeit) + ")");
  
  calculatedReflection = calculateReflection(); // read the reflectionsensor and save the result in a variable to avoid changing values while processing
  // Serial.println(calculatedReflection);
  if (calculatedReflection == "frontalLine")    // detected crosssection
  {
    kreuzung(true, 0);
    y = 0;
  }
  else if (calculatedReflection == "sideLeftLine")
  {
    kreuzung(false, -1);
    y = 0;
  }
  else if (calculatedReflection == "sideRightLine")
  {
    kreuzung(false, 1);
    y = 0;
  }
  else if (calculatedReflection == "normalLine") // detected normal line
  {
    straight();
    y = 0;
  }
  else if (calculatedReflection == "leftLine") // detected a slight left line
  {
    straight_left();
    y = 0;
  }
  else if (calculatedReflection == "rightLine") // detected a slight right line
  {
    straight_right();
    y = 0;
  }
  else if (calculatedReflection == "hardleftLine") // detected a hard left line
  {
    left_to_line();
    y = 0;
  }
  else if (calculatedReflection == "hardrightLine") // detected a hard right line
  {
    right_to_line();
    y = 0;
  }
  else if (calculatedReflection == "noLine") // no line detected
  {
    Serial.print("\n");
    Serial.print("keine Linie...");
    straight();
    y++;
  }
  delay(10); // don't max out processor
  x++;
}