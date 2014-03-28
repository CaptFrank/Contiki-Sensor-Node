/*
	TSL2561 illumination sensor library for Arduino
	Mike Grusin, SparkFun Electronics
	
	This library provides functions to access the TAOS TSL2561
	Illumination Sensor.
	
	Our example code uses the "beerware" license. You can do anything
	you like with this code. No really, anything. If you find it useful,
	buy me a beer someday.

	version 1.0 2013/09/20 MDG initial version
*/

#include <TSL2561-driver.h>

/**
 * This is the default constructor for the class
 */
TSL2561::TSL2561(void){

	//! Set the internal values
	this->_address = LIGHT_SENSOR_ADDRESS;
	this->_error = 0;
}

/**
 * This is the initializer method for the i2c communication for the
 * remote device.
 *
 * @return true									- the install status
 */
bool TSL2561::begin(){

	//! We check presence
	if(check_presence(this->_address)){
		return true;
	}
	return false;
}


char TSL2561::setPowerUp(void)
	// Turn on TSL2561, begin integrations
	// Returns true (1) if successful, false (0) if there was an I2C error
	// (Also see getError() below)
{
	// Write 0x03 to command byte (power on)
	return(writeByte(TSL2561_REG_CONTROL,0x03));
}


char TSL2561::setPowerDown(void)
	// Turn off TSL2561
	// Returns true (1) if successful, false (0) if there was an I2C error
	// (Also see getError() below)
{
	// Clear command byte (power off)
	return(writeByte(TSL2561_REG_CONTROL,0x00));
}


char TSL2561::setTiming(char gain, unsigned char time)
	// If gain = false (0), device is set to low gain (1X)
	// If gain = high (1), device is set to high gain (16X)
	// If time = 0, integration will be 13.7ms
	// If time = 1, integration will be 101ms
	// If time = 2, integration will be 402ms
	// If time = 3, use manual start / stop
	// Returns true (1) if successful, false (0) if there was an I2C error
	// (Also see getError() below)
{
	unsigned char timing;

	// Get timing byte
	if (readByte(TSL2561_REG_TIMING,timing))
	{
		// Set gain (0 or 1)
		if (gain)
			timing |= 0x10;
		else
			timing &= ~0x10;

		// Set integration time (0 to 3)
		timing &= ~0x03;
		timing |= (time & 0x03);

		// Write modified timing byte back to device
		if (writeByte(TSL2561_REG_TIMING,timing))
			return(true);
	}
	return(false);
}


char TSL2561::setTiming(char gain, unsigned char time, unsigned int &ms)
	// If gain = false (0), device is set to low gain (1X)
	// If gain = high (1), device is set to high gain (16X)
	// If time = 0, integration will be 13.7ms
	// If time = 1, integration will be 101ms
	// If time = 2, integration will be 402ms
	// If time = 3, use manual start / stop (ms = 0)
	// ms will be set to integration time
	// Returns true (1) if successful, false (0) if there was an I2C error
	// (Also see getError() below)
{
	// Calculate ms for user
	switch (time)
	{
		case 0: ms = 14; break;
		case 1: ms = 101; break;
		case 2: ms = 402; break;
		default: ms = 0;
	}
	// Set integration using base function
	return(setTiming(gain,time));
}


char TSL2561::manualStart(void)
	// Starts a manual integration period
	// After running this command, you must manually stop integration with manualStop()
	// Internally sets integration time to 3 for manual integration (gain is unchanged)
	// Returns true (1) if successful, false (0) if there was an I2C error
	// (Also see getError() below)
{
	unsigned char timing;
	
	// Get timing byte
	if (readByte(TSL2561_REG_TIMING,timing))
	{
		// Set integration time to 3 (manual integration)
		timing |= 0x03;

		if (writeByte(TSL2561_REG_TIMING,timing))
		{
			// Begin manual integration
			timing |= 0x08;

			// Write modified timing byte back to device
			if (writeByte(TSL2561_REG_TIMING,timing))
				return(true);
		}
	}
	return(false);
}


char TSL2561::manualStop(void)
	// Stops a manual integration period
	// Returns true (1) if successful, false (0) if there was an I2C error
	// (Also see getError() below)
{
	unsigned char timing;
	
	// Get timing byte
	if (readByte(TSL2561_REG_TIMING,timing))
	{
		// Stop manual integration
		timing &= ~0x08;

		// Write modified timing byte back to device
		if (writeByte(TSL2561_REG_TIMING,timing))
			return(true);
	}
	return(false);
}


