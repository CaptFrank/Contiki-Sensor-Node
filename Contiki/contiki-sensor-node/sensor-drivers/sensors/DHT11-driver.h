/*
 * HIH6130-driver.h
 *
 *  Created on: 2014-03-18
 *      Author: francispapineau
 */

#ifndef SHT15_DRIVER_H_
#define SHT15_DRIVER_H_

//! Includes
#include "pinControlLib.h"
#include "../i2c-driver.h"

//! Defines
#define DHT_LIB_VERSION 		"0.1.09"

#define DHTLIB_OK				0
#define DHTLIB_ERROR_CHECKSUM	-1
#define DHTLIB_ERROR_TIMEOUT	-2
#define DHTLIB_INVALID_VALUE	-999

//! #define TIMEOUT 10000
//! uint16_t for UNO, higher CPU speeds => exceed MAXINT.
//! works for DUE
//! 16 MHz => 10000
//! 84 MHz => 52500
//! 100MHz => 62500
#define TIMEOUT (F_CPU/1600)

/**
 * This class is the humidity sensor handler class,
 * it extends the i2c driver class.
 */
class DHT11 {

	// Public Context
	public:

	/**
	 * This is the default constructor for the class.
	 */
	DHT11(void* port, uint8_t pin);

	/**
	 * This is the default virtual deconstructor for the class.
	 */
	virtual void ~DHT11();

	/**
	 * This method reads channel 11.
	 *
	 * @param pin				- the channel pin
	 * @return int				- the value read
	 */
	int read11(uint8_t pin);

	/**
	 * This method reads channel 21.
	 *
	 * @param pin				- the channel pin
	 * @return int				- the value read
	 */
	int read21(uint8_t pin);

	/**
	 * This method reads channel 22.
	 *
	 * @param pin				- the channel pin
	 * @return int				- the value read
	 */
	int read22(uint8_t pin);

	/**
	 * This method gets the humidity value calculated
	 *
	 * @return double			- the value
	 */
    double get_humidity();

    /**
	 * This method gets the temperature value calculated
	 *
	 * @return double			- the value
     */
    double get_temperature();

    // Private Context
	private:

    /**
     * The port of the pin
     */
    void* _port;

    /**
     * The one wire pin
     */
    uint8_t _pin;

    /**
     * The internal stored values
     */
    double _humidity, _temperature;

    /**
     * buffer to receive data
     */
    uint8_t bits[5];

    /**
     *	This is the read method to read from a pin
     *
	 * @param pin				- the channel pin
	 * @return int				- the value read
     */
    int read(uint8_t pin);
};


#endif /* HIH6130_DRIVER_H_ */
