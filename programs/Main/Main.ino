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

#include "includes.h"
#include "variables.h"

// INCLUDES
/** I2C Adresse: 0x6A (7-bit) (unveränderlich) */
#include <Arduino_LSM6DSOX.h>
#include <imuFilter.h>
imuFilter Fusion = imuFilter(); // Objekt, dass Sensordaten fusionieren kann
#include <WiFiNINA.h>

int rgbValues[] = {255, 0, 0}; // 0=Red, 1=Green and 2=Blue
bool imuFusionInitialisiert = false;
double accxalt;
double accyalt;
double acczalt;
// hier speichern wir die Sensorwerte ab:
vec3_t acc = vec3_t(0, 0, 0);  // (x, y, z)
vec3_t gyro = vec3_t(0, 0, 0); // (x, y, z)

void setup()
{
  Serial.begin(115200);
  // I2C Bus 1x für alle Bus-Teilnehmer initialisieren (sonst crasht das Betriebssystem)
  Wire.begin(); // Bus I2C0
  // Wire.setClock(1000000); // 1MHz Kommunikationsgeschwindigkeit
  Wire1.begin(); // Bus I2C1
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
    Serial.Println("Onboard broken");
  }
  motoren.flipLeftMotor(true);
  motoren.flipRightMotor(true);
}

#include "Kalibrierung.h"
#include "Motorbewegungen.h"
#include "Farbauslese.h"
#include "Reflektionsauslese.h"
#include "doppelschwarz.h"

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
