/** der I2C Bus */
#include <Wire.h>
/** I2C Adresse: 0x29 (7-bit) (unveränderlich) */
#include <Adafruit_TCS34725.h>
/** optional: Stoppuhr, um zu Verbindungsverluste zu erkennen */
#include <QTRSensors.h>
#include <ZumoMotors.h>