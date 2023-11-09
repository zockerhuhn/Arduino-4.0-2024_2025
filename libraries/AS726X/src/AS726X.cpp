#include "AS726X.h"
#include "Arduino.h"
//Sets up the sensor for constant read
//Returns the sensor version (AS7262 or AS7263)

AS726X::AS726X()
{
	
}

bool AS726X::begin(TwoWire &wirePort, uint8_t gain, MeasurementMode measurementMode)
{
	_i2cPort = &wirePort;
	if (!virtualReadRegister(AS726x_HW_VERSION, _sensorVersion)) { return false; }
	if (_sensorVersion != 0x3E && _sensorVersion != 0x3F) //HW version for AS7262 and AS7263
	{
		return false;
	}

	setBulbCurrent(0b00); //Set to 12.5mA (minimum)
	disableBulb(); //Turn off to avoid heating the sensor

	setIndicatorCurrent(0b11); //Set to 8mA (maximum)
	disableIndicator(); //Turn off lights to save power

	setIntegrationTime(50); //50 * 2.8ms = 140ms. 0 to 255 is valid.
							//If you use Mode 2 or 3 (all the colors) then integration time is double. 140*2 = 280ms between readings.

	setGain(gain); //Set gain to 64x

	setMeasurementMode(measurementMode); //One-shot reading of VBGYOR

	if (_sensorVersion == 0)
	{
		return false;
	}
	return true;
}

uint8_t AS726X::getVersion()
{
	return _sensorVersion;
}

//Sets the measurement mode
void AS726X::setMeasurementMode(MeasurementMode mode)
{
	uint8_t intMode = (uint8_t) mode;
	if (intMode > 0b11) intMode = 0b11;

	//Read, mask/set, write
	uint8_t value = 0;
	virtualReadRegister(AS726x_CONTROL_SETUP, value); //Read
	value &= 0b11110011; //Clear BANK bits
	value |= (intMode << 2); //Set BANK bits with user's choice
	virtualWriteRegister(AS726x_CONTROL_SETUP, value); //Write
}

//Sets the gain value
//Gain 0: 1x (power-on default)
//Gain 1: 3.7x
//Gain 2: 16x
//Gain 3: 64x
void AS726X::setGain(uint8_t gain)
{
	if (gain > 0b11) gain = 0b11;

	//Read, mask/set, write
	uint8_t value = 0;
	virtualReadRegister(AS726x_CONTROL_SETUP, value); //Read
	value &= 0b11001111; //Clear GAIN bits
	value |= (gain << 4); //Set GAIN bits with user's choice
	virtualWriteRegister(AS726x_CONTROL_SETUP, value); //Write
}

//Sets the integration value
//Give this function a uint8_t from 0 to 255.
//Time will be 2.8ms * [integration value]
void AS726X::setIntegrationTime(uint8_t integrationValue)
{
	virtualWriteRegister(AS726x_INT_T, integrationValue); //Write
}

void AS726X::enableInterrupt()
{
	//Read, mask/set, write
	uint8_t value = 0;
	virtualReadRegister(AS726x_CONTROL_SETUP, value); //Read
	value |= 0b01000000; //Set INT bit
	virtualWriteRegister(AS726x_CONTROL_SETUP, value); //Write
}

//Disables the interrupt pin
void AS726X::disableInterrupt()
{
	//Read, mask/set, write
	uint8_t value = 0;
	virtualReadRegister(AS726x_CONTROL_SETUP, value); //Read
	value &= 0b10111111; //Clear INT bit
	virtualWriteRegister(AS726x_CONTROL_SETUP, value); //Write
}

//Tells IC to take measurements and polls for data ready flag
void AS726X::takeMeasurements()
{
	clearDataAvailable(); //Clear DATA_RDY flag when using Mode 3

						  //Goto mode 3 for one shot measurement of all channels
	setMeasurementMode(ON_REQUEST_READ_ALL);

	//Wait for data to be ready
	while (dataAvailable() == false) delay(POLLING_DELAY);

	//Readings can now be accessed via getViolet(), getBlue(), etc
}

