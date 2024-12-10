//REFLEKTIONSSENSOREN
int varrechts = 0;
int varlinks = 0;
const uint8_t SENSOR_LEISTE_ANZAHL_SENSOREN = 6;
const uint8_t SENSOR_LEISTE_PINS[] = {D6, D0, D1, D7, D8, D9};
// hier speichern wir die 6 Reflektionssensorwerte ab:
uint16_t helligkeiten[SENSOR_LEISTE_ANZAHL_SENSOREN];
QTRSensors sensorLeiste = QTRSensors();
String calculatedReflection;
// hier speichern wir die Farbsensorwerte ab:
// Roh-Werte (Es gibt auch kalibierte Werte, aber die sind sehr langsam auszulesen):
uint16_t rot, gruen, blau, helligkeit;
uint16_t rot2, gruen2, blau2, helligkeit2;


//FARBSENSOREN
/** Sensor sehr schnell einstellen (ungenauer):
 *  Gain 4x fand ich am besten, aber dann sind die Werte so stabil,
 *  dass die Fehlerdetektion immer ausgelöst hat (siehe unten "helligkeitStatischStoppuhr.hasPassed"). */
Adafruit_TCS34725 rgbSensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_60MS, TCS34725_GAIN_4X);
Adafruit_TCS34725 rgbSensor2 = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_60MS, TCS34725_GAIN_4X);


//MOTOREN
// Dieses Objekt repräsentiert 2 Motor-Kanäle (1..2 Motoren pro Kanal):
RescueBoardMotors motors = RescueBoardMotors();
#define motorpin D12
#define kalibrierung A7


//ABSTANDSSENSOR
/** optional: Stoppuhr, um zu Verbindungsverluste zu erkennen */
bool debT = false; // debT = debug timer, machen wir an falls wir Probleme beim Abstandssensor haben und dann sagt der Timer ob überhaupt Werte ankommen
Chrono keineNeuenDatenStoppuhr = Chrono(Chrono::MILLIS, false); // Uhr noch nicht gestartet 
const uint16_t VERBINDUNG_VERLOREN = 0;
uint16_t vorheriger_abstand = VERBINDUNG_VERLOREN;
SparkFun_VL53L5CX abstandsSensor = SparkFun_VL53L5CX();
const uint8_t NEUE_ABSTANDSADDRESSE = 0x35 ;
typedef struct Einstellungen { // Structs sind schön weil die so wie Listen mit Konstruktor und verschiedenen Datentypen sind
    uint8_t aufloesung;
    uint8_t bildSeitenlaenge;
    uint8_t maxMessfrequenz;
} Einstellungen_t; // null ahnung was das eigentlich macht
// Könnte "kaputte Pixel" ausgeben:
const Einstellungen ACHT_MAL_ACHT = { VL53L5CX_RESOLUTION_8X8, 8, 15 }; // 8x8: max 15Hz
// Vermutlich stabiler
const Einstellungen VIER_MAL_VIER = { VL53L5CX_RESOLUTION_4X4, 4, 60 }; // 4x4: max 60Hz
Einstellungen einstellungen = VIER_MAL_VIER;
enum Modus {
    /* Werte im Serial Monitor anzeigen. */
    ABSTANDS_WERTE_LOGGEN,
    /* Sensor gehen eine Fläche richten (z.B. Monitor) und in weniger als SCHWELLENWERT mm etwas davor halten.  */
    SCHWELLENWERT_VISUALISIERUNG,
};
const int SCHWELLENWERT = 100;
/** hier einstellen, was das Programm mit den Sensorwerten anfangen soll: */
Modus modus = SCHWELLENWERT_VISUALISIERUNG;
// hier speichern wir die 6 TOFsensorwerte ab:
VL53L5CX_ResultsData messDaten;


//IRGENDWAS
#define CMPS12 0x60
uint16_t direction;
uint16_t currentdirection;