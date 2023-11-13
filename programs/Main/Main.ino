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
 *
 * Der VL53L0X ist ein externer Sensor, kann also entweder
 * - an   Bus I2C0 ("Wire")
 * - oder Bus I2C1 ("Wire1") verbunden werden.
 */

/** der I2C Bus */
#include <Wire.h>
/** I2C Adresse: 0x29 (7-bit) (unveränderlich) */
#include <Adafruit_TCS34725.h>
/** Sensor sehr schnell einstellen (ungenauer):
 *  Gain 4x fand ich am besten, aber dann sind die Werte so stabil,
 *   dass die Fehlerdetektion immer ausgelöst hat (siehe unten "helligkeitStatischStoppuhr.hasPassed"). */
Adafruit_TCS34725 rgbSensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

/** optional: Stoppuhr, um zu Verbindungsverluste zu erkennen */
#include <Chrono.h>
Chrono helligkeitStatischStoppuhr = Chrono(Chrono::MILLIS, false); // noch nicht gestartet

#include <QTRSensors.h>
#include "ZumoMotors.h"

ZumoMotors motoren;

int varrechts = 0;
int varlinks = 0;
QTRSensors sensorLeiste = QTRSensors();
const uint8_t SENSOR_LEISTE_ANZAHL_SENSOREN = 6;
const uint8_t SENSOR_LEISTE_PINS[] = {10, 11, 12, 9, 8, 7};

void setup()
{
  Serial.begin(115200);
  // I2C Bus 1x für alle Bus-Teilnehmer initialisieren (sonst crasht das Betriebssystem)
  Wire.begin();           // Bus I2C0
  Wire.setClock(1000000); // 1MHz Kommunikationsgeschwindigkeit
  // Wire1.begin();  // Bus I2C1
  //  hier den zu nutzenden I2C Bus einstellen:
  if (!rgbSensor.begin(TCS34725_ADDRESS, &Wire))
  {
    delay(10000); // damit wir Zeit haben den Serial Monitor zu öffnen nach dem Upload
    Serial.println("RGB Farbsensor Verdrahtung prüfen!");
  }
  helligkeitStatischStoppuhr.start();
  Serial.println("Initialisierung Farbe 1 abgeschlossen");
  sensorLeiste.setTypeRC();
  sensorLeiste.setSensorPins(SENSOR_LEISTE_PINS, SENSOR_LEISTE_ANZAHL_SENSOREN);
  Serial.println("Initialisierung Reflektionssensor abgeschlossen");
  motoren.flipLeftMotor(true);
  motoren.flipRightMotor(true);
}

// hier speichern wir die Sensorwerte ab:
// Roh-Werte (Es gibt auch kalibierte Werte, aber die sind sehr langsam auszulesen):
uint16_t rot, gruen, blau, helligkeit;
uint16_t rot2, gruen2, blau2, helligkeit2;
// hier speichern wir die 6 Sensorwerte ab:
uint16_t helligkeiten[SENSOR_LEISTE_ANZAHL_SENSOREN];

void doppelschwarz()
{
  Serial.print("\n");
  Serial.print("alles-schwarz");
  motoren.setSpeeds(0, 0);
  delay(1000);
  readColor2();
  readColor();
  if (((gruen2 >= blau2) && (gruen2 >= rot2)) && (gruen2 <= 1000) && (gruen2 >= 650))
  {
    if (((gruen >= blau) && (gruen >= rot)) && (gruen <= 1000) && (gruen >= 650))
    {
      turn();
    }
    else
    {
      straight();
      delay(1250);
      right();
      delay(1000);
      read_reflectionandprint();
      while (helligkeiten[5] <= 120)
      {
        read_reflectionandprint();
      }
    }
  }
  else
  {
    if (((gruen >= blau) && (gruen >= rot)) && (gruen <= 1000) && (gruen >= 650))
    {
      straight();
      delay(1250);
      left();
      delay(1000);
      read_reflectionandprint();
      while (helligkeiten[1] <= 120)
      {
        read_reflectionandprint();
      }
    }
    else
    {
      straight();
      delay(1200);
      motoren.setSpeeds(0, 0);
      read_reflectionandprint();
      if (helligkeiten[0] >= 120 || helligkeiten[1] >= 120 || helligkeiten[2] >= 120 || helligkeiten[3] >= 120 || helligkeiten[4] >= 120 || helligkeiten[5] >= 120)
      {
        // not else lol
      }
      else
      {
        left();
        delay(2500);
        right();
        read_reflectionandprint();
        while (helligkeiten[1] <= 120 && helligkeiten[2] <= 120 && helligkeiten[3] <= 120 && helligkeiten[4] <= 120)
        {
          read_reflectionandprint();
          Serial.print("\n");
          Serial.print("suche...");
        }
      }
    }
  }
}

