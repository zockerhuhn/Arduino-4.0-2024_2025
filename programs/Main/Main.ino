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

// INCLUDES
/** I2C Adresse: 0x6A (7-bit) (unveränderlich) */
#include <Arduino_LSM6DSOX.h>
#include <imuFilter.h>
imuFilter Fusion = imuFilter(); // Objekt, dass Sensordaten fusionieren kann
#include <WiFiNINA.h>


/** der I2C Bus */
#include <Wire.h>
/** I2C Adresse: 0x29 (7-bit) (unveränderlich) */
#include <Adafruit_TCS34725.h>
/** optional: Stoppuhr, um zu Verbindungsverluste zu erkennen */


#include <QTRSensors.h>

#include "ZumoMotors.h"

// VARIABLES


int rgbValues[] = {255, 0, 0}; // 0=Red, 1=Green and 2=Blue
bool imuFusionInitialisiert = false;
double accxalt;
double accyalt;
double acczalt;
// hier speichern wir die Sensorwerte ab:
vec3_t acc = vec3_t(0, 0, 0);  // (x, y, z)
vec3_t gyro = vec3_t(0, 0, 0); // (x, y, z)


int varrechts = 0;
int varlinks = 0;

const uint8_t SENSOR_LEISTE_ANZAHL_SENSOREN = 6;
const uint8_t SENSOR_LEISTE_PINS[] = {10, 11, 12, 9, 8, 7};
// hier speichern wir die 6 Reflektionssensorwerte ab:
uint16_t helligkeiten[SENSOR_LEISTE_ANZAHL_SENSOREN];
QTRSensors sensorLeiste = QTRSensors();
String calculatedReflection;

// hier speichern wir die Farbsensorwerte ab:
// Roh-Werte (Es gibt auch kalibierte Werte, aber die sind sehr langsam auszulesen):
uint16_t rot, gruen, blau, helligkeit;
uint16_t rot2, gruen2, blau2, helligkeit2;
const uint16_t VERBINDUNG_VERLOREN = 0;
uint16_t vorheriges_rot, vorheriges_gruen, vorheriges_blau, vorherige_helligkeit = VERBINDUNG_VERLOREN;
uint16_t vorheriges_rot2, vorheriges_gruen2, vorheriges_blau2, vorherige_helligkeit2 = VERBINDUNG_VERLOREN;
QTRSensors sensorLeiste = QTRSensors();
ZumoMotors motoren;

/** Sensor sehr schnell einstellen (ungenauer):
 *  Gain 4x fand ich am besten, aber dann sind die Werte so stabil,
 *  dass die Fehlerdetektion immer ausgelöst hat (siehe unten "helligkeitStatischStoppuhr.hasPassed"). */
Adafruit_TCS34725 rgbSensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

Adafruit_TCS34725 rgbSensor2 = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
String calculatedReflection;

void setup()
{
  Serial.begin(115200);
  // I2C Bus 1x für alle Bus-Teilnehmer initialisieren (sonst crasht das Betriebssystem)
  Wire.begin();           // Bus I2C0
  // Wire.setClock(1000000); // 1MHz Kommunikationsgeschwindigkeit
  Wire1.begin();  // Bus I2C1
  //  hier den zu nutzenden I2C Bus einstellen:
  if (!rgbSensor.begin(TCS34725_ADDRESS, &Wire))
  {
    delay(10000); // damit wir Zeit haben den Serial Monitor zu öffnen nach dem Upload
    Serial.println("RGB Farbsensor Verdrahtung prüfen!");
  }
  Serial.println("Initialisierung Farbe 1 abgeschlossen");
   if (!rgbSensor2.begin(TCS34725_ADDRESS, &Wire1))
  {
    delay(10000); // damit wir Zeit haben den Serial Monitor zu öffnen nach dem Upload
    Serial.println("RGB Farbsensor Verdrahtung prüfen!");
  }
  Serial.println("Initialisierung Farbe 2 abgeschlossen");
  sensorLeiste.setTypeRC();
  sensorLeiste.setSensorPins(SENSOR_LEISTE_PINS, SENSOR_LEISTE_ANZAHL_SENSOREN);
  Serial.println("Initialisierung Reflektionssensor abgeschlossen");
  if (!IMU.begin())
  {
    delay(10000); // damit wir Zeit haben den Serial Monitor zu öffnen nach dem Upload
    Serial.println("IMU Verdrahtung prüfen!");
  }
  Serial.println("Onboard_Sensoren Initialisierung abgeschlossen");
  motoren.flipLeftMotor(true);
  motoren.flipRightMotor(true);
}

