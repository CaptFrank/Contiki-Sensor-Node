/*
 * i2cdriver.cpp
 *
 *  Created on: 2014-03-23
 *      Author: francispapineau
 */

#include "i2c-driver.h"

/**
 * This is the default constructor for the base class.
 */
base_i2c_driver::base_i2c_driver() {

	//! Initialize the valid flag to invalid
	this->_valid = INVALID;
	this->_error = 0;

	//! Start the i2c daemon
	i2c_init();
}

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
read_request_t* base_i2c_driver::set_rx_request(uint8_t address, uint8_t* command,
							   uint8_t command_length, uint8_t read_length){

	//! Create the packet
	ENTER_CRITICAL_SECTION();
	this->rx_request.target_address = address;
	this->rx_request.target_read_command = command;
	this->rx_request.target_command_length = command_length;
	this->rx_request.target_read_length = read_length;
	EXIT_CRITICAL_SECTION();

	//! Return the request
	return this->rx_request;
}

/**
 * This method provides the interface to creating a transmit request.
 *
 * @param address				- the address to send to
 * @param data					- the data to send
 * @param data_length			- the data length
 * @return	write_request_t		- the write_request
 */
write_request_t* base_i2c_driver::set_tx_request(uint8_t address, uint8_t* data,
								uint8_t data_length){

	//! Create the packet
	ENTER_CRITICAL_SECTION();
	this->tx_request.target_address = address;
	this->tx_request.target_data = data;
	this->tx_request.target_data_length = data_length;
	EXIT_CRITICAL_SECTION();

	//! Return the request
	return this->tx_request;
}

// -- Reading methods

/**
 * This method reads one byte from the remote node.
 *
 * @param read_req				- the read request
 * @return value				- the buffer value
 */
i2c_packet* base_i2c_driver::read_bytes(struct read_request_t* read_req){

	uint8_t i = 0;

	//! The data to the read byte method.
	//! Set up command byte for read
	i2c_start(read_req->target_address, I2C_WRITE);

	while(read_req->target_command_length --){

		i2c_write(read_req->target_read_command[i]);
		i++;
	}

	//! End the transmission
	if ((this->_error = i2c_stop()) == 0){
		this->_valid = VALID;
	}else{
		this->_valid = INVALID;
	}

	//! Read requested byte
	i2c_start(read_req->target_address, I2C_READ);
	i2c_read_many(this->_buffer.buffer, (uint8_t)read_req->target_read_length, 0x00);

	this->_buffer.address = read_req->target_address;
	this->_buffer.length = read_req->target_read_length;

	//! We check the length received.
	if(read_req->target_read_length == i++){
		this->_buffer.valid_packet = true;
		this->_valid = VALID;
	}else{
		this->_valid = INVALID;
	}

	//! Send the received pointer
	return this->_buffer;
}

/**
 * This method reads one byte from the remote node.
 *
 * @param read_req				- the read request
 * @return value				- the _buffer value
 */
i2c_packet* base_i2c_driver::read_byte(struct read_request_t* read_req){

	read_req->target_read_length = sizeof(uint8_t);

	//! Get the data
	return this->read_bytes(read_req);
}

/**
 * This method reads an integer from the i2c pipe.
 *
 * @param read_req				- the read request
 * @return int					- the _buffer int
 */
int base_i2c_driver::read_int(struct read_request_t* read_req){

	//! Variable definition
	int value;
	i2c_packet* temp;

	//! Set the length
	read_req->target_read_length = sizeof(int);

	//! Get the data
	temp = this->read_bytes(read_req);

	//! Check if valid
	if(temp->valid_packet){
		value = (((int)this->_buffer.buffer[0]<<8)|(int)this->_buffer.buffer[1]);
	}

	//! return conversion
	return value;
}

/**
 * This method reads an unsigned integer from the i2c pipe.
 *
 * @param read_req				- the read request
 * @return int					- the _buffer int
 */
int base_i2c_driver::read_uint(struct read_request_t* read_req){

	//! Variable definition
	unsigned int value;
	i2c_packet* temp;

	//! Set the length
	read_req->target_read_length = sizeof(int);

	//! Get the data
	temp = this->read_bytes(read_req);

	//! Check if valid
	if(temp->valid_packet){
		value = (((unsigned int)this->_buffer.buffer[0]<<8)|(unsigned int)this->_buffer.buffer[1]);
	}

	//! return conversion
	return value;
}

// -- Writing methods

/**
 * This method writes multiple bytes to the i2c bus.
 *
 * @param write_req				- the write request
 * @return valid				- if the transaction was successful
 */
valid_t base_i2c_driver::write_bytes(struct write_request_t* write_req){

	//! Start the transmission
	i2c_start(write_req->target_address, I2C_WRITE);

	//! Write the bytes
	i2c_write_array((uint8_t*)write_req->target_data, write_req->target_data_length);

	//! End the transaction
	if ((this->_error = i2c_stop()) == 0){
		this->_valid = VALID;
	}else{
		this->_valid = INVALID;
	}
	return this->_valid;
}

/**
 * This method writes one byte on the i2c pipe.
 *
 * @param write_req				- the write request
 * @return valid				- if the transaction was successful
 */
valid_t base_i2c_driver::write_byte(struct write_request_t* write_req){

	//! Set the length to read
	write_req->target_data_length = sizeof(uint8_t);

	//! Override the call
	this->write_bytes(write_req);
	return this->_valid;
}

/**
 * This method writes an unsigned integer from the i2c pipe.
 *
* @param write_req				- the write request
 * @return valid				- if the transaction was successful
 */
valid_t base_i2c_driver::write_uint(struct write_request_t* write_req){

	//! Set the length to read
	write_req->target_data_length = sizeof(int);

	//! Override the call
	this->write_bytes(write_req);
	return this->_valid;
}

/**
 * This method writes an integer from the i2c pipe.
 *
 * @param address				- the remote device
 * @param data					- the pointer to the data to be sent
 * @return valid				- if the transaction was successful
 */
valid_t base_i2c_driver::write_int(struct write_request_t* write_req){

	//! Set the length to read
	write_req->target_data_length = sizeof(int);

	//! Override the call
	this->write_bytes(write_req);
	return this->_valid;
}

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
uint8_t base_i2c_driver::get_error(){
	return _error;
}

