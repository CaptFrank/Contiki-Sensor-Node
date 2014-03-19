/*
 * shell-i2c-driver.c
 *
 *  Created on: 2014-03-17
 *      Author: francispapineau
 */

#include "shell-i2c-driver.h"
#include "shell-i2c-conf.h"

#include <avr/io.h>

/**
 * This is the bus initializer function.
 */
void init_i2c_bus(void){

	// We start the timer
	timerInit();

	// We start the I2C bus driver
	i2cInit();

	// Set the local device address
	i2cSetLocalDeviceAddr(LOCAL_ADDRESS);

	// We register the I2C receiver service
	i2cSetSlaveReceiveHandler(handle_i2c_read);

	// We register the I2C transmitter service
	i2cSetSlaveTransmitHandler(handle_i2c_transmit);

	// We return to the main context
	return;
}

/**
 * This function is the i2c read function which reads a specified number of
 * bytes from the i2c bus.
 *
 * @param receive_data_length 				- the number of bytes to read
 * @param receive_data						- the received data pointer
 */
void handle_i2c_read(uint8_t receive_data_length, uint8_t* receive_data){

	// this function will run when a master somewhere else on the bus
	// addresses us and wishes to write data to us
	cbi(PORTB, PB6);

	// copy the received data to a local buffer
	for(uint8_t i = 0; i < receive_data_length; i++){

		// we increment uint8_t number of bytes
		buffer.buffer[i] = *receive_data++;
	}

	// we set the received data length
	buffer.length = receive_data_length;
	return;
}

/**
 * This function transmits a specific number of bytes given.
 *
 * @param transmit_data_length_max			- the max data length to send
 * @param transmit_data						- the data pointer to send
 * @return length							- returns the number of bytes sent
 */
uint8_t handle_i2c_transmit(uint8_t transmit_data_length_max, uint8_t* transmit_data){


	// this function will run when a master somewhere else on the bus
	// addresses us and wishes to read data from us
	cbi(PORTB, PB7);

	// copy the local buffer to the transmit buffer
	for(uint8_t i = 0; i < buffer.length; i++){

		// we set the transmit data buffer
		(*transmit_data++) = buffer.buffer[i];
	}

	// add 1 to 0th position
	buffer.buffer[0]++;

	// we return the length sent
	return buffer.length;
}