void loop()
{
  read_reflectionandprint();
  if ((helligkeiten[0] >= 120) && (helligkeiten[5] >= 120))
  {
    doppelschwarz();
  }
  else if ((helligkeiten[2] >= 120 || helligkeiten[3] >= 120) && (helligkeiten[0] <= 120 && helligkeiten[1] <= 120 && helligkeiten[4] <= 120 && helligkeiten[5] <= 120))
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
  else if (helligkeiten[0] >= 120 || helligkeiten[1] >= 120)
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
  else if (helligkeiten[4] >= 120 || helligkeiten[5] >= 120)
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
  else if (helligkeiten[0] <= 120 && helligkeiten[1] <= 120 && helligkeiten[2] <= 120 && helligkeiten[3] <= 120 && helligkeiten[4] <= 120 && helligkeiten[5] <= 120)
  {
    Serial.print("\n");
    Serial.print("keine Linie...");
    straight();
  }
  delay(50);
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

const uint16_t VERBINDUNG_VERLOREN = 0;
uint16_t vorheriges_rot, vorheriges_gruen, vorheriges_blau, vorherige_helligkeit = VERBINDUNG_VERLOREN;
uint16_t vorheriges_rot2, vorheriges_gruen2, vorheriges_blau2, vorherige_helligkeit2 = VERBINDUNG_VERLOREN;

void readColor()
{
  rgbSensor.getRawData(&rot, &gruen, &blau, &helligkeit);
  Serial.println("R:" + String(rot) + " G:" + String(gruen) + " B:" + String(blau) + " C:" + String(helligkeit));
  /** Dieser Mechanismus hier ist gefährlich, wenn es passieren kann, dass die Sensoren lange Zeit das selbe sehen:
   *  In meinen Versuchen habe ich oben den Gain von 4x auf 16x gestellt, um mehr Rauschen zu bekommen.
   *  Mit Timeout 5s sehe ich keine False-Negatives mehr: */
  if (!helligkeitStatischStoppuhr.hasPassed(5000))
  {
    // alles OK
    if (vorheriges_rot != rot || vorheriges_gruen != gruen ||
        vorheriges_blau != blau || vorherige_helligkeit != helligkeit)
    {
      // merken: der Wert hat sich verändert
      vorheriges_rot = rot;
      vorheriges_gruen = gruen;
      vorheriges_blau = blau;
      vorherige_helligkeit = helligkeit;
      helligkeitStatischStoppuhr.restart();
    }
    return; // rausgehen aus der Funktion, damit wir nicht zum Fehler kommen
  }

  // Fehler:
  rot = VERBINDUNG_VERLOREN;
  gruen = VERBINDUNG_VERLOREN;
  blau = VERBINDUNG_VERLOREN;
  helligkeit = VERBINDUNG_VERLOREN;
  helligkeitStatischStoppuhr.restart(); // um bei Wackelkontakt Wiederverbindung zu erlauben
  Serial.println("RGB Sensor 1 Verdrahtung prüfen!");
}

void readColor2()
{
  rgbSensor.getRawData(&rot2, &gruen2, &blau2, &helligkeit2);
  Serial.println("R:" + String(rot2) + " G:" + String(gruen2) + " B:" + String(blau2) + " C:" + String(helligkeit2));
  /** Dieser Mechanismus hier ist gefährlich, wenn es passieren kann, dass die Sensoren lange Zeit das selbe sehen:
   *  In meinen Versuchen habe ich oben den Gain von 4x auf 16x gestellt, um mehr Rauschen zu bekommen.
   *  Mit Timeout 5s sehe ich keine False-Negatives mehr: */
  if (!helligkeitStatischStoppuhr.hasPassed(5000))
  {
    // alles OK
    if (vorheriges_rot2 != rot2 || vorheriges_gruen2 != gruen2 ||
        vorheriges_blau2 != blau2 || vorherige_helligkeit2 != helligkeit2)
    {
      // merken: der Wert hat sich verändert
      vorheriges_rot2 = rot2;
      vorheriges_gruen2 = gruen2;
      vorheriges_blau2 = blau2;
      vorherige_helligkeit2 = helligkeit2;
      helligkeitStatischStoppuhr.restart();
    }
    return; // rausgehen aus der Funktion, damit wir nicht zum Fehler kommen
  }

  // Fehler:
  rot2 = VERBINDUNG_VERLOREN;
  gruen2 = VERBINDUNG_VERLOREN;
  blau2 = VERBINDUNG_VERLOREN;
  helligkeit2 = VERBINDUNG_VERLOREN;
  helligkeitStatischStoppuhr.restart(); // um bei Wackelkontakt Wiederverbindung zu erlauben
  Serial.println("RGB Sensor 2 Verdrahtung prüfen!");
}

void calculatecolor()
{
  if (((gruen >= blau) && (gruen >= rot)) && (gruen <= 1000) && (gruen >= 650))
  {
    Serial.println("green");
  }
  else
  {
    Serial.println("not green");
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