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

#ifndef SFE_BMP180_h
#define SFE_BMP180_h

#include "../i2c-conf.h"
#include "../i2c-driver.h"
#include "i2c.h"

#define	BMP180_REG_CONTROL 0xF4
#define	BMP180_REG_RESULT 0xF6

#define	BMP180_COMMAND_TEMPERATURE 0x2E
#define	BMP180_COMMAND_PRESSURE0 0x34
#define	BMP180_COMMAND_PRESSURE1 0x74
#define	BMP180_COMMAND_PRESSURE2 0xB4
#define	BMP180_COMMAND_PRESSURE3 0xF4

class BMP180 : public base_i2c_driver
{
	public:
		BMP180(); // base type

		char begin();
			// call pressure.begin() to initialize BMP180 before use
			// returns 1 if success, 0 if failure (bad component or I2C bus shorted?)
		
		char startTemperature(void);
			// command BMP180 to start a temperature measurement
			// returns (number of ms to wait) for success, 0 for fail

		char getTemperature(double &T);
			// return temperature measurement from previous startTemperature command
			// places returned value in T variable (deg C)
			// returns 1 for success, 0 for fail

		char startPressure(char oversampling);
			// command BMP180 to start a pressure measurement
			// oversampling: 0 - 3 for oversampling value
			// returns (number of ms to wait) for success, 0 for fail

		char getPressure(double &P, double &T);
			// return absolute pressure measurement from previous startPressure command
			// note: requires previous temperature measurement in variable T
			// places returned value in P variable (mbar)
			// returns 1 for success, 0 for fail

		double sealevel(double P, double A);
			// convert absolute pressure to sea-level pressure (as used in weather data)
			// P: absolute pressure (mbar)
			// A: current altitude (meters)
			// returns sealevel pressure in mbar

		double altitude(double P, double P0);
			// convert absolute pressure to altitude (given baseline pressure; sea-level, runway, etc.)
			// P: absolute pressure (mbar)
			// P0: fixed baseline pressure (mbar)
			// returns signed altitude in meters

		char getError(void);
			// If any library command fails, you can retrieve an extended
			// error code using this command. Errors are from the wire library: 
			// 0 = Success
			// 1 = Data too long to fit in transmit buffer
			// 2 = Received NACK on transmit of address
			// 3 = Received NACK on transmit of data
			// 4 = Other error

	private:

		// Variable definition
		int AC1,AC2,AC3,VB1,VB2,MB,MC,MD;
		unsigned int AC4,AC5,AC6; 
		double c5,c6,mc,md,x0,x1,x2,y0,y1,y2,p0,p1,p2;
		char _error;
};

#endif
