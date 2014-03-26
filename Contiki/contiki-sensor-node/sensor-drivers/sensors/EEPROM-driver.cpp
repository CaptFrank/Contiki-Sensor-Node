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

  You should have readd a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

/******************************************************************************
 * Includes
 ******************************************************************************/

//#include "WConstants.h"
#include "EEPROM-driver.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/

/******************************************************************************
 * Constructors
 ******************************************************************************/
EEPROM::EEPROM(){}

/******************************************************************************
 * User API
 ******************************************************************************/

void EEPROM::begin(uint8_t address){
	this->_address = address;
}

uint8_t EEPROM::read(unsigned int addr)
{
  uint8_t rdata = 0x00;
  Wire.beginTransmission(this->address);
  Wire.write((int)(addr >> 8)); // MSB
  Wire.write((int)(addr & 0xFF)); // LSB
  Wire.endTransmission();
  Wire.requestFrom(this->address, 1);
  if (Wire.available()) rdata = Wire.read();
  return rdata;
}

void EEPROM::readPage(unsigned int addr, uint8_t *buffer, int length = 16)
{
  Wire.beginTransmission(this->address);
  Wire.write((int)(addr >> 8)); // MSB
  Wire.write((int)(addr & 0xFF)); // LSB
  Wire.endTransmission();
  Wire.requestFrom(this->address,length);
  int c = 0;
  for ( c = 0; c < length; c++ )
    if (Wire.available()) buffer[c] = Wire.read();
}

void EEPROM::write(unsigned int addr, uint8_t value)
{ 
  Wire.beginTransmission(this->address);
  Wire.write((int)(addr >> 8)); // MSB
  Wire.write((int)(addr & 0xFF)); // LSB
  Wire.write((int)value);
  Wire.endTransmission();
}

void EEPROM::writePage(unsigned int addr, uint8_t *data, int length = 16)
{
  Wire.beginTransmission(this->address);
  Wire.write((int)(addr >> 8)); // MSB
  Wire.write((int)(addr & 0xFF)); // LSB
  int c;
  for ( c = 0; c < length; c++)
    Wire.write(data[c]);
  Wire.endTransmission();
}
