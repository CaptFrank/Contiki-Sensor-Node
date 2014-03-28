
#ifndef TSL2561_h
#define TSL2561_h

#include "../i2c-driver.h"

//! Definition of the TSL2561 registers
#define TSL2561_CMD           0x80
#define TSL2561_CMD_CLEAR     0xC0
#define	TSL2561_REG_CONTROL   0x00
#define	TSL2561_REG_TIMING    0x01
#define	TSL2561_REG_THRESH_L  0x02
#define	TSL2561_REG_THRESH_H  0x04
#define	TSL2561_REG_INTCTL    0x06
#define	TSL2561_REG_ID        0x0A
#define	TSL2561_REG_DATA_0    0x0C
#define	TSL2561_REG_DATA_1    0x0E

/**
 * This is the light sensor handler driver. It handles the
 * communication between the light sensor and the cpu. The
 * data processing and acquisition is done through this class.
 *
 * This class extends the base i2c driver class.
 */
class TSL2561 : public base_i2c_driver {

	// Public context
	public:

	/**
	 * This is the default constructor for the class
	 */
	TSL2561(void);
			
	/**
	 * This is the virtual default deconstructor for the class.
	 */
	virtual ~TSL2561(void);

	/**
	 * This is the initializer method for the i2c communication for the
	 * remote device.
	 *
	 * @return true									- the install status
	 */
	bool begin();


	char setPowerUp(void);
			// Turn on TSL2561, begin integration
			// Returns true (1) if successful, false (0) if there was an I2C error
			// (Also see getError() below)

		char setPowerDown(void);
			// Turn off TSL2561
			// Returns true (1) if successful, false (0) if there was an I2C error
			// (Also see getError() below)

		char setTiming(char gain, unsigned char time);
			// If gain = false (0), device is set to low gain (1X)
			// If gain = high (1), device is set to high gain (16X)
			// If time = 0, integration will be 13.7ms
			// If time = 1, integration will be 101ms
			// If time = 2, integration will be 402ms
			// If time = 3, use manual start / stop
			// Returns true (1) if successful, false (0) if there was an I2C error
			// (Also see getError() below)

		char setTiming(char gain, unsigned char time, unsigned int &ms);
			// Identical to above command, except ms is set to selected integration time
			// If gain = false (0), device is set to low gain (1X)
			// If gain = high (1), device is set to high gain (16X)
			// If time = 0, integration will be 13.7ms
			// If time = 1, integration will be 101ms
			// If time = 2, integration will be 402ms
			// If time = 3, use manual start / stop (ms = 0)
			// ms will be set to requested integration time
			// Returns true (1) if successful, false (0) if there was an I2C error
			// (Also see getError() below)

		char manualStart(void);
			// Starts a manual integration period
			// After running this command, you must manually stop integration with manualStop()
			// Internally sets integration time to 3 for manual integration (gain is unchanged)
			// Returns true (1) if successful, false (0) if there was an I2C error
			// (Also see getError() below)

		char manualStop(void);
			// Stops a manual integration period
			// Returns true (1) if successful, false (0) if there was an I2C error
			// (Also see getError() below)

		char getData(unsigned int &CH0, unsigned int &CH1);
			// Retrieve raw integration results
			// data0 and data1 will be set to integration results
			// Returns true (1) if successful, false (0) if there was an I2C error
			// (Also see getError() below)
			
		char getLux(unsigned char gain, unsigned int ms, unsigned int CH0, unsigned int CH1, double &lux);
			// Convert raw data to lux
			// gain: 0 (1X) or 1 (16X), see setTiming()
			// ms: integration time in ms, from setTiming() or from manual integration
			// CH0, CH1: results from getData()
			// lux will be set to resulting lux calculation
			// returns true (1) if calculation was successful
			// RETURNS false (0) AND lux = 0.0 IF EITHER SENSOR WAS SATURATED (0XFFFF)

		/**
		 * Sets up interrupt operations
		 *
		 * If control = 0, interrupt output disabled
		 * If control = 1, use level interrupt, see setInterruptThreshold()
		 * If persist = 0, every integration cycle generates an interrupt
		 * If persist = 1, any value outside of threshold generates an interrupt
		 * If persist = 2 to 15, value must be outside of threshold for 2 to 15 integration cycles
		 *
		 * @param control						- the control variable
		 * @param persist						- the persits variable
		 * @return bool							- success
		 */
		bool set_interrupt_control(unsigned char control, unsigned char persist);

		/**
		 * Set interrupt thresholds (channel 0 only)
		 *
		 * @param low							- the low threshold value
		 * @param high							- the high threshold value
		 * @return bool							- the success
		 */
		bool set_interrupt_threshold(unsigned int low, unsigned int high);

		/**
		 * This method clears the remote device interrupt flag.
		 *
		 * @return success						- the success in clearing the flag
		 */
		bool clear_interrupt(void);

		/**
		 * This method gets the id of the sensor.
		 *
		 * @return i2c_packet 					- the id of the sensor
		 */
		i2c_packet* get_ID(void);
			
	// Private context
	private:

	/**
	 * This is the remote device address
	 */
	char _address;
};

#endif