//Turns on bulb, takes measurements, turns off bulb
void AS726X::takeMeasurementsWithBulb()
{
	//enableIndicator(); //Tell the world we are taking a reading. 
	//The indicator LED is red and may corrupt the readings

	enableBulb(); //Turn on bulb to take measurement

	takeMeasurements();

	disableBulb(); //Turn off bulb to avoid heating sensor
				   //disableIndicator();
}

//Get the various color readings
int AS726X::getViolet() { return(getChannel(AS7262_V)); }
int AS726X::getBlue() { return(getChannel(AS7262_B)); }
int AS726X::getGreen() { return(getChannel(AS7262_G)); }
int AS726X::getYellow() { return(getChannel(AS7262_Y)); }
int AS726X::getOrange() { return(getChannel(AS7262_O)); }
int AS726X::getRed() { return(getChannel(AS7262_R)); }

//Get the various NIR readings
int AS726X::getR() { return(getChannel(AS7263_R)); }
int AS726X::getS() { return(getChannel(AS7263_S)); }
int AS726X::getT() { return(getChannel(AS7263_T)); }
int AS726X::getU() { return(getChannel(AS7263_U)); }
int AS726X::getV() { return(getChannel(AS7263_V)); }
int AS726X::getW() { return(getChannel(AS7263_W)); }

//A the 16-bit value stored in a given channel registerReturns 
int AS726X::getChannel(uint8_t channelRegister)
{
	uint8_t hi = 0;
	virtualReadRegister(channelRegister, hi); //High uint8_t
	uint8_t lo = 0;
	virtualReadRegister(channelRegister + 1, lo); //Low uint8_t
	return (lo | (hi << 8));
}

//Returns the various calibration data
float AS726X::getCalibratedViolet() { return(getCalibratedValue(AS7262_V_CAL)); }
float AS726X::getCalibratedBlue() { return(getCalibratedValue(AS7262_B_CAL)); }
float AS726X::getCalibratedGreen() { return(getCalibratedValue(AS7262_G_CAL)); }
float AS726X::getCalibratedYellow() { return(getCalibratedValue(AS7262_Y_CAL)); }
float AS726X::getCalibratedOrange() { return(getCalibratedValue(AS7262_O_CAL)); }
float AS726X::getCalibratedRed() { return(getCalibratedValue(AS7262_R_CAL)); }

float AS726X::getCalibratedR() { return(getCalibratedValue(AS7263_R_CAL)); }
float AS726X::getCalibratedS() { return(getCalibratedValue(AS7263_S_CAL)); }
float AS726X::getCalibratedT() { return(getCalibratedValue(AS7263_T_CAL)); }
float AS726X::getCalibratedU() { return(getCalibratedValue(AS7263_U_CAL)); }
float AS726X::getCalibratedV() { return(getCalibratedValue(AS7263_V_CAL)); }
float AS726X::getCalibratedW() { return(getCalibratedValue(AS7263_W_CAL)); }

//Given an address, read four uint8_ts and return the floating point calibrated value
float AS726X::getCalibratedValue(uint8_t calAddress)
{
	uint8_t b0 = 0, b1 = 0, b2 = 0, b3 = 0;
	virtualReadRegister(calAddress + 0, b0);
	virtualReadRegister(calAddress + 1, b1);
	virtualReadRegister(calAddress + 2, b2);
	virtualReadRegister(calAddress + 3, b3);

	//Channel calibrated values are stored big-endian
	uint32_t calBytes = 0;
	calBytes |= ((uint32_t)b0 << (8 * 3));
	calBytes |= ((uint32_t)b1 << (8 * 2));
	calBytes |= ((uint32_t)b2 << (8 * 1));
	calBytes |= ((uint32_t)b3 << (8 * 0));

	return (convertBytesToFloat(calBytes));
}

