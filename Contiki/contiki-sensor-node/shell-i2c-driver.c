/*
 * shell-i2c-driver.c
 *
 *  Created on: 2014-03-17
 *      Author: francispapineau
 */

#include "shell-i2c-driver.h"
#include <avr/io.h>

void init_i2c_bus(void){

	// We start the I2C bus driver
	i2cInit();

	// Set the local device address
	i2cSetLocalDeviceAddr(LOCAL_ADDRESS);

	// We register the I2C receiver service
	i2cSetSlaveReceiveHandler(handle_i2c_request);

	// We register the I2C transmitter service
	i2cSetSlaveTransmitHandler(handle_i2c_transmit);

	// We return to the main context
	return;
}

void handle_i2c_request(uint8_t receive_data_length, uint8_t* receive_data){

	uint8_t i;

	// this function will run when a master somewhere else on the bus
	// addresses us and wishes to write data to us

	cbi(PORTB, PB6);

	// copy the received data to a local buffer
	for(i=0; i<receive_data_length; i++){
		buffer.local_buffer[i] = *receive_data++;
	}
	buffer.length = receive_data_length;
	return;
}

uint8_t handle_i2c_transmit(uint8_t transmit_data_length_max, uint8_t* transmit_data){

	uint8_t i;

	// this function will run when a master somewhere else on the bus
	// addresses us and wishes to read data from us

	cbi(PORTB, PB7);

	// copy the local buffer to the transmit buffer
	for(i=0; i<buffer.length; i++){
		*transmit_data++ = buffer.local_buffer[i];
	}
	buffer.local_buffer[0]++;
	return buffer.length;
}

