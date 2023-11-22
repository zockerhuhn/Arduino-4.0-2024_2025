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
ZumoMotors motoren;
/** Sensor sehr schnell einstellen (ungenauer):
 *  Gain 4x fand ich am besten, aber dann sind die Werte so stabil,
 *  dass die Fehlerdetektion immer ausgelöst hat (siehe unten "helligkeitStatischStoppuhr.hasPassed"). */
Adafruit_TCS34725 rgbSensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
Adafruit_TCS34725 rgbSensor2 = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
