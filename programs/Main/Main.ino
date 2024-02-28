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

#include "includes.h" //all libraries
#include "variables.h" //all declarations and variables

void setup()
{
  delay(300);
  pinMode(motorpin, INPUT_PULLDOWN); //define pinmode for switch on the side of bot
  pinMode(killswitch, INPUT); //define pinmode for killswitch
  Serial.begin(115200);
  // I2C Bus 1x für alle Bus-Teilnehmer initialisieren (sonst crasht das Betriebssystem)
  Wire.begin(); // Bus I2C0
  Wire.setClock(1000000); // 1MHz Kommunikationsgeschwindigkeit
  Wire1.begin(); // Bus I2C1
  //  hier den zu nutzenden I2C Bus einstellen:
  if (!rgbSensor.begin(TCS34725_ADDRESS, &Wire)) //test colorsensor 1
  {
    delay(10000); // damit wir Zeit haben den Serial Monitor zu öffnen nach dem Upload
    Serial.println("RGB Farbsensor Verdrahtung prüfen!");
  }
  Serial.println("Initialisierung Farbe 1 abgeschlossen");
  if (!rgbSensor2.begin(TCS34725_ADDRESS, &Wire1)) //test colorsensor 2
  {
    delay(10000); // damit wir Zeit haben den Serial Monitor zu öffnen nach dem Upload
    Serial.println("RGB Farbsensor Verdrahtung prüfen!");
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

#include "Kalibrierung.h" //calibration values for reflectionsensor and colorsensors
#include "Motorbewegungen.h" //predefined motor commands
#include "Farbauslese.h" //commands for reading and processing colorsensors
#include "Reflektionsauslese.h" //commands for reading and processing reflectionsensor
#include "doppelschwarz.h" //command for handling crosssections

void loop()
{
  if (digitalRead(killswitch)){ //kill the bot if killswitch is pressed
    motors.setSpeeds(0,0);
    while(true){
      delay(1);
    }
  }
  calculatedReflection = calculateReflection(); //read the reflectionsensor and save the result in a variable to avoid changing values while processing 
  if (calculatedReflection == "frontalLine") //detected crosssection
  {
    doppelschwarz();
  }
  else if (calculatedReflection == "normalLine") //detected normal line
  {
    Serial.print("\n");
    Serial.print("Linie");
    straight();
  }
  else if (calculatedReflection == "leftLine") //detected a slight left line
  {
    Serial.print("\n");
    Serial.print("links");
    straight_left();
  }
  else if (calculatedReflection == "rightLine") //detected a slight right line
  {
    Serial.print("\n");
    Serial.print("rechts");
    straight_right();
  }
  else if (calculatedReflection == "hardleftLine") //detected a hard left line
  {
    Serial.print("\n");
    Serial.print("links");
    left();
  }
  else if (calculatedReflection == "hardrightLine") //detected a hard right line
  {
    Serial.print("\n");
    Serial.print("rechts");
    right();
  }
  else if (calculatedReflection == "noLine") //no line detected 
  {
    Serial.print("\n");
    Serial.print("keine Linie...");
    straight();
  }
  delay(10); //don't max out processor
}
