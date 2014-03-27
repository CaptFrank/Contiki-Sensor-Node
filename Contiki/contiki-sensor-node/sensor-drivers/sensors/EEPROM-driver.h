/*
  EEEPROM.h - External (I2C) EEPROM libray.
  Copyright (c) 2009 Amadeusz Jasak.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef EEEPROM_h
#define EEEPROM_h

#include <inttypes.h>
#include <i2c-driver.h>

#define TOTAL_EEPROM_SPACE			4000 * 1000

#define DEFAULT_READ				16
#define DEFAULT_WRITE				16

/**
 * This class is the eeprom handler. It provides the standard interface for the
 * state saver to save the data to the buffer.
 */
class EEPROM : public base_i2c_driver{

	// Private Context
	private:

	/**
	 * The internal address of the eeprom.
	 */
    uint8_t _address;

    /**
     * The current size of the eeprom
     */
    uint16_t _current_space;

    /**
     * The current address pointer to the eeprom sector
     */
    uint16_t _current_sector;

    /**
     * The global memory space valid flag
     */
    uint8_t _valid_flag :1;

    /**
     * The rx packet handle
     */
    i2c_packet* _eeprom_rx_packet;

    // Public context
	public:

    /**
     * This is the default constructor for the class
     */
    EEPROM();

    /**
     * This is the default virtual decronstructor for the class
     */
    virtual ~EEPROM();

    /**
     * This method starts the eeprom driver.
     *
     * @param address					- the device address
     */
    void begin(uint8_t address);
  
    /**
     * This method reads a byte from the eeprom storage.
     *
     * @param address					- the address to read
     * @return i2c_packet 				- the byte at that address
     */
    i2c_packet* eeprom_read_byte(uint16_t address);
    
    /**
     * This method writes one byte to the address specified.
     *
     * @param address					- the address to write
     * @param data						- the data to write
     */
    bool eeprom_write_byte(uint16_t address, uint8_t data);

    /**
     * This method reads a page length on teh remote device.
     *
     * @param address					- the address to read from
     * @param length					- the length to read
     * @return i2c_packet				- the packet in the form of i2c packet
     */
    i2c_packet* eeprom_read_page(uint16_t address, uint8_t length);

    /**
     * This method writes a page of data to the remote device.
     *
     * @param address					- the address to write to
     * @param data						- the data to write
     * @param length					- the length of the data
     */
    bool eeprom_write_page(uint16_t address, uint8_t *data, uint8_t length);
};

#endif

