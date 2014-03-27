/*
 * i2cdriver.h
 *
 *  Created on: 2014-03-23
 *      Author: francispapineau
 */

#ifndef I2CDRIVER_H_
#define I2CDRIVER_H_

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include <i2c/i2c.h>

#include "i2c-conf.h"

//! Define the mutex operation macros
#define ENTER_CRITICAL_SECTION() \
	cli();

#define EXIT_CRITICAL_SECTION() \
	sei();

/**
 * This is the i2c packet type structure definition.
 * We use this packet structure to receive or transmit packets
 * on the i2c bus.
 */
typedef struct i2c_packet {

	/**
	 * The received and transmit buffer definitions. This could be
	 * either:
	 * 	- rx bound
	 * 	- tx bound
	 */
	uint8_t 	buffer[I2C_RECEIVE_DATA_BUFFER_SIZE];

	/**
	 * The receive and transmit length definitions. This could be
	 * either:
	 * 	- rx packet length
	 * 	- tx packet length
	 */
	uint8_t 	length;

	/**
	 * This is the received address or transmit address container.
	 * It could contain either:
	 * 	- transmit address
	 * 	- received address
	 */
	uint8_t 	address;

	/**
	 * This is the valid flag.
	 */
	uint8_t 	valid_packet;
};

/**
 * This is a read request structure which encompasses the necessary
 * data to read from a specific remote device.
 */
typedef struct read_request_t{

	uint8_t 	target_address;
	uint8_t 	target_read_length;
	uint8_t*	target_read_command;
	uint8_t		target_command_length;

};

/**
 * This is a write request structure which encompasses the necessary
 * data to write to a specific remote device.
 */
typedef struct write_request_t{

	uint8_t 	target_address;
	uint8_t		target_data;
	uint8_t		target_data_length;
};

/**
 * This is the valid check enum
 */
enum valid_t{
	VALID, 	//!< VALID
	INVALID	//!< INVALID
};

/**
 * This is the base class for the i2c drivers. We use this base class to design
 * the necessary functions pertaining to the sensor that we want to interface with.
 * It provides a standard functionality to each and everyone of the sensors.
 */
class base_i2c_driver {

	// Public context
	public:

		/**
		 * This is the default constructor for the base class.
		 */
		base_i2c_driver();

		/**
		 * These methods have to be implemented in the child class.
		 * It provides a standard init method.
		 */
		virtual valid_t begin();

		//! Variation of the init method.
		virtual valid_t begin(uint8_t address);

	// Private context
	private:

		/**
		 * We define the internal buffer type needed.
		 */
		i2c_packet _buffer;

		/**
		 * This is the valid container.
		 */
		valid_t _valid;

		/**
		 * The error if there is one
		 */
		uint8_t _error;

		/**
		 * The internal read request struct
		 */
		read_request_t rx_request;

		/**
		 * The internal write request struct
		 */
		write_request_t tx_request;

		/**
		 * This is the deconstructor for the class.
		 */
		~base_i2c_driver();

		// -- Utility methods

		/**
		 * This method provides the interface to creating a read request.
		 *
		 * @param address				- the address to send to
		 * @param command				- the command to execute
		 * @param command_length		- the command length
		 * @param read_length			- the read length
		 * @return	read_request_t		- the read_request
		 */
		read_request_t* set_rx_request(uint8_t address, uint8_t* command,
									   uint8_t command_length, uint8_t read_length);

		/**
		 * This method provides the interface to creating a transmit request.
		 *
		 * @param address				- the address to send to
		 * @param data					- the data to send
		 * @param data_length			- the data length
		 * @return	write_request_t		- the write_request
		 */
		write_request_t* set_tx_request(uint8_t address, uint8_t* data,
										uint8_t data_length);

		/**
		 * This method checks if the device is present
		 *
		 * @param target_address		- the remote address
		 * @return bool					- presence
		 */
		bool check_presence(uint8_t target_address);

		// -- Reading methods

		/**
		 * This method reads one byte from the remote node.
		 *
		 * @param read_req				- the read request
		 * @return value				- the buffer value
		 */
		i2c_packet* read_byte(struct read_request_t* read_req);

		/**
		 * This method reads many bytes from the i2c pipe.
		 *
		 * @param read_req				- the read request
		 * @return ptr					- the buffer pointer
		 */
		i2c_packet* read_bytes(struct read_request_t* read_req);

		/**
		 * This method reads an integer from the i2c pipe.
		 *
		 * @param read_req				- the read request
		 * @return int					- the buffer int
		 */
		int read_int(struct read_request_t* read_req);


		/**
		 * This method reads an unsigned integer from the i2c pipe.
		 *
		 * @param read_req				- the read request
		 * @return int					- the buffer int
		 */
		int read_uint(struct read_request_t* read_req);

		// -- Writing methods

		/**
		 * This method writes one byte on the i2c pipe.
		 *
		 * @param write_req				- the write request
		 * @return valid				- if the transaction was successful
		 */
		valid_t write_byte(struct write_request_t* write_req);

		/**
		 * This method writes multiple bytes to the i2c bus.
		 *
		 * @param write_req				- the write request
		 * @return valid				- if the transaction was successful
		 */
		valid_t write_bytes(struct write_request_t* write_req);

		/**
		 * This method writes an unsigned integer from the i2c pipe.
		 *
		 * @param write_req				- the write request
		 * @return valid				- if the transaction was successful
		 */
		valid_t write_uint(struct write_request_t* write_req);

		/**
		 * This method writes an integer from the i2c pipe.
		 *
		 * @param write_req				- the write request
		 * @return valid				- if the transaction was successful
		 */
		valid_t write_int(struct write_request_t* write_req);

		/**
		 * If any library command fails, you can retrieve an extended
		 * error code using this command. Errors are from the wire library:
		 * 		0 = Success
		 * 		1 = Data too long to fit in transmit buffer
		 * 		2 = Received NACK on transmit of address
		 * 		3 = Received NACK on transmit of data
		 * 		4 = Other error
		 *
		 * @return error				- the wire library error
		 */
		uint8_t get_error();
};

#endif /* I2CDRIVER_H_ */