//Given 4 uint8_ts returns the floating point value
float AS726X::convertBytesToFloat(uint32_t myLong)
{
	float myFloat;
	memcpy(&myFloat, &myLong, 4); //Copy uint8_ts into a float
	return (myFloat);
}

//Checks to see if DRDY flag is set in the control setup register
bool AS726X::dataAvailable()
{
	uint8_t value = 0;
	if (!virtualReadRegister(AS726x_CONTROL_SETUP, value)) { return true; } // to avoid blocking calling loop
	return (value & (1 << 1)); //Bit 1 is DATA_RDY
}

//Clears the DRDY flag
//Normally this should clear when data registers are read
void AS726X::clearDataAvailable()
{
	uint8_t value = 0;
	virtualReadRegister(AS726x_CONTROL_SETUP, value);
	value &= ~(1 << 1); //Set the DATA_RDY bit
	virtualWriteRegister(AS726x_CONTROL_SETUP, value);
}

//Enable the onboard indicator LED
void AS726X::enableIndicator()
{
	//Read, mask/set, write
	uint8_t value = 0;
	virtualReadRegister(AS726x_LED_CONTROL, value);
	value |= (1 << 0); //Set the bit
	virtualWriteRegister(AS726x_LED_CONTROL, value);
}

//Disable the onboard indicator LED
void AS726X::disableIndicator()
{
	//Read, mask/set, write
	uint8_t value = 0;
	virtualReadRegister(AS726x_LED_CONTROL, value);
	value &= ~(1 << 0); //Clear the bit
	virtualWriteRegister(AS726x_LED_CONTROL, value);
}

//Set the current limit of onboard LED. Default is max 8mA = 0b11.
void AS726X::setIndicatorCurrent(uint8_t current)
{
	if (current > 0b11) current = 0b11;
	//Read, mask/set, write
	uint8_t value = 0;
	virtualReadRegister(AS726x_LED_CONTROL, value); //Read
	value &= 0b11111001; //Clear ICL_IND bits
	value |= (current << 1); //Set ICL_IND bits with user's choice
	virtualWriteRegister(AS726x_LED_CONTROL, value); //Write
}

//Enable the onboard 5700k or external incandescent bulb
void AS726X::enableBulb()
{
	//Read, mask/set, write
	uint8_t value = 0;
	virtualReadRegister(AS726x_LED_CONTROL, value);
	value |= (1 << 3); //Set the bit
	virtualWriteRegister(AS726x_LED_CONTROL, value);
}

//Disable the onboard 5700k or external incandescent bulb
void AS726X::disableBulb()
{
	//Read, mask/set, write
	uint8_t value = 0;
	virtualReadRegister(AS726x_LED_CONTROL, value);
	value &= ~(1 << 3); //Clear the bit
	virtualWriteRegister(AS726x_LED_CONTROL, value);
}

//Set the current limit of bulb/LED.
//Current 0: 12.5mA
//Current 1: 25mA
//Current 2: 50mA
//Current 3: 100mA
void AS726X::setBulbCurrent(uint8_t current)
{
	if (current > 0b11) current = 0b11; //Limit to two bits

										//Read, mask/set, write
	uint8_t value = 0;
	virtualReadRegister(AS726x_LED_CONTROL, value); //Read
	value &= 0b11001111; //Clear ICL_DRV bits
	value |= (current << 4); //Set ICL_DRV bits with user's choice
	virtualWriteRegister(AS726x_LED_CONTROL, value); //Write
}

//Returns the temperature in C
//Pretty inaccurate: +/-8.5C
uint8_t AS726X::getTemperature()
{
	uint8_t value = 0;
	virtualReadRegister(AS726x_DEVICE_TEMP, value);
	return value;
}

