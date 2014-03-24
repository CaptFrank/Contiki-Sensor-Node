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
	union {

		uint8_t rx_buffer[I2C_RECEIVE_DATA_BUFFER_SIZE];
		uint8_t tx_buffer[I2C_SEND_DATA_BUFFER_SIZE];

	}buffer; // The useful struct address.

	/**
	 * The receive and transmit length definitions. This could be
	 * either:
	 * 	- rx packet length
	 * 	- tx packet length
	 */
	union {
		uint8_t rx_length;
		uint8_t tx_length;

	}length; // The useful length data.

	/**
	 * This is the received address or transmit address container.
	 * It could contain either:
	 * 	- transmit address
	 * 	- received address
	 */
	union {

		uint8_t transmit_address;
		uint8_t received_address;

	}address; // useful address pointer.

	/**
	 * This is the valid flag.
	 */
	uint8_t valid_packet;
};

/**
 * This is the valid check enum
 */
enum valid_t{
	VALID,
	INVALID
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
		virtual void begin();

		//! Variation of the init method.
		virtual void begin(uint8_t address);

	// Private context
	private:

		/**
		 * We define the internal buffer type needed.
		 */
		i2c_packet buffer;

		/**
		 * This is the valid container.
		 */
		valid_t valid;

		/**
		 * This is the deconstructor for the class.
		 */
		~base_i2c_driver();

		// -- Reading methods

		/**
		 * This method reads one byte from the i2c pipe.
		 *
		 * @param buffer				- the i2c packet buffer struct pointer
		 * @return pointer				- the i2c_packet pointer of data
		 */
		i2c_packet* read_byte(i2c_packet* buffer);

		/**
		 * This method reads many bytes from the i2c pipe.
		 *
		 * @param buffer				- the i2c packet buffer struct pointer
		 * @return pointer				- the i2c_packet pointer of data
		 */
		i2c_packet* read_byte(i2c_packet* buffer);

		/**
		 * This method reads an integer from the i2c pipe.
		 *
		 * @param buffer				- the i2c packet buffer struct pointer
		 * @return pointer				- the i2c_packet pointer of data
		 */
		i2c_packet* read_int(i2c_packet* buffer);


		/**
		 * This method reads an unsigned integer from the i2c pipe.
		 *
		 * @param buffer				- the i2c packet buffer struct pointer
		 * @return pointer				- the i2c_packet pointer of data
		 */
		i2c_packet* read_uint(i2c_packet* buffer);

		// -- Writing methods

		/**
		 * This method writes one byte on the i2c pipe.
		 *
		 * @param buffer				- the i2c packet buffer struct pointer
		 */
		void write_byte(i2c_packet* buffer);

		/**
		 * This method writes many bytes on the i2c pipe.
		 *
		 * @param buffer				- the i2c packet buffer struct pointer
		 */
		void write_byte(i2c_packet* buffer);

		/**
		 * This method writes an unsigned integer from the i2c pipe.
		 *
		 * @param buffer				- the i2c packet buffer struct pointer
		 * @return pointer				- the i2c_packet pointer of data
		 */
		void write_uint(i2c_packet* buffer);

		/**
		 * This method writes an integer from the i2c pipe.
		 *
		 * @param buffer				- the i2c packet buffer struct pointer
		 * @return pointer				- the i2c_packet pointer of data
		 */
		void write_int(i2c_packet* buffer);
};

#endif /* I2CDRIVER_H_ */
