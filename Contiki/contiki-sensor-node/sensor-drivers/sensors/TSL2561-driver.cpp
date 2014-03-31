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
	this->_timing = 0;
	this->_lux = 0;
	this->_error = 0;
	this->_data0 = 0;
	this->_data1 = 0;
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

/**
 * Turn on TSL2561, begin integration
 */
void TSL2561::set_power_up(void){

	//! Write 0x03 to command byte (power on)

	//! Set up command byte for interrupt clear
	uint8_t data[] = {TSL2561_CMD_MACRO(TSL2561_REG_CONTROL), 0x03};

	//! Create a write request
	write_request_t* req = set_tx_request(this->_address, data, sizeof(data));

	if(this->write_bytes(req) != VALID){
		this->_error = true;
	}
	return;
}

/**
 * Turn off TSL2561
 */
void TSL2561::set_power_down(void){

	//! Clear command byte (power off)
	//! Set up command byte for interrupt clear
	uint8_t data[] = {TSL2561_CMD_MACRO(TSL2561_REG_CONTROL), 0x00};

	//! Create a write request
	write_request_t* req = set_tx_request(this->_address, data, sizeof(data));

	if(this->write_bytes(req) != VALID){
		this->_error = true;
	}
	return;
}

/**
 * This method sets the gain and the integration time
 *
 * If gain = false (0), device is set to low gain (1X)
 * If gain = high (1), device is set to high gain (16X)
 * If time = 0, integration will be 13.7ms
 * If time = 1, integration will be 101ms
 * If time = 2, integration will be 402ms
 * If time = 3, use manual start / stop
 *
 * @param gain							- the gain
 * @param time							- the time
 */
void TSL2561::set_timing(uint8_t gain, uint8_t time){

	//! Read the timing location
	i2c_packet* ptr = read_timing();

	//! Get timing byte
	if (ptr->valid_packet){

		this->_timing = ptr->buffer[0];

		//! Set gain (0 or 1)
		if (gain)
			this->_timing |= 0x10;
		else
			this->_timing &= ~0x10;

		//! Set integration time (0 to 3)
		this->_timing &= ~0x03;
		this->_timing |= (time & 0x03);

		//! Write the timing back
		write_timing();
	}
	return;
}

/**
 * This method starts a manual integration period.
 *
 * @return								- if the command was successful
 */
bool TSL2561::manual_start(void){
	
	//! We read
	i2c_packet* ptr = read_timing();

	//! Get timing byte
	if (ptr->valid_packet){

		//! Set integration time to 3 (manual integration)
		this->_timing |= 0x03;

		if (write_timing()){

			//! Begin manual integration
			this->_timing |= 0x08;

			//! Write modified timing byte back to device
			if (write_timing())
				return true;
		}
	}
	return false;
}

/**
 * This method stops a manual integration period.
 *
 * @return bool							- if the command was successful
 */
bool TSL2561::manual_stop(void){

	//! We read
	i2c_packet* ptr = read_timing();
	
	//! Get timing byte
	if (ptr->valid_packet){

		//! Stop manual integration
		this->_timing &= ~0x08;

		//! Write modified timing byte back to device
		if (write_timing())
			return true;
	}
	return false;
}

/**
 * This method gets the raw data from the I2C bus and converts them into internal
 * data.
 *
 * @return bool							- if the data acquisition was successful
 */
bool TSL2561::get_data(){

	//! Create the command
	uint8_t data[] = {TSL2561_CMD_MACRO(TSL2561_REG_DATA_0)};

	//! We create a read request
	read_request_t* req = set_rx_request(this->_address, data, sizeof(data), 0x02);

	//! Get ID byte from ID register
	this->_data0 = this->read_uint(req);

	if (this->_data0 <= 0){
		this->_error = true;
		return false;
	}

	//! Create the command
	uint8_t data[] = {TSL2561_CMD_MACRO(TSL2561_REG_DATA_1)};

	//! We create a read request
	read_request_t* req = set_rx_request(this->_address, data, sizeof(data), 0x02);

	//! Get ID byte from ID register
	this->_data1 = this->read_uint(req);

	if (this->_data1 <= 0){
		this->_error = true;
		return false;
	}

	return true;
}

/**
 * This method converts a lux value into a double value and stores it internally.
 *
 * @param gain							- 0 = 1X and 1 = 16X
 * @param ms							- the intergration time in ms
 * @param CH0							- result from get_data()
 * @param CH1							- result from get_data()
 */
