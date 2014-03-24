/*
 * shell-i2c-driver.h
 *
 *  Created on: 2014-03-17
 *      Author: francispapineau
 */

#ifndef SHELL_I2C_DRIVER_H_
#define SHELL_I2C_DRIVER_H_

#include <inttypes.h>

#include "i2c-conf.h"
#include "i2c.h"

/**
 * This is the buffer typedef that is used to structure buffered
 * i2c operations, such as read and writes.
 * It comprises of the buffer array, then length of the buffer
 * and the received packet address.
 */
struct buffer_struct{
	uint8_t buffer[I2C_SEND_DATA_BUFFER_SIZE];
	uint8_t length;
	uint8_t received_address;
}buffer;

/**
 * This is the bus initializer function.
 */
void init_i2c_bus(void);

/**
 * Read bytes function
 * @param buffer								- values container
 */
void read_bytes(struct buffer_struct* buffer);

/**
 * Write bytes function
 * @param buffer								- values container
 */
void write_bytes(struct buffer_struct* buffer);

/**
 * The read one byte function
 * @param address								- the remote address
 * @return byte									- the read byte
 */
uint8_t read_byte(uint8_t address);

/**
 * The write byte function
 * @param address								- the address to send
 * @param byte									- the byte to send
 */
void write_byte(uint8_t address, uint8_t byte);

/**
 * Reads an unsigned interger value
 * @param address								- the remote device
 * @return value								- value
 */
char readUInt(char address);

/**
 * Reads an unsigned interger value
 * @param address								- the remote device
 * @return value								- value
 */
char readInt(char address);

#endif /* SHELL_I2C_DRIVER_H_ */
