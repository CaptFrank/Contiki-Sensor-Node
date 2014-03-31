
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

#define TSL2561_CMD_MACRO(x) ((x & 0x0F) | TSL2561_CMD)

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


	/**
	 * Turn on TSL2561, begin integration
	 */
	void set_power_up(void);

	/**
	 * Turn off TSL2561
	 */
	void set_power_down(void);

	/**
	 * This method sets the gain and the integration time
	 *
	 * If gain = false (0), device is set to low gain (1X)
	 * If gain = high (1), device is set to high gain (16X)
	 * If time = 0, integration will be 13.7ms
	 * If time = 1, integration will be 101ms
	 * If time = 2, integration will be 402ms
	 * If time = 3, use manual start / stop
	 *
	 * @param gain							- the gain
	 * @param time							- the time
	 */
	void set_timing(uint8_t gain, uint8_t time);

	/**
	 * This method starts a manual integration period.
	 *
	 * @return								- if the command was successful
	 */
	bool manual_start(void);

	/**
	 * This method stops a manual integration period.
	 *
	 * @return bool							- if the command was successful
	 */
	bool manual_stop(void);

	/**
	 * This method gets the raw data from the I2C bus and converts them into internal
	 * data.
	 *
	 * @return bool							- if the data acquisition was successful
	 */
	bool get_data();

	/**
	 * This method converts a lux value into a double value and stores it internally.
	 *
	 * @param gain							- 0 = 1X and 1 = 16X
	 * @param ms							- the intergration time in ms
	 * @param CH0							- result from get_data()
	 * @param CH1							- result from get_data()
	 */
	void convert_lux(uint8_t gain, uint16_t ms, uint16_t CH0, uint16_t CH1);

	/**
	 * This is the getter method for getting the lux value stored internally
	 *
	 * @return double						- the lux value
	 */
	double get_lux();

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

	/**
	 * The data from each channel
	 */
	uint16_t _data1, _data0;

	/**
	 * The lux value
	 */
	double _lux;

	/**
	 * The internal setting
	 */
	uint8_t _timing;

	/**
	 * This is the read_timing method that stored the timing variable internally
	 *
	 * @return i2c_packet					- the packet read
	 */
	i2c_packet* read_timing();

	/**
	 * This is the writes_timing method that stored the timing variable internally
	 *
	 * @return bool							- the success
	 */
	bool write_timing();
};

#endif
