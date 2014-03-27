
#include <BMP180-driver.h>
#include <stdio.h>
#include <math.h>

/**
 * The default class constructor
 */
BMP180::BMP180(){

	this->_delay = 0;
	this->_temperature = 0;
	this->_pressure = 0;
	this->_baseline_pressure = 0;
	this->_altitude = 0;

}

/**
 * This initializes the register values in the internal class.
 * It returns VALID if okay and INVALID if an error occurred.
 *
 * @return valid					- the valid flag
 */
valid_t BMP180::begin(){

	//! Variable declaration
	double c3,c4,b1;

	//! We check to see if the device is there
	if(!check_presence(PRESSURE_SENSOR_ADDRESS)){
		return INVALID;
	}

	//! The BMP180 includes factory calibration data stored on the device.
	//! Each device has different numbers, these must be retrieved and
	//! used in the calculations when taking pressure measurements.

	//! Retrieve calibration data from device:
	if (check_registers()){

		//! All reads completed successfully!

		//! If you need to check your math using known numbers,
		//! you can uncomment one of these examples.
		//! (The correct results are commented in the below functions.)

		//! Example from Bosch datasheet
		//! AC1 = 408; AC2 = -72; AC3 = -14383; AC4 = 32741; AC5 = 32757; AC6 = 23153;
		//! B1 = 6190; B2 = 4; MB = -32768; MC = -8711; MD = 2868;

		//! Example from http://wmrx00.sourceforge.net/Arduino/BMP180-Calcs.pdf
		//! AC1 = 7911; AC2 = -934; AC3 = -14306; AC4 = 31567; AC5 = 25671; AC6 = 18974;
		//! VB1 = 5498; VB2 = 46; MB = -32768; MC = -11075; MD = 2432;
		
		//! Compute floating-point polynominals:

		c3 = 160.0 * pow(2,-15) * AC3;
		c4 = pow(10,-3) * pow(2,-15) * AC4;
		b1 = pow(160,2) * pow(2,-30) * VB1;
		c5 = (pow(2,-15) / 160) * AC5;
		c6 = AC6;
		mc = (pow(2,11) / pow(160,2)) * MC;
		md = MD / 160.0;
		x0 = AC1;
		x1 = 160.0 * pow(2,-13) * AC2;
		x2 = pow(160,2) * pow(2,-25) * VB2;
		y0 = c4 * pow(2,15);
		y1 = c4 * c3;
		y2 = c4 * b1;
		p0 = (3791.0 - 8.0) / 1600.0;
		p1 = 1.0 - 7357.0 * pow(2,-20);
		p2 = 3038.0 * 100.0 * pow(2,-36);
		
		//! Set the pressure baseline
		this->convert_pressure();
		this->_baseline_pressure = this->get_pressure();

		//! Good computation
		return VALID;
	}

	//! Invalid computation
	return INVALID;
}

/**
 * This method starts the temperature conversion process by sending
 * a start command to the remote device.
 *
 * @return valid					- the valid flag
 */
valid_t BMP180::start_temperature(void){

	//! Variable declaration
	valid_t result;

	//! Command to send
	uint8_t data[] = {BMP180_REG_CONTROL,
					  BMP180_COMMAND_TEMPERATURE};

	write_request_t* req = set_tx_request(PRESSURE_SENSOR_ADDRESS, data, 2);

	//! Write the packet
	result = write_bytes(req);

	//! Set the delay
	if(result == VALID){
		this->_delay = 5;
	}

	//! Return the result
	return result;
}

/**
 * This method returns a temperature measurement in degrees C.
 *
 * @return valid					- the valid flag
 */
valid_t BMP180::convert_temperature(){

	//! Variable declaration
	double tu, a;
	i2c_packet* packet;

	//! Set the command
	uint8_t data[] = {BMP180_REG_RESULT};

	read_request_t* req = set_rx_request(PRESSURE_SENSOR_ADDRESS, data, 1, 2);

	//! Issue the request
	packet = read_bytes(req);

	//! good read, calculate temperature
	if (packet->valid_packet) {

		tu = (packet->buffer[0] * 256.0) + packet->buffer[1];
		a = c5 * (tu - c6);
		this->_temperature = a + (mc / (a + md));
		return VALID;
	}

	return INVALID;
}

/**
 * This is the access to the temperature method. It returns the
 * temperature from within the class.
 *
 * @return temp						- the temperature
 */
double BMP180::get_temperature(){

	//! Get the shared resource temperature
	ENTER_CRITICAL_SECTION();
	double temp = this->_temperature;
	EXIT_CRITICAL_SECTION();

	return temp;
}

/**
 * This method sets the pressure oversampling and also starts the
 * pressure sampling and conversion.
 *
 * @param oversampling				- the sampling <0-3>
 * @return valid					- the valid flag
 */
