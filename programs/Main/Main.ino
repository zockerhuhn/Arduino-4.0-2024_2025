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
  pinMode(motorpin, INPUT_PULLDOWN); // define pinmode for switch on the side of the bot
  pinMode(kalibrierung, INPUT);      // define pinmode for calibration button
  Serial.begin(115200);
  // I2C Bus 1x für alle Bus-Teilnehmer initialisieren (sonst crasht das Betriebssystem)
  Wire.begin();           // Bus I2C0
  Wire.setClock(1000000); // 1MHz Kommunikationsgeschwindigkeit
  Wire1.begin();          // Bus I2C1

  // ABSTANDSSENSOR-INITIALISIEREN
  if(debT) {Chrono keineNeuenDatenStoppuhr = Chrono(Chrono::MILLIS, false);}; // Uhr noch nicht gestartet 
  Serial.println("Initialisierung des 64-Kanal ToF kann bis zu 10 Sekunden dauern...");
  // hier den zu nutzenden I2C Bus und die zu nutzende I2C Adresse eintragen:
  if (!abstandsSensor.begin(NEUE_ABSTANDSADDRESSE, Wire)) {
      delay(10000); // damit wir Zeit haben den Serial Monitor zu öffnen nach dem Upload
      Serial.println("ToF64 Verdrahtung prüfen! Roboter aus- und einschalten! Programm Ende.");
      while (1);
  }
  if (!abstandsSensor.setResolution(einstellungen.aufloesung) ||
      !abstandsSensor.setRangingFrequency(einstellungen.maxMessfrequenz)) {  // siehe oben
          delay(10000); // damit wir Zeit haben den Serial Monitor zu öffnen nach dem Upload
          Serial.println("ToF64 Auflösung oder Messfrequenz konnte nicht geändert werden! Programm Ende.");
          while (1);
  }
  abstandsSensor.startRanging();
  if(debT){keineNeuenDatenStoppuhr.start();}
  Serial.println("Initialisierung Abstandssensor abgeschlossen");
  if (!rgbSensor.begin(TCS34725_ADDRESS, &Wire))
  {
    delay(10000); // damit wir Zeit haben den Serial Monitor zu öffnen nach dem Upload
    Serial.println("RGB Farbsensor Verdrahtung prüfen!");
  }
  Serial.println("Initialisierung Farbe 1 abgeschlossen");
  if (!rgbSensor2.begin(TCS34725_ADDRESS, &Wire1)) // test colorsensor 2
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

#include "Reflektionsauslese.h" //commands for reading and processing reflectionsensor
#include "Motorbewegungen.h"    //predefined motor movements
#include "Farbauslese.h"        //commands for reading and processing colorsensors
#include "doppelschwarz.h"      //command for handling crosssections
#include "Opfer.h"              //Du Opfer

#include "Abstand.h"            // Abstand, noch nicht einsortiert zwischen die restlichen includes

int x = 0;
int y = 0;

void loop()
{
  if (y >= 70)
  {
    opfer();
    Serial.println("opfer");
    y = 0;
  }
  if (digitalRead(motorpin))
  {
    stop();
    for (int i = 0; i < 5; i++)
    { // 5x blinken (AN/AUS):
      digitalWrite(LED_BUILTIN, HIGH);
      delay(250);
      digitalWrite(LED_BUILTIN, LOW);
      delay(250);
    }
    readColor();
    readColor2();
    colorBrightMaxThreshold = max(helligkeit, helligkeit2) + 1500;
    colorBrightMinThreshold = min(helligkeit, helligkeit2) - 1500;
    // 5x blinken (AN/AUS):
    delay(1000);
    for (int i = 0; i < 5; i++)
    {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(250);
      digitalWrite(LED_BUILTIN, LOW);
      delay(250);
    }
  }
  if (x == 2)
  {
    x = 1;
    readColor();
    readColor2();
    while ((rot-250 >= gruen || rot-250 >= blau || rot2-250 >= gruen2 || rot2-250 >= gruen2) && (helligkeit <= colorBrightMaxThreshold || helligkeit2 <= colorBrightMaxThreshold))
    {
      readColor();
      readColor2();
      stop();
      Serial.println("red");
    }
  }
  calculatedReflection = calculateReflection(); // read the reflectionsensor and save the result in a variable to avoid changing values while processing
  if (calculatedReflection == "frontalLine")    // detected crosssection
  {
    doppelschwarz(true);
    y = 0;
  }
  else if (calculatedReflection == "sideLine")
  {
    doppelschwarz(false);
    y = 0;
  }
  else if (calculatedReflection == "normalLine") // detected normal line
  {
    Serial.print("\n");
    Serial.print("Linie");
    straight();
    y = 0;
  }
  else if (calculatedReflection == "leftLine") // detected a slight left line
  {
    Serial.print("\n");
    Serial.print("links");
    straight_left();
    y = 0;
  }
  else if (calculatedReflection == "rightLine") // detected a slight right line
  {
    Serial.print("\n");
    Serial.print("rechts");
    straight_right();
    y = 0;
  }
  else if (calculatedReflection == "hardleftLine") // detected a hard left line
  {
    Serial.print("\n");
    Serial.print("links");
    left();
    y = 0;
  }
  else if (calculatedReflection == "hardrightLine") // detected a hard right line
  {
    Serial.print("\n");
    Serial.print("rechts");
    right();
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