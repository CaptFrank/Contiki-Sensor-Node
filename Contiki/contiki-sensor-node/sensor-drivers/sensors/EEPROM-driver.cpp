
#include "EEPROM-driver.h"

/**
 * This is the default constructor for the class
 */
EEPROM::EEPROM(){

	//! Set the default values
	this->_address = 0;
	this->_current_sector = 0;
	this->_current_space = TOTAL_EEPROM_SPACE;
	this->_eeprom_rx_packet = 0;
	this->_valid_flag = 1;
}

/**
 * This method starts the eeprom driver.
 *
 * @param address					- the device address
 */
void EEPROM::begin(uint8_t address){
	this->_address = address;
}

/**
 * This method reads a byte from the eeprom storage.
 *
 * @param address					- the address to read
 * @return i2c_packet 				- the byte at that address
 */
i2c_packet* EEPROM::eeprom_read_byte(uint16_t address){

	//! Create a read request
	uint8_t data[] = {(address >> 8), (address & 0xFF)};

	//! Create the request
	read_request_t* req = this->set_rx_request(this->_address, data, sizeof(data), sizeof(uint8_t));

	//! We get the read packet
	return this->read_bytes(req);

}

/**
 * This method reads a page length on teh remote device.
 *
 * @param address					- the address to read from
 * @param length					- the length to read
 * @return i2c_packet				- the packet in the form of i2c packet
 */
i2c_packet* EEPROM::eeprom_read_page(uint16_t address, uint8_t length = DEFAULT_READ){

	//! Create a read request
	uint8_t data[] = {(address >> 8), (address & 0xFF)};

	//! Create the request
	read_request_t* req = this->set_rx_request(this->_address, data, sizeof(data), length);

	//! We get the read packet
	return this->read_bytes(req);
}

/**
 * This method writes one byte to the address specified.
 *
 * @param address					- the address to write
 * @param data						- the data to write
 */
bool EEPROM::eeprom_write_byte(uint16_t address, uint8_t data){

	//! Create the write container
	uint8_t data_container[] = {(address >> 8), (address & 0xFF), data};

	//! Create a write request
	write_request_t* req = this->set_tx_request(this->_address, data_container, sizeof(data));

	//! Valid write
	if(this->write_byte(req) == VALID){
		return true;
	}

	//! Error occured
	return false;
}

/**
 * This method writes a page of data to the remote device.
 *
 * @param address					- the address to write to
 * @param data						- the data to write
 * @param length					- the length of the data
 */
bool EEPROM::eeprom_write_page(uint16_t address, uint8_t *data, uint8_t length = DEFAULT_WRITE){

	//! Create the write container
	uint8_t data_container[length + 2];

	//! Set the data array
	data_container[0] = (address >> 8);
	data_container[1] = (address & 0xFF);
	memcpy((void*) data_container[2], (void*)data, length);

	//! Create a write request
	write_request_t* req = this->set_tx_request(this->_address, data_container, sizeof(data));

	//! Valid write
	if(this->write_byte(req) == VALID){
		return true;
	}

	//! Error occured
	return false;
}