valid_t BMP180::start_pressure(uint8_t oversampling){

	uint8_t data[2], result;
	write_request_t* req;
	
	data[0] = BMP180_REG_CONTROL;

	//! Set the sampling rate
	switch (oversampling)
	{
		case 0:
			data[1] = BMP180_COMMAND_PRESSURE0;
			this->_delay = 5;
		break;
		case 1:
			data[1] = BMP180_COMMAND_PRESSURE1;
			this->_delay = 8;
		break;
		case 2:
			data[1] = BMP180_COMMAND_PRESSURE2;
			this->_delay = 14;
		break;
		case 3:
			data[1] = BMP180_COMMAND_PRESSURE3;
			this->_delay = 26;
		break;
		default:
			data[1] = BMP180_COMMAND_PRESSURE0;
			this->_delay = 5;
		break;
	}

	//! Create the i2c request
	req = set_tx_request(PRESSURE_SENSOR_ADDRESS, data, sizeof(data));

	//! Issue the request
	result = write_bytes(req);
	return result;
}

/**
 * This method issues a command to the pressure sensor to convert
 * the pressure and send it to our internal container.
 *
 * @return valid					- the valid flag
 */
valid_t BMP180::convert_pressure(){

	//! Variable declaration
	double pu,s,x,y,z;
	i2c_packet* packet;
	read_request_t* req;

	uint8_t data[] = {BMP180_REG_RESULT};

	//! Create a read request
	req = set_rx_request(PRESSURE_SENSOR_ADDRESS, data, 1, 3);

	//! Issue the request
	packet = read_bytes(req);
	
	//! good read, calculate pressure
	if (packet->valid_packet) {

		pu = (packet->buffer[0] * 256.0) + packet->buffer[1] + (packet->buffer[2]/256.0);
		
		s = this->_temperature - 25.0;
		x = (x2 * pow(s,2)) + (x1 * s) + x0;
		y = (y2 * pow(s,2)) + (y1 * s) + y0;
		z = (pu - x) / y;
		this->_pressure = (p2 * pow(z,2)) + (p1 * z) + p0;

		return VALID;
	}

	return INVALID;
}

/**
 * This method returns the internal pressure value in mbars.
 *
 * @return pressure					- the pressure in mbars
 */
double BMP180::get_pressure(){

	//! Get the data
	ENTER_CRITICAL_SECTION();
	double pres = this->_pressure;
	EXIT_CRITICAL_SECTION();

	return pres;
}

/**
 * This calculates the absolute sea level pressure in mbars and returns it.
 *
 * @return pressure 				- the sealevel pressure in mbars
 */
double BMP180::get_sealevel(){

	//! Calculate the pressure at sealevel
	ENTER_CRITICAL_SECTION();
	double pres = (this->_pressure/pow(1-(this->_altitude/44330.0),5.255));
	EXIT_CRITICAL_SECTION();

	//! Return the pressure
	return pres;
}

/**
 * This method calculates the altitude given a pressure baseline and
 * the varying pressure variable.
 *
 * @param pressure						- the varying pressure
 * @return altitude						- the calculated altitude in meters
 */
double BMP180::get_altitude(double pressure){

	//! Calculate altitude
	ENTER_CRITICAL_SECTION();
	double alt = (44330.0*(1 - pow(pressure / this->_baseline_pressure , 1/5.255)));
	EXIT_CRITICAL_SECTION();

	//! Return the altitude
	return alt;

}

/**
 * This is the coherence check for the altimeter on boot up.
 * It checks the register access and returns if it can.
 *
 * @return vlaid						- if the registers are accessible
 */
bool BMP180::check_registers(){

	uint8_t address = PRESSURE_SENSOR_ADDRESS;
	uint8_t data[];

	return \
			((AC1 = read_int(set_rx_request(address, data = {0xAA}, 1, NONE)) > 0) ? true : false) &&
			((AC2 = read_int(set_rx_request(address, data = {0xAC}, 1, NONE)) > 0) ? true : false) &&
			((AC3 = read_int(set_rx_request(address, data = {0xAE}, 1, NONE)) > 0) ? true : false) &&
			((AC4 = read_int(set_rx_request(address, data = {0xB0}, 1, NONE)) > 0) ? true : false) &&
			((AC5 = read_int(set_rx_request(address, data = {0xB2}, 1, NONE)) > 0) ? true : false) &&
			((AC6 = read_int(set_rx_request(address, data = {0xB4}, 1, NONE)) > 0) ? true : false) &&
			((VB1 = read_int(set_rx_request(address, data = {0xB6}, 1, NONE)) > 0) ? true : false) &&
			((VB2 = read_int(set_rx_request(address, data = {0xB8}, 1, NONE)) > 0) ? true : false) &&
			((MB  = read_int(set_rx_request(address, data = {0xBA}, 1, NONE)) > 0) ? true : false) &&
			((MC  = read_int(set_rx_request(address, data = {0xBC}, 1, NONE)) > 0) ? true : false) &&
			((MD  = read_int(set_rx_request(address, data = {0xAD}, 1, NONE)) > 0) ? true : false);
}