// Sensorenwerte für Kalibrierung
int colorMinThreshold = 650;
int colorMaxThreshold = 1000;

int reflectionBlackThreshold = 120;

/* Liegt der Arduino gerade auf dem Tisch und wird nicht bewegt, muss die Z-Achse 1G (Erdbeschleunigung) ausgeben und der Rest muss 0 sein.
   Das ist nicht wirklich so und wird mit diesen Werten kalibriert: */
const float ACC_X_OFFSET = +0.0231;
const float ACC_Y_OFFSET = +0.0105;
const float ACC_Z_OFFSET = -0.0102;

/* Wird der Arduino nicht rotiert, müssen alle Winkelbeschleunigungen 0 sein.
   Das ist nicht wirklich so und wird mit diesen Werten kalibriert: */
const float GYRO_X_OFFSET = -0.43;
const float GYRO_Y_OFFSET = +0.67;
const float GYRO_Z_OFFSET = +0.31;

void loop()
{
  readAcc();
  if (acc.x <= 0.7)
  {
    straight();
  }
  else
  {
    calculatedReflection = calculateReflection();
    if (calculatedReflection == "frontLine")
    {
      doppelschwarz();
    }
    else if (calculatedReflection == "normalLine")
    {
      Serial.print("\n");
      Serial.print("Linie");
      straight();
      if (varrechts > 0)
      {
        varrechts = varrechts - 5;
      }
      if (varlinks > 0)
      {
        varlinks = varlinks - 5;
      }
    }
    else if (calculatedReflection == "leftLine")
    {
      Serial.print("\n");
      Serial.print("links");
      left();
      varlinks = varlinks + 5;
      if (varlinks + varrechts >= 400)
      {
        varlinks = 0;
        varrechts = 0;
        doppelschwarz();
      }
    }
    else if (calculatedReflection == "rightLine")
    {
      Serial.print("\n");
      Serial.print("rechts");
      right();
      varrechts = varrechts + 5;
      if (varlinks + varrechts >= 400)
      {
        varlinks = 0;
        varrechts = 0;
        doppelschwarz();
      }
    }
    else if (calculatedReflection == "noLine")
    {
      Serial.print("\n");
      Serial.print("keine Linie...");
      straight();
    }
    delay(50);
  }
}

void doppelschwarz()
{
  Serial.print("\n");
  Serial.print("alles-schwarz");
  motoren.setSpeeds(0, 0);
  delay(1000);
  readColor2();
  readColor();
  if (calculateColor())
  {
    if (calculateColor2())
    {
      turn();
    }
    else
    {
      straight();
      delay(1250);
      right();
      delay(1000);
      while (calculateReflection() == "noLine")
      {
        delay(1);
      }
    }
  }
  else
  {
    if (calculateColor2())
    {
      straight();
      delay(1250);
      left();
      delay(1000);
      while (calculateReflection() == "noLine")
      {
        delay(1);
      }
    }
    else
    {
      straight();
      delay(1200);
      motoren.setSpeeds(0, 0);
      if (!(calculateReflection() == "noLine"))
      {
        // not else lol
      }
      else
      {
        left();
        delay(2500);
        right();
        while (calculateReflection() == "noLine")
        {
          Serial.print("\n");
          Serial.print("suche...");
        }
      }
    }
  }
}

void straight()
{
  motoren.flipLeftMotor(true);
  motoren.flipRightMotor(true);
  motoren.setSpeeds(75, 75);
}

void left()
{
  motoren.flipLeftMotor(false);
  motoren.flipRightMotor(true);
  motoren.setSpeeds(75, 75);
}

void right()
{
  motoren.flipLeftMotor(true);
  motoren.flipRightMotor(false);
  motoren.setSpeeds(75, 75);
}

void turn()
{
  delay(1); // placeholder
}

void readColor()
{
  rgbSensor.getRawData(&rot, &gruen, &blau, &helligkeit);
  Serial.println("R:" + String(rot) + " G:" + String(gruen) + " B:" + String(blau) + " C:" + String(helligkeit));
  /** Dieser Mechanismus hier ist gefährlich, wenn es passieren kann, dass die Sensoren lange Zeit das selbe sehen:
   *  In meinen Versuchen habe ich oben den Gain von 4x auf 16x gestellt, um mehr Rauschen zu bekommen.
   *  Mit Timeout 5s sehe ich keine False-Negatives mehr: */
}

