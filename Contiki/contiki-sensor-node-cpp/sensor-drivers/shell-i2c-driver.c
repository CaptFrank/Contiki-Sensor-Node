/*
 * shell-i2c-driver.c
 *
 *  Created on: 2014-03-17
 *      Author: francispapineau
 */

#include "shell-i2c-driver.h"
#include "i2c-conf.h"

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

	// We return to the main context
	return;
}

/**
 * Read bytes function
 * @param buffer								- values container
 */
void read_bytes(struct buffer_struct* buffer){

	i2cSendStart();
	i2cWaitForComplete();
	i2cSendByte(TEMPERATURE_SENSOR_ADDRESS & WRITE);
	i2cWaitForComplete();
	i2cSendByte(buffer->buffer[0]);
	i2cWaitForComplete();
	i2cSendStop();
	i2cMasterReceive(buffer->received_address = TEMPERATURE_SENSOR_ADDRESS, buffer->length, buffer->buffer);
	i2cWaitForComplete();
}

/**
 * Write bytes function
 * @param buffer								- values container
 */
void write_bytes(struct buffer_struct* buffer){

	i2cSendStart();
	i2cWaitForComplete();
	i2cSendByte(TEMPERATURE_SENSOR_ADDRESS & WRITE);
	i2cWaitForComplete();
	i2cMasterSend(buffer->received_address = TEMPERATURE_SENSOR_ADDRESS, buffer->length, buffer->buffer);
	i2cSendStop();
}

/**
 * The read one byte function
 * @param address								- the remote address
 * @return byte									- the read byte
 */
uint8_t read_byte(uint8_t address){

	i2cSendStart();
	i2cWaitForComplete();
	i2cReceiveByte(0x01);
	i2cWaitForComplete();
	i2cSendStop();

	return i2cGetReceivedByte();
}

/**
 * The write byte function
 * @param address								- the address to send
 * @param byte									- the byte to send
 */
void write_byte(uint8_t address, uint8_t byte){

	i2cSendStart();
	i2cWaitForComplete();
	i2cSendByte(byte);
	i2cWaitForComplete();
	i2cSendStop();
}


int readInt(char address)
// Read a signed integer (two bytes) from device
// address: register to start reading (plus subsequent register)
// value: external variable to store data (function modifies value)
{
	buffer.buffer[0] = address;
	buffer.length = 2;

	read_bytes(&buffer);
	int value = (((int)buffer.buffer[0]<<8)|(int)buffer.buffer[1]);
	//if (*value & 0x8000) *value |= 0xFFFF0000; // sign extend if negative
	return(value);
}

int readUInt(char address)
// Read an unsigned integer (two bytes) from device
// address: register to start reading (plus subsequent register)
// value: external variable to store data (function modifies value)
{

	buffer.buffer[0] = address;
	buffer.length = 2;

	read_bytes(&buffer);
	int value = (((unsigned int)buffer.buffer[0]<<8)|(unsigned int)buffer.buffer[1]);
	return(value);
}
