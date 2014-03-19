/*
 * shell-i2c-driver.h
 *
 *  Created on: 2014-03-17
 *      Author: francispapineau
 */

#ifndef SHELL_I2C_DRIVER_H_
#define SHELL_I2C_DRIVER_H_

#include "shell-i2c-conf.h"
#include "i2c.h"

/**
 * This is the buffer typedef that is used to structure buffered
 * i2c operations, such as read and writes.
 * It comprises of the buffer array, then length of the buffer
 * and the received packet address.
 */
struct buffer_t {
	uint8_t buffer[I2C_SEND_DATA_BUFFER_SIZE];
	uint8_t length;
	uint8_t received_address;
}buffer;

/**
 * This is the bus initializer function.
 */
void init_i2c_bus(void);

/**
 * This function is the i2c read function which reads a specified number of
 * bytes from the i2c bus.
 *
 * @param receive_data_length 				- the number of bytes to read
 * @param receive_data						- the received data pointer
 */
void handle_i2c_read(uint8_t receive_data_length, uint8_t* receive_data);

/**
 * This function transmits a specific number of bytes given.
 *
 * @param transmit_data_length_max			- the max data length to send
 * @param transmit_data						- the data pointer to send
 * @return length							- returns the number of bytes sent
 */
uint8_t handle_i2c_transmit(uint8_t transmit_data_length_max, uint8_t* transmit_data);


#endif /* SHELL_I2C_DRIVER_H_ */
