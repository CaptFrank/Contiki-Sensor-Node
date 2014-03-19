/*
 * BMP180-driver.h
 *
 *  Created on: 2014-03-18
 *      Author: francispapineau
 */

#ifndef BMP180_DRIVER_H_
#define BMP180_DRIVER_H_

// BMP180 Regsiters

#define	BMP180_REG_CONTROL 0xF4
#define	BMP180_REG_RESULT 0xF6

#define	BMP180_COMMAND_TEMPERATURE 0x2E
#define	BMP180_COMMAND_PRESSURE0 0x34
#define	BMP180_COMMAND_PRESSURE1 0x74
#define	BMP180_COMMAND_PRESSURE2 0xB4
#define	BMP180_COMMAND_PRESSURE3 0xF4

int AC1,AC2,AC3,VB1,VB2,MB,MC,MD;
unsigned int AC4,AC5,AC6;
double c5,c6,mc,md,x0,x1,x2,y0,y1,y2,p0,p1,p2;
char _error;


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

char readInt(char address, int &value);
			// read an signed int (16 bits) from a BMP180 register
			// address: BMP180 register address
			// value: external signed int for returned value (16 bits)
			// returns 1 for success, 0 for fail, with result in value

char readUInt(char address, unsigned int &value);
			// read an unsigned int (16 bits) from a BMP180 register
			// address: BMP180 register address
			// value: external unsigned int for returned value (16 bits)
			// returns 1 for success, 0 for fail, with result in value





#endif /* BMP180_DRIVER_H_ */