//Convert to F if needed
float AS726X::getTemperatureF()
{
	float temperatureF = getTemperature();
	temperatureF = temperatureF * 1.8 + 32.0;
	return (temperatureF);
}

//Does a soft reset
//Give sensor at least 1000ms to reset
void AS726X::softReset()
{
	//Read, mask/set, write
	uint8_t value = 0;
	virtualReadRegister(AS726x_CONTROL_SETUP, value); //Read
	value |= (1 << 7); //Set RST bit
	virtualWriteRegister(AS726x_CONTROL_SETUP, value); //Write
}

//Read a virtual register from the AS726x
bool AS726X::virtualReadRegister(uint8_t virtualAddr, uint8_t &read_val)
{
	uint8_t status;

	//Do a prelim check of the read register
	if (!readRegister(AS72XX_SLAVE_STATUS_REG, status)) { return false; }
	if ((status & AS72XX_SLAVE_RX_VALID) != 0) //There is data to be read
	{
		//Serial.println("Premptive read");
		uint8_t incoming = readRegister(AS72XX_SLAVE_READ_REG, status); //Read the uint8_t but do nothing with it
	}

	//Wait for WRITE flag to clear
	while (1)
	{
		if (!readRegister(AS72XX_SLAVE_STATUS_REG, status)) { break; }
		if ((status & AS72XX_SLAVE_TX_VALID) == 0) break; // If TX bit is clear, it is ok to write
		delay(POLLING_DELAY);
	}

	// Send the virtual register address (bit 7 should be 0 to indicate we are reading a register).
	writeRegister(AS72XX_SLAVE_WRITE_REG, virtualAddr);

	//Wait for READ flag to be set
	while (1)
	{
		if (!readRegister(AS72XX_SLAVE_STATUS_REG, status)) { break; }
		if ((status & AS72XX_SLAVE_RX_VALID) != 0) break; // Read data is ready.
		delay(POLLING_DELAY);
	}

	if (!readRegister(AS72XX_SLAVE_READ_REG, status)) { return false; }
	read_val = status;
	return true;
}

//Write to a virtual register in the AS726x
void AS726X::virtualWriteRegister(uint8_t virtualAddr, uint8_t dataToWrite)
{
	uint8_t status;

	//Wait for WRITE register to be empty
	while (1)
	{
		if (!readRegister(AS72XX_SLAVE_STATUS_REG, status)) { break; }
		if ((status & AS72XX_SLAVE_TX_VALID) == 0) break; // No inbound TX pending at slave. Okay to write now.
		delay(POLLING_DELAY);
	}

	// Send the virtual register address (setting bit 7 to indicate we are writing to a register).
	writeRegister(AS72XX_SLAVE_WRITE_REG, (virtualAddr | 0x80));

	//Wait for WRITE register to be empty
	while (1)
	{
		if (!readRegister(AS72XX_SLAVE_STATUS_REG, status)) { break; }
		if ((status & AS72XX_SLAVE_TX_VALID) == 0) break; // No inbound TX pending at slave. Okay to write now.
		delay(POLLING_DELAY);
	}

	// Send the data to complete the operation.
	writeRegister(AS72XX_SLAVE_WRITE_REG, dataToWrite);
}

//Reads from a give location from the AS726x
bool AS726X::readRegister(uint8_t addr, uint8_t& read_val)
{
	_i2cPort->beginTransmission(AS726X_ADDR);
	_i2cPort->write(addr);
	_i2cPort->endTransmission();

	_i2cPort->requestFrom(AS726X_ADDR, 1);
	if (_i2cPort->available()) {
		read_val = (_i2cPort->read());
		return true;
	}
	else {
		return false; // Error
	}
}

//Write a value to a spot in the AS726x
void AS726X::writeRegister(uint8_t addr, uint8_t val)
{
	_i2cPort->beginTransmission(AS726X_ADDR);
	_i2cPort->write(addr);
	_i2cPort->write(val);
	_i2cPort->endTransmission();
}