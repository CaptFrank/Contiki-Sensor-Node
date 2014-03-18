/*
 * shell-i2c-driver.h
 *
 *  Created on: 2014-03-17
 *      Author: francispapineau
 */

#ifndef SHELL_I2C_DRIVER_H_
#define SHELL_I2C_DRIVER_H_

#include "shell-sensor-addresses.h"
#include "shell-i2c-conf.h"
#include "i2c.h"

struct buffer_t {

	uint8_t local_buffer[I2C_SEND_DATA_BUFFER_SIZE];
	uint8_t received_address;
	uint8_t valid_packet;
	uint8_t length;
}buffer;


void init_i2c_bus(void);
void handle_i2c_request(uint8_t receiveDataLength, uint8_t* receiveData);
uint8_t handle_i2c_transmit(uint8_t transmitDataLengthMax, uint8_t* transmitData);


#endif /* SHELL_I2C_DRIVER_H_ */