void readColor2()
{
  rgbSensor2.getRawData(&rot2, &gruen2, &blau2, &helligkeit2);
  Serial.println("R:" + String(rot2) + " G:" + String(gruen2) + " B:" + String(blau2) + " C:" + String(helligkeit2));
  /** Dieser Mechanismus hier ist gefährlich, wenn es passieren kann, dass die Sensoren lange Zeit das selbe sehen:
   *  In meinen Versuchen habe ich oben den Gain von 4x auf 16x gestellt, um mehr Rauschen zu bekommen.
   *  Mit Timeout 5s sehe ich keine False-Negatives mehr: */
}

boolean calculateColor()
{
  {
    if (((gruen >= blau) && (gruen >= rot)) && (gruen <= colorMaxThreshold) && (gruen >= colorMinThreshold))
    {
      return true;
    }
    else
    {
      return false;
    }
  }
}

boolean calculateColor2()
{
  {
    if (((gruen2 >= blau2) && (gruen2 >= rot2)) && (gruen2 <= colorMaxThreshold) && (gruen2 >= colorMinThreshold))
    {
      return true;
    }
    else
    {
      return false;
    }
  }
}

void read_reflectionandprint()
{
  sensorLeiste.read(helligkeiten);
  for (int i = 0; i < SENSOR_LEISTE_ANZAHL_SENSOREN; i++)
  {
    Serial.print(String(helligkeiten[i]) + '\t'); // alles in eine Zeile
  }
  Serial.println(); // neue Zeile beginnen
}

void readAcc()
{
  if (IMU.accelerationAvailable())
  {
    IMU.readAcceleration(acc.x, acc.y, acc.z);
    acc.x += ACC_X_OFFSET;
    acc.y += ACC_Y_OFFSET;
    acc.z += ACC_Z_OFFSET;
  } // else: nicht jedes Mal, wenn dir den Sensor fragen, hat er auch neue Werte
}

void ReadGyro()
{
  if (IMU.gyroscopeAvailable())
  {
    IMU.readGyroscope(gyro.x, gyro.y, gyro.z);
    gyro.x += GYRO_X_OFFSET;
    gyro.y += GYRO_Y_OFFSET;
    gyro.z += GYRO_Z_OFFSET;
  } // else: nicht jedes Mal, wenn dir den Sensor fragen, hat er auch neue Werte
}

void accWerteLoggen()
{
  Serial.println(
      "Beschleunigung x=" + String(acc.x) + " y=" + String(acc.y) + " z=" + String(acc.z) + " Rotationsgeschwindigkeit x=" + String(gyro.x) + " y=" + String(gyro.y) + " z=" + String(gyro.z));
}

void fusionInitialisieren()
{
  Serial.println("Der Arduino muss jetzt gerade liegen und darf nicht bewegt werden!");
  Fusion.setup(acc);
}

void fusionLoggen()
{
  Fusion.update(gyro, acc);
  /* Wenn man aus Sicht des Arduino Richting USB-Stecker schaut: */
  Serial.println(
      "Gierwinkel (links<->rechts schauen)=" + String(Fusion.yaw()) + " Nickwinkel (hoch<->runten schauen)=" + String(Fusion.pitch()) + " Rollwinkel (links<->rechts kippen)=" + String(Fusion.roll()));
}


String calculateReflection()
{
  read_reflectionandprint();
  if ((helligkeiten[0] >= reflectionBlackThreshold) && (helligkeiten[5] >= reflectionBlackThreshold))
  {
    return "frontalLine";
  }
  else if ((helligkeiten[2] >= reflectionBlackThreshold || helligkeiten[3] >= reflectionBlackThreshold) && (helligkeiten[0] <= reflectionBlackThreshold && helligkeiten[1] <= reflectionBlackThreshold && helligkeiten[4] <= reflectionBlackThreshold && helligkeiten[5] <= reflectionBlackThreshold))
  {
    return "normalLine";
  }
  else if (helligkeiten[0] >= reflectionBlackThreshold || helligkeiten[1] >= reflectionBlackThreshold)
  {
    return "leftLine";
  }
  else if (helligkeiten[4] >= reflectionBlackThreshold || helligkeiten[5] >= reflectionBlackThreshold)
  {
    return "rightLine";
  }
  else
  {
    return "noLine";
  }
}