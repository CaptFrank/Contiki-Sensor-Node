/*
	SFE_BMP180.h
	Bosch BMP180 pressure sensor library for the Arduino microcontroller
	Mike Grusin, SparkFun Electronics

	Uses floating-point equations from the Weather Station Data Logger project
	http://wmrx00.sourceforge.net/
	http://wmrx00.sourceforge.net/Arduino/BMP085-Calcs.pdf

	Forked from BMP085 library by M.Grusin

	version 1.0 2013/09/20 initial version
	
	Our example code uses the "beerware" license. You can do anything
	you like with this code. No really, anything. If you find it useful,
	buy me a (root) beer someday.
*/

#ifndef BMP180_h
#define BMP180_h

#include "Wire.h"

#include "../i2c-conf.h"
#include "../i2c-driver.h"

//! Register definitions
#define	BMP180_REG_CONTROL 				0xF4
#define	BMP180_REG_RESULT 				0xF6

#define	BMP180_COMMAND_TEMPERATURE 		0x2E
#define	BMP180_COMMAND_PRESSURE0 		0x34
#define	BMP180_COMMAND_PRESSURE1 		0x74
#define	BMP180_COMMAND_PRESSURE2 		0xB4
#define	BMP180_COMMAND_PRESSURE3 		0xF4

#define NONE							0x00

/**
 * This is the BMP180 driver for the system. we use these methods to get
 * the appropriate data pieces. It extends the base i2c driver class.
 */
class BMP180 : public base_i2c_driver
{
	public:

		/**
		 * The default class constructor
		 */
		BMP180();

		/**
		 * The default class deconstructor
		 */
		virtual ~BMP180();

		/**
		 * This initializes the register values in the internal class.
		 * It returns VALID if okay and INVALID if an error occurred.
		 *
		 * @return valid					- the valid flag
		 */
		valid_t begin();
		
		/**
		 * This method starts the temperature conversion process by sending
		 * a start command to the remote device.
		 *
		 * @return valid					- the valid flag
		 */
		valid_t start_temperature(void);

		/**
		 * This method returns if the conversion and receiving of the
		 * data was successful. The data is then contained internally.
		 *
		 * @return valid					- the valid flag
		 */
		valid_t convert_temperature();

		/**
		 * This is the access to the temperature method. It returns the
		 * temperature from within the class.
		 *
		 * @return temp						- the temperature
		 */
		double get_temperature();

		/**
		 * This method sets the pressure oversampling and also starts the
		 * pressure sampling and conversion.
		 *
		 * @param oversampling				- the sampling <0-3>
		 * @return valid					- the valid flag
		 */
		valid_t start_pressure(uint8_t oversampling);

		/**
		 * This method issues a command to the pressure sensor to convert
		 * the pressure and send it to our internal container.
		 *
		 * @return valid					- the valid flag
		 */
		valid_t convert_pressure();

		/**
		 * This method returns the internal pressure value in mbars.
		 *
		 * @return pressure					- the pressure in mbars
		 */
		double get_pressure();

		/**
		 * This calculates the absolute sea level pressure in mbars and returns it.
		 *
		 * @return pressure 				- the sealevel pressure in mbars
		 */
		double get_sealevel();


		/**
		 * This method calculates the altitude given a pressure baseline and
		 * the varying pressure variable.
		 *
		 * @param pressure						- the varying pressure
		 * @return altitude						- the calculated altitude in meters
		 */
		double get_altitude(double pressure);

	//! Private Context
	private:

		//! Variable definition
		int AC1 = 0, AC2 = 0, AC3 = 0, VB1 = 0, \
			VB2 = 0, MB = 0, MC = 0, MD = 0;

		unsigned int AC4 = 0, AC5 = 0, AC6 = 0;

		double c5 = 0, c6 = 0, mc = 0, md = 0, x0 = 0,\
			   x1 = 0, x2 = 0, y0 = 0, y1 = 0, y2 = 0,\
			   p0 = 0, p1 = 0, p2 = 0;

		//! The internal address
		const uint8_t _address = PRESSURE_SENSOR_ADDRESS;

		//! Baseline pressure
		double _baseline_pressure;

		//! Delay container
		uint8_t _delay;

		//! Temperature container
		double _temperature;

		//! Pressure container
		double _pressure;

		//! Altitude container
		double _altitude;

		/**
		 * This is the coherence check for the altimeter on boot up.
		 * It checks the register access and returns if it can.
		 *
		 * @return vlaid						- if the registers are accessible
		 */
		bool check_registers();
};

#endif
