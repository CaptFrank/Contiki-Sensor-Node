#include "DHT11-driver.h"
#include <Arduino.h>
// Only micros is used from Arduino.h

// Public context
/**
 * This is the default constructor for the class.
 */
DHT11::DHT11(void* port, uint8_t pin){

	this->_pin = pin;
	this->_port = port;
	this->_humidity = 0;
	this->_temperature = 0;

}


/**
 * This method reads channel 11.
 *
 * @param pin				- the channel pin
 * @return int				- the value read
 */
int DHT11::read11(uint8_t pin){

    //! READ VALUES
    int rv = read(pin);
    if (rv != DHTLIB_OK){

        this->_humidity    = DHTLIB_INVALID_VALUE; // invalid value, or is NaN prefered?
        this->_temperature = DHTLIB_INVALID_VALUE; // invalid value
        return rv;
    }

    //! CONVERT AND STORE
    this->_humidity    = bits[0];  // bits[1] == 0;
    this->_temperature = bits[2];  // bits[3] == 0;

    //! TEST CHECKSUM
    //! bits[1] && bits[3] both 0
    uint8_t sum = bits[0] + bits[2];
    if (bits[4] != sum) return DHTLIB_ERROR_CHECKSUM;

    return DHTLIB_OK;
}

/**
 * This method reads channel 21.
 *
 * @param pin				- the channel pin
 * @return int				- the value read
 */
int DHT11::read21(uint8_t pin){

    return read22(pin);  //! dataformat identical to DHT22
}

/**
 * This method reads channel 22.
 *
 * @param pin				- the channel pin
 * @return int				- the value read
 */
int DHT11::read22(uint8_t pin) {

    //! READ VALUES
    int rv = read(pin);
    if (rv != DHTLIB_OK){

    	this->_humidity    = DHTLIB_INVALID_VALUE;  // invalid value, or is NaN prefered?
    	this->_temperature = DHTLIB_INVALID_VALUE;  // invalid value
        return rv; //! propagate error value
    }

    //! CONVERT AND STORE
    this->_humidity = uint16_t(bits[0], bits[1]) * 0.1;

    //! negative temperature
    if (bits[2] & 0x80) {

    	this->_temperature = -0.1 * uint16_t(bits[2] & 0x7F, bits[3]);
    }
    else{

    	this->_temperature = 0.1 * uint16_t(bits[2], bits[3]);
    }

    //! TEST CHECKSUM
    uint8_t sum = bits[0] + bits[1] + bits[2] + bits[3];
    if (bits[4] != sum) return DHTLIB_ERROR_CHECKSUM;

    return DHTLIB_OK;
}

/**
 * This method gets the humidity value calculated
 *
 * @return double			- the value
 */
double DHT11::get_humidity(){
	return this->_humidity;
}

/**
 * This method gets the temperature value calculated
 *
 * @return double			- the value
 */
double DHT11::get_temperature(){
	return this->_temperature;
}

// Private context

/**
 *	This is the read method to read from a pin
 *
 * @param pin				- the channel pin
 * @return int				- the value read
 */
int DHT11::read(uint8_t pin){

    //! INIT BUFFERVAR TO RECEIVE DATA
    uint8_t mask = 128;
    uint8_t idx = 0;

    //! EMPTY BUFFER
    for (uint8_t i=0; i< 5; i++) bits[i] = 0;

    //! REQUEST SAMPLE
    pinModeSet(this->_port, this->_pin, outputMode);
    pinSet(this->_port, this->_pin, LOW);

    _delay_ms(20);

    pinSet(this->_port, this->_pin, HIGH);

    _delay_us(40);

    pinModeSet(this->_port, this->_pin, inputModeWithPullup);

    //! GET ACKNOWLEDGE or TIMEOUT
    unsigned int loopCnt = TIMEOUT;
    while(pinRead(this->_port, this->_pin) == LOW)
    if (--loopCnt == 0) return DHTLIB_ERROR_TIMEOUT;

    loopCnt = TIMEOUT;
    while(pinRead(this->_port, this->_pin) == HIGH)
    if (--loopCnt == 0) return DHTLIB_ERROR_TIMEOUT;

    //! READ THE OUTPUT - 40 BITS => 5 BYTES
    for (uint8_t i=0; i<40; i++){

        loopCnt = TIMEOUT;
        while(pinRead(this->_port, this->_pin) == LOW)
        if (--loopCnt == 0) return DHTLIB_ERROR_TIMEOUT;

        unsigned long t = micros();

        loopCnt = TIMEOUT;
        while(pinRead(this->_port, this->_pin) == HIGH)
        if (--loopCnt == 0) return DHTLIB_ERROR_TIMEOUT;

        if ((micros() - t) > 40) bits[idx] |= mask;
        mask >>= 1;
        if (mask == 0){

            mask = 128;
            idx++;
        }
    }

    return DHTLIB_OK;
}
