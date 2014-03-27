// #############################################################################
// #
// # Scriptname : DS1307new.h
// # Author     : Peter Schmelzer
// # Contributor: Oliver Kraus
// # contact    : schmelle2@googlemail.com
// # Date       : 2010-11-01
// # Version    : 0.2
// # License    : cc-by-sa-3.0
// #
// # Description:
// # Headerfile for the DS1307new Library
// # 
// #############################################################################
// *********************************************
// DEFINE
// *********************************************
#ifndef DS1307_h
#define DS1307_h

#include <Utility.h>
#include <i2c-driver.h>

/**
 * This is the time structure which holds the current time
 * data from the RTC.
 */
typedef struct time_t {

    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t dow;			// day of week, 0 = sunday
    uint8_t day;
    uint8_t month;
    uint16_t year;

    uint16_t ydn;		// day within the year (year day number, starts with 1 = 1. Jan)
    uint16_t cdn;		// days after 2000-01-01 (century day number, starts with 0)
    uint32_t time2000;	// seconds after 2000-01-01 00:00 (max value: 2136-02-07 06:28:15)
};

/**
 * This is the RTC handler class.
 * We use this class to comunicate with the RTC on the TWI
 * interface. It extends the base i2c driver as it is an i2c driver.
 */
class DS1307 : public base_i2c_driver{

	// Public Context
	public:

	/**
	 * This is the default constructor for the class.
	 */
    DS1307();

    /**
     * This is the default virtual deconstructor for the class.
     */
    virtual ~DS1307();

    /**
     * This is the overriden begin method found in the base i2c driver
     * class. It sets the appropriate addresses.
     *
     * @param address							- the RTC address
     * @return valid_start						- is it a valid start
     */
    bool begin(uint8_t address);

    /**
     * This method starts the clock ticks.
     */
    void start_clock(void);

    /**
     * This method stops the clock ticks
     */
    void stop_clock(void);

    /**
     * This method sets the time from the internal time struct
     */
    void set_time(void);

    /**
     * This method gets the time in the internal time struct
     */
    void get_time(void);

    /**
     * This method gets the control bit internally.
     */
    i2c_packet* get_CTRL(void);

    /**
     * This method sets the control bit externally.
     */
    void set_CTRL(void);

    /**
     * This method gets a segment of ram from the RTC.
     *
     * @param rtc_addr 						- the rtc address
     * @param rtc_ram						- the rtc ram pointer
     * @param rtc_quantity					- the length to get
     */
    i2c_packet* get_RAM(uint8_t rtc_addr, uint8_t rtc_quantity);

    /**
     * This method sets a segment of ram in the RTC.
     *
     * @param rtc_addr 						- the rtc address
     * @param rtc_ram						- the rtc ram pointer
     * @param rtc_quantity					- the length to set
     */
    void set_RAM(uint8_t rtc_addr, uint8_t * rtc_ram, uint8_t rtc_quantity);
  
    /**
     * This fills the ram based on CDN.
     *
     *   Variable updates:
     *		cdn, ydn, year, month, day
     *
     * @param _cdn							- cdn value
     */
    void fill_by_CDN(uint16_t _cdn);

    /**
     * This fills the ram by time from 2000
     *
     *   Variable updates:
     *		time2000, cdn, ydn, year, month, day, hour, minute, second
     *
     * @param _time2000						- the time since 2000
     */
    void fill_by_time_2000(uint32_t _time2000);

    /**
     * This fills the ram based on HMS time.
     *
     * @param h								- hours
     * @param m								- minutes
     * @param s								- seconds
     */
    void fill_by_HMS(uint8_t h, uint8_t m, uint8_t s);

    /**
     * This fills the rma based on the YMD time
     *
     * @param y								- year
     * @param m								- month
     * @param d								- day
     */
    void fill_by_YMD(uint16_t y, uint8_t m, uint8_t d);

    /**
     * This checks if it is day light savings or not
     *
     * @return bool							- yes or no
     */
    uint8_t is_MEZ_Summer_Time(void);

    /**
     * This gets the internal time struct.
     *
     * @return time_t 						- the pointer to the internal time struct
     */
    time_t* return_time();

    /**
     * This gets the private variable ctl.
     *
     * @return uint8_t						- the control flag
     */
    uint8_t return_ctl();

    // Private context
	private:

    /**
     * The address of the device
     */
    uint8_t _address;

    /**
     * This is the internal time struct
     */
    time_t _time;

    /**
     * This is the control register from the RTC.
     */
    uint8_t _ctrl;

    /**
     * This checks if its a leap year
     *
     *   Prototype:
     *		uint8_t DS1307new::is_leap_year(uint16_t y)
  	 * 	 Description:
     *		Calculate leap year
  	 * 	 Arguments:
     *		y   		year, e.g. 2011 for year 2011
  	 * 	 Result:
     *		0           not a leap year
     *		1           leap year
     *
     * @param y								- year
     * @return uint8_t 						- yes or no
     */
    uint8_t is_leap_year(uint16_t y);

    /**
     * This calculates ydn from year, month & day
     *
     *   Prototype:
     *		void calculate_ydn(void)
  	 * 	  Description:
     *		Calculate the day number within a year. 1st of Jan has the number 1.
     *		"Robertson" Algorithm
  	 * 	  Arguments:
     *		this->year           	year, e.g. 2011 for year 2011
     *		this->month         	month with 1 = january to 12 = december
     *		this->day          	day starting with 1
  	 * 	  Result:
     * 		this->ydn		The "day number" within the year: 1 for the 1st of Jan.
     *
     */
    void calculate_ydn(void);

    /**
     * This calculates cdn from year & ydn
     *
     *   Prototype:
     *		uint16_t to_century_day_number(uint16_t y, uint16_t ydn)
  	 * 	 Description:
     *		Calculate days since January, 1st, 2000
  	 * 	  Arguments:
     *		this->y           year, e.g. 2011 for year 2011
     *		this->ydn	year day number (1st of Jan has the number 1)
  	 * 	  Result
     *		this->cdn	days since 2000-01-01 (2000-01-01 has the cdn 0)
     */
    void calculate_cdn(void);

    /**
     * This calculates dow from ydn
     *
     *   calculate day of week (dow)
     * 	  0 = sunday .. 6 = saturday
     * 	  Arguments:
     *   	this->cdn	days since 2000-01-01 (2000-01-01 has the cdn 0 and is a saturday)
     */
    void calculate_dow(void);

    /**
      * This calculates time2000 from cdn, hour, minute & second
      *
      *   Calculate the seconds after 2000-01-01 00:00. The largest possible
      *	  time is 2136-02-07 06:28:15
      *	  Arguments:
      *  	this->h         hour
      *  	this->m	minutes
      *  	this->s		seconds
      *  	this->cdn	days since 2000-01-01 (2000-01-01 has the cdn 0)
      */
    void calculate_time2000(void);

    /**
     * This calculates the corrected year by day number.
     *
     * @return corrected_year				- the year corrected
     */
    uint16_t _corrected_year_day_number(void);

    /**
     * This calculates the month by year and ydn
     */
    void calculate_month_by_year_and_ydn(void);

    /**
     * This calculates the day by month year and ydn
     */
    void calculate_day_by_month_year_and_ydn(void);

};
#endif
