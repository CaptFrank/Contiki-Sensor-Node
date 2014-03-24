/*
 * i2cdriver.h
 *
 *  Created on: 2014-03-23
 *      Author: francispapineau
 */

#ifndef I2CDRIVER_H_
#define I2CDRIVER_H_

#include <inttypes.h>

#include "i2c-conf.h"
#include "i2c.h"

typedef struct i2c_packet {

	union {
		uint8_t rx_buffer[I2C_RECEIVE_DATA_BUFFER_SIZE];
		uint8_t tx_buffer[I2C_SEND_DATA_BUFFER_SIZE];
	}buffer;

	union {
		uint8_t rx_length;
		uint8_t tx_length;
	}length;

	union {
		uint8_t transmit_address;
		uint8_t received_address;
	}address;

	uint8_t valid_packet;
};


class i2c_driver {

	public:
		i2c_driver();

		virtual void begin();
		virtual void begin(uint8_t address);

	private:

		buffer_struct buffer;

		virtual ~i2c_driver();

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

		char readBytes(unsigned char *values, char length);
			// read a number of bytes from a BMP180 register
			// values: array of char with register address in first location [0]
			// length: number of bytes to read back
			// returns 1 for success, 0 for fail, with read bytes in values[] array

		char writeBytes(unsigned char *values, char length);
			// write a number of bytes to a BMP180 register (and consecutive subsequent registers)
			// values: array of char with register address in first location [0]
			// length: number of bytes to write
			// returns 1 for success, 0 for fail

		char writeUInt(unsigned char address, unsigned int value);
			// Write an unsigned integer (16 bits) to a TSL2561 address (low byte first)
			// Address: TSL2561 address (0 to 15), low byte first
			// Value: unsigned int to write to address
			// Returns true (1) if successful, false (0) if there was an I2C error
			// (Also see getError() above)

		char i2c_driver::readByte(unsigned char address, unsigned char &value);

		char i2c_driver::writeByte(unsigned char address, unsigned char value);

};

#endif /* I2CDRIVER_H_ */
