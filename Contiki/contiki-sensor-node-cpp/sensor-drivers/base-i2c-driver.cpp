/*
 * i2cdriver.cpp
 *
 *  Created on: 2014-03-23
 *      Author: francispapineau
 */

#include "base-i2c-driver.h"

i2c_driver::i2c_driver() {

}

char i2c_driver::readInt(char address, int &value)
// Read a signed integer (two bytes) from device
// address: register to start reading (plus subsequent register)
// value: external variable to store data (function modifies value)
{
	unsigned char data[2];

	data[0] = address;
	if (readBytes(data,2))
	{
		value = (((int)data[0]<<8)|(int)data[1]);
		//if (*value & 0x8000) *value |= 0xFFFF0000; // sign extend if negative
		return(1);
	}
	value = 0;
	return(0);
}


char i2c_driver::readUInt(char address, unsigned int &value)
// Read an unsigned integer (two bytes) from device
// address: register to start reading (plus subsequent register)
// value: external variable to store data (function modifies value)
{
	unsigned char data[2];

	data[0] = address;
	if (readBytes(data,2))
	{
		value = (((unsigned int)data[0]<<8)|(unsigned int)data[1]);
		return(1);
	}
	value = 0;
	return(0);
}


char i2c_driver::readBytes(unsigned char *values, char length)
// Read an array of bytes from device
// values: external array to hold data. Put starting register in values[0].
// length: number of bytes to read
{
	char x;

	Wire.beginTransmission(BMP180_ADDR);
	Wire.write(values[0]);
	_error = Wire.endTransmission();
	if (_error == 0)
	{
		Wire.requestFrom(BMP180_ADDR,length);
		while(Wire.available() != length) ; // wait until bytes are ready
		for(x=0;x<length;x++)
		{
			values[x] = Wire.read();
		}
		return(1);
	}
	return(0);
}


char i2c_driver::writeBytes(unsigned char *values, char length)
// Write an array of bytes to device
// values: external array of data to write. Put starting register in values[0].
// length: number of bytes to write
{
	char x;

	Wire.beginTransmission(BMP180_ADDR);
	Wire.write(values,length);
	_error = Wire.endTransmission();
	if (_error == 0)
		return(1);
	else
		return(0);
}

char i2c_driver::writeUInt(unsigned char address, unsigned int value)
	// Write an unsigned integer (16 bits) to a TSL2561 address (low byte first)
	// Address: TSL2561 address (0 to 15), low byte first
	// Value: unsigned int to write to address
	// Returns true (1) if successful, false (0) if there was an I2C error
	// (Also see getError() above)
{
	// Split int into lower and upper bytes, write each byte
	if (writeByte(address,lowByte(value))
		&& writeByte(address + 1,highByte(value)))
		return(true);

	return(false);
}

char i2c_driver::readByte(unsigned char address, unsigned char &value)
	// Reads a byte from a TSL2561 address
	// Address: TSL2561 address (0 to 15)
	// Value will be set to stored byte
	// Returns true (1) if successful, false (0) if there was an I2C error
	// (Also see getError() above)
{
	// Set up command byte for read
	Wire.beginTransmission(_i2c_address);
	Wire.write((address & 0x0F) | TSL2561_CMD);
	_error = Wire.endTransmission();

	// Read requested byte
	if (_error == 0)
	{
		Wire.requestFrom(_i2c_address,1);
		if (Wire.available() == 1)
		{
			value = Wire.read();
			return(true);
		}
	}
	return(false);
}

char i2c_driver::writeByte(unsigned char address, unsigned char value)
	// Write a byte to a TSL2561 address
	// Address: TSL2561 address (0 to 15)
	// Value: byte to write to address
	// Returns true (1) if successful, false (0) if there was an I2C error
	// (Also see getError() above)
{
	// Set up command byte for write
	Wire.beginTransmission(_i2c_address);
	Wire.write((address & 0x0F) | TSL2561_CMD);
	// Write byte
	Wire.write(value);
	_error = Wire.endTransmission();
	if (_error == 0)
		return(true);

	return(false);
}