void TSL2561::convert_lux(uint8_t gain, uint16_t ms, uint16_t CH0, uint16_t CH1){

	double ratio, d0, d1;

	//! Determine if either sensor saturated (0xFFFF)
	//! If so, abandon ship (calculation will not be accurate)
	if ((CH0 == 0xFFFF) || (CH1 == 0xFFFF)){

		this->_lux = 0.0;
		return;
	}

	//! Convert from unsigned integer to floating point
	d0 = CH0; d1 = CH1;

	//! We will need the ratio for subsequent calculations
	ratio = d1 / d0;

	//! Normalize for integration time
	d0 *= (402.0/ms);
	d1 *= (402.0/ms);

	//! Normalize for gain
	if (gain){

		d0 /= 16;
		d1 /= 16;
	}

	//! Determine lux per datasheet equations:
	if (ratio < 0.5){

		this->_lux = 0.0304 * d0 - 0.062 * d0 * pow(ratio,1.4);
		return;
	}if (ratio < 0.61){

		this->_lux = 0.0224 * d0 - 0.031 * d1;
		return;
	}if (ratio < 0.80){

		this->_lux = 0.0128 * d0 - 0.0153 * d1;
		return;
	}if (ratio < 1.30){

		this->_lux = 0.00146 * d0 - 0.00112 * d1;
		return;
	}

	//! if (ratio > 1.30)
	this->_lux = 0.0;
	return;
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
bool TSL2561::set_interrupt_control(unsigned char control, unsigned char persist){

	//! Set up command byte for interrupt clear
	uint8_t data[] = {TSL2561_CMD_MACRO(TSL2561_REG_INTCTL), ((control | 0B00000011) << 4) & (persist | 0B00001111)};

	//! Create a write request
	write_request_t* req = set_tx_request(this->_address, data, sizeof(data));

	if(this->write_bytes(req) != VALID){
		this->_error = true;
		return false;
	}
		
	return true;
}

/**
 * Set interrupt thresholds (channel 0 only)
 *
 * @param low							- the low threshold value
 * @param high							- the high threshold value
 * @return bool							- the success
 */
bool TSL2561::set_interrupt_threshold(unsigned int low, unsigned int high){

	//! Set up command byte for interrupt clear
	uint8_t data[] = {TSL2561_CMD_MACRO(TSL2561_REG_THRESH_L), low};

	//! Create a write request
	write_request_t* req = set_tx_request(this->_address, data, sizeof(data));

	if(this->write_bytes(req) != VALID){
		return false;
	}

	//! Set up command byte for interrupt clear
	uint8_t data[] = {TSL2561_CMD_MACRO(TSL2561_REG_THRESH_H), high};

	//! Create a write request
	write_request_t* req = set_tx_request(this->_address, data, sizeof(data));

	if(this->write_bytes(req) != VALID){
		this->_error = true;
		return false;
	}
		
	return true;
}

/**
 * This method clears the remote device interrupt flag.
 *
 * @return success						- the success in clearing the flag
 */
bool TSL2561::clear_interrupt(void){

	//! Set up command byte for interrupt clear
	uint8_t data[] = {TSL2561_CMD_MACRO(TSL2561_CMD_CLEAR)};

	//! Create a write request
	write_request_t* req = set_tx_request(this->_address, data, sizeof(data));

	if(this->write_byte(req) != VALID){
		this->_error = true;
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
	uint8_t data[] = {TSL2561_CMD_MACRO(TSL2561_REG_ID)};

	//! We create a read request
	read_request_t* req = set_rx_request(this->_address, data, sizeof(data), 0x01);

	//! Get ID byte from ID register
	return this->read_byte(req);
}

// Private context

/**
 * This is the read_timing method that stored the timing variable internally
 *
 * @return bool							- the success
 */
i2c_packet* TSL2561::read_timing(){

	//! Create the command
	uint8_t data[] = {TSL2561_CMD_MACRO(TSL2561_REG_TIMING)};

	//! We create a read request
	read_request_t* req = set_rx_request(this->_address, data, sizeof(data), 0x01);

	//! Get ID byte from ID register
	return this->read_byte(req);

}

/**
 * This is the writes_timing method that stored the timing variable internally
 *
 * @return bool							- the success
 */
bool TSL2561::write_timing(){

	//! Set up command byte for interrupt clear
	uint8_t data[] = {TSL2561_CMD_MACRO(TSL2561_REG_TIMING), this->_timing};

	//! Create a write request
	write_request_t* req = set_tx_request(this->_address, data, sizeof(data));

	if(this->write_byte(req) != VALID){
		this->_error = true;
		return false;
	}
	return true;
}