char TSL2561::getData(unsigned int &data0, unsigned int &data1)
	// Retrieve raw integration results
	// data0 and data1 will be set to integration results
	// Returns true (1) if successful, false (0) if there was an I2C error
	// (Also see getError() below)
{
	// Get data0 and data1 out of result registers
	if (readUInt(TSL2561_REG_DATA_0,data0) && readUInt(TSL2561_REG_DATA_1,data1)) 
		return(true);

	return(false);
}


char TSL2561::getLux(unsigned char gain, unsigned int ms, unsigned int CH0, unsigned int CH1, double &lux)
	// Convert raw data to lux
	// gain: 0 (1X) or 1 (16X), see setTiming()
	// ms: integration time in ms, from setTiming() or from manual integration
	// CH0, CH1: results from getData()
	// lux will be set to resulting lux calculation
	// returns true (1) if calculation was successful
	// RETURNS false (0) AND lux = 0.0 IF EITHER SENSOR WAS SATURATED (0XFFFF)
{
	double ratio, d0, d1;

	// Determine if either sensor saturated (0xFFFF)
	// If so, abandon ship (calculation will not be accurate)
	if ((CH0 == 0xFFFF) || (CH1 == 0xFFFF))
	{
		lux = 0.0;
		return(false);
	}

	// Convert from unsigned integer to floating point
	d0 = CH0; d1 = CH1;

	// We will need the ratio for subsequent calculations
	ratio = d1 / d0;

	// Normalize for integration time
	d0 *= (402.0/ms);
	d1 *= (402.0/ms);

	// Normalize for gain
	if (gain)
	{
		d0 /= 16;
		d1 /= 16;
	}

	// Determine lux per datasheet equations:
	
	if (ratio < 0.5)
	{
		lux = 0.0304 * d0 - 0.062 * d0 * pow(ratio,1.4);
		return(true);
	}

	if (ratio < 0.61)
	{
		lux = 0.0224 * d0 - 0.031 * d1;
		return(true);
	}

	if (ratio < 0.80)
	{
		lux = 0.0128 * d0 - 0.0153 * d1;
		return(true);
	}

	if (ratio < 1.30)
	{
		lux = 0.00146 * d0 - 0.00112 * d1;
		return(true);
	}

	// if (ratio > 1.30)
	lux = 0.0;
	return(true);
}


/**
 * Sets up interrupt operations
 *
 * If control = 0, interrupt output disabled
 * If control = 1, use level interrupt, see setInterruptThreshold()
 * If persist = 0, every integration cycle generates an interrupt
 * If persist = 1, any value outside of threshold generates an interrupt
 * If persist = 2 to 15, value must be outside of threshold for 2 to 15 integration cycles
 *
 * @param control						- the control variable
 * @param persist						- the persits variable
 * @return bool							- success
 */
bool setInterruptControl(unsigned char control, unsigned char persist){

	// Place control and persist bits into proper location in interrupt control register
	if (writeByte(TSL2561_REG_INTCTL,((control | 0B00000011) << 4) & (persist | 0B00001111)))
		return(true);
		
	return(false);
}


/**
 * Set interrupt thresholds (channel 0 only)
 *
 * @param low							- the low threshold value
 * @param high							- the high threshold value
 * @return bool							- the success
 */
bool set_interrupt_threshold(unsigned int low, unsigned int high){

	// Write low and high threshold values
	if (writeUInt(TSL2561_REG_THRESH_L,low) && writeUInt(TSL2561_REG_THRESH_H,high))
		return(true);
		
	return(false);
}

/**
 * This method clears the remote device interrupt flag.
 *
 * @return success						- the success in clearing the flag
 */
bool TSL2561::clear_interrupt(void){

	//! Set up command byte for interrupt clear
	uint8_t data[] = {TSL2561_CMD_CLEAR};

	//! Create a write request
	write_request_t* req = set_tx_request(this->_address, data, sizeof(data));

	if(this->write_byte(req) != VALID){
		return false;
	}
	return true;
}

/**
 * This method gets the id of the sensor.
 *
 * @return i2c_packet 					- the id of the sensor
 */
i2c_packet* TSL2561::get_ID(){

	//! Create the command
	uint8_t data[] = {TSL2561_REG_ID};

	//! We create a read request
	read_request_t* req = set_rx_request(this->_address, data, sizeof(data), 0x01);

	//! Get ID byte from ID register
	return this->read_byte(req);
}
