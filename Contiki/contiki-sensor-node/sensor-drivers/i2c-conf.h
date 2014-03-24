/*! \file i2c.h \brief I2C (TWI) interface configuration. */
//*****************************************************************************
//
// File Name	: 'i2cconf.h'
// Title		: I2C (TWI) interface configuration
// Author		: Pascal Stang - Copyright (C) 2002-2003
// Created		: 2002.06.25
// Revised		: 2003.03.02
// Version		: 0.7
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#ifndef I2CCONF_H
#define I2CCONF_H

// define I2C data buffer sizes
// These buffers are used in interrupt-driven Master sending and receiving,
// and in slave sending and receiving.  They must be large enough to store
// the largest I2C packet you expect to send and receive, respectively.
#define I2C_SEND_DATA_BUFFER_SIZE		0x20
#define I2C_RECEIVE_DATA_BUFFER_SIZE	0x20

// define the hardware sensor addresses on the i2c bus.
// we use these to address each and every device and get
// specific data from each sensor.
// the sensor addresses are then concatenated in an address array,
// where they are globally accessible.
#define LOCAL_ADDRESS					0x00

// TSL2561
//	- ambient light
//	- IR light
// The address pin is put to ground
#define LIGHT_SENSOR_ADDRESS			0x39

// HIH6130
//	- temperature
//	- humidity
// The
#define HUMIDITY_SENSOR_ADDRESS			0x27

// These parameters are measured by 1 sensor
// BMP180
//	- temperature
//	- pressure
//	- altitude
#define TEMPERATURE_SENSOR_ADDRESS		0x77
#define PRESSURE_SENSOR_ADDRESS			TEMPERATURE_SENSOR_ADDRESS
#define ALTITUDE_SENSOR_ADDRESS			TEMPERATURE_SENSOR_ADDRESS

// Buffer eeprom address
// AT24C32D - 32Kbits
#define BUFFER_EEPROM_ADDRESS			0xA0

// This is done by the on board adc
#define ONBOARD_TEMPERATURE				0x01
#define BATTERY_SENSOR_ADDRESS			0x01

#define WRITE							0xFE

// global list of sensor addresses.
const unsigned char device_addresses[8] = {
										LOCAL_ADDRESS,
										LIGHT_SENSOR_ADDRESS,
										HUMIDITY_SENSOR_ADDRESS,
										TEMPERATURE_SENSOR_ADDRESS,
										PRESSURE_SENSOR_ADDRESS,
										ALTITUDE_SENSOR_ADDRESS,
										BATTERY_SENSOR_ADDRESS,
										BUFFER_EEPROM_ADDRESS
										};

// compiler warning, where the sensor addresses cannot be 0x00.
#if (LIGHT_SENSOR_ADDRESS == 0x00) || (HUMIDITY_SENSOR_ADDRESS == 0x00) || \
	(TEMPERATURE_SENSOR_ADDRESS == 0x00) || (PRESSURE_SENSOR_ADDRESS == 0x00) || \
	(ALTITUDE_SENSOR_ADDRESS == 0x00) || (BATTERY_SENSOR_ADDRESS == 0x00) || \
	(BUFFER_EEPROM_ADDRESS == 0x00) || (ONBOARD_TEMPERATURE == 0x00)
	#warning "Device addresses need to be implemented"
#endif
#endif
