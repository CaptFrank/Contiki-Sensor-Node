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

	public:
    EEPROM();
    virtual ~EEPROM();

    void begin(uint8_t address);
  
    uint8_t read(unsigned int);
    void write(unsigned int, uint8_t);
    
    void readPage(unsigned int addr, uint8_t *buffer, int length);
    void writePage(unsigned int addr, uint8_t *data, int length);
};

#endif

