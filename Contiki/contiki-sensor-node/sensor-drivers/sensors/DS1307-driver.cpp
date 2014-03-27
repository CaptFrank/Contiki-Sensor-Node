
#include "DS1307-driver.h"


// Public Context
/**
 * This is the default constructor for the class.
 */
DS1307::DS1307(){}

/**
 * This is the overriden begin method found in the base i2c driver
 * class. It sets the appropriate addresses.
 *
 * @param address							- the RTC address
 * @return valid_start						- is it a valid start
 */
bool DS1307::begin(uint8_t address){

	//! Set the internal address
	this->_address = address;

	//! We check presence
	if(check_presence(this->_address)){
		return true;
	}
	return false;
}

/**
 * This method starts the clock ticks.
 */
void start_clock(void){

	//! Create the transmit request



  Wire.beginTransmission(DS1307_ID);
  Wire.write((uint8_t)0x00);                      // Register 0x00 holds the oscillator start/stop bit
  Wire.endTransmission();
  Wire.requestFrom(DS1307_ID, 1);
  second = Wire.read() & 0x7f;       // save actual seconds and AND sec with bit 7 (sart/stop bit) = clock started
  Wire.beginTransmission(DS1307_ID);
  Wire.write((uint8_t)0x00);
  Wire.write((uint8_t)second);                    // write seconds back and start the clock
  Wire.endTransmission();
}

void DS1307::stopClock(void)         // set the ClockHalt bit high to stop the rtc
{
  Wire.beginTransmission(DS1307_ID);
  Wire.write((uint8_t)0x00);                      // Register 0x00 holds the oscillator start/stop bit
  Wire.endTransmission();
  Wire.requestFrom(DS1307_ID, 1);
  second = Wire.read() | 0x80;       // save actual seconds and OR sec with bit 7 (sart/stop bit) = clock stopped
  Wire.beginTransmission(DS1307_ID);
  Wire.write((uint8_t)0x00);
  Wire.write((uint8_t)second);                    // write seconds back and stop the clock
  Wire.endTransmission();
}

/**
 * This method gets the time in the internal time struct
 */
void DS1307::get_time(void){

	//! Create command
	uint8_t data[1] = {0x00};

	//! Create a read request
	read_request_t req = set_rx_request(RTC_ADDRESS, data, sizeof(data), 0x07);

	//! We send th read request
	i2c_packet* time = this->read_bytes(&req);

	if(time->valid_packet){

		this->_time.second = bcd2dec(time->buffer[0] & 0x7F);
		this->_time.minute = bcd2dec(time->buffer[1] & 0x7F);
		this->_time.hour = bcd2dec(time->buffer[2] & 0x7F);
		this->_time.dow = bcd2dec(time->buffer[3] & 0x7F);
		this->_time.dow --;

		this->_time.day = bcd2dec(time->buffer[4] & 0x7F);
		this->_time.month = bcd2dec(time->buffer[5] & 0x7F);
		this->_time.year = bcd2dec(time->buffer[6] & 0x7F) + 2000;

		//! Recalculate all other values
		calculate_ydn();
		calculate_cdn();
		calculate_dow();
		calculate_time2000();
	}
	return;

}

/**
 * This method sets the time from the internal time struct
 */
void DS1307::set_time(void){

	//! Create the data array
	uint8_t data[] = {0x00, dec2bcd(this->_time.second) | 0x80,
					  dec2bcd(this->_time.minute),
					  dec2bcd(this->_time.hour) & 0x3f,
					  dec2bcd(this->_time.dow+1),
					  dec2bcd(this->_time.day),
					  dec2bcd(this->_time.month),
					  dec2bcd(this->_time.year - 2000)};

	//! Create the write request
	write_request_t req = set_tx_request(RTC_ADDRESS, data, sizeof(data));

	//! We send it off
	if(this->write_bytes(&req) != VALID){
		this->_error = true;
	}
}

/**
 * This method gets the control bit internally.
 */
i2c_packet* DS1307::get_CTRL(void){

	//! Create the data
	uint8_t data[1];
	data[0] = (0x07);

	//! Create read request
	read_request_t req = set_rx_request(RTC_ADDRESS, data, sizeof(data), sizeof(data));

	//! Send the read request
	i2c_packet* i2c_packet_returned = this->read_bytes(&req);

	//! Set internal value
	this->_ctrl = i2c_packet_returned->buffer[0];

	return i2c_packet_returned;
}

/**
 * This method gets the control bit internally.
 */
void DS1307::set_CTRL(void){

	//! Create data
	uint8_t data[2] = {0x07, this->_ctrl};

	//! Create a write request
	write_request_t req = set_tx_request(RTC_ADDRESS, data, sizeof(data));

	//! We send the packet
	if(this->write_bytes(&req) != VALID){
		this->_error = true;
	}
}

/**
 * This method gets a segment of ram from the RTC.
 *
 * @param rtc_addr 						- the rtc address
 * @param rtc_quantity					- the length to get (MAX = 56 bytes)
 */
i2c_packet* DS1307::get_RAM(uint8_t rtc_addr, uint8_t rtc_quantity){

	//! Create the data
	uint8_t data[1];
	data[0] = (rtc_addr & 63) + 8;

	//! Create read request
	read_request_t req = set_rx_request(RTC_ADDRESS, data, sizeof(data), rtc_quantity);

	//! Send the read request
	return this->read_bytes(&req);
}

/**
 * This method sets a segment of ram in the RTC.
 *
 * @param rtc_addr 						- the rtc address
 * @param rtc_ram						- the rtc ram pointer
 * @param rtc_quantity					- the length to set
 */
void DS1307::set_RAM(uint8_t rtc_addr, uint8_t* rtc_ram, uint8_t rtc_quantity){

	//! Create the data
	uint8_t data[rtc_quantity + 1];
	data[0] = (rtc_addr & 63) + 8;
	memcpy((void*)data, (void*)rtc_ram, rtc_quantity);

	//! Create a write request
	write_request_t req = set_tx_request(RTC_ADDRESS, data, sizeof(data));

	//! Send out the
	if(this->write_bytes(&req) != VALID){
		this->_error = true;
	}
}

/**
 * This fills the ram based on CDN.
 *
 *   Variable updates:
 *		cdn, ydn, year, month, day
 *
 * @param _cdn							- cdn value
 */
void DS1307::fill_by_CDN(uint16_t _cdn){

	//! Variable Declaration
	uint16_t y, days_per_year;

	//! Fill
	this->_time.cdn = _cdn;
	y = 2000;
	for(;;){

		days_per_year = 365;
		days_per_year += is_leap_year(y);

		if ( _cdn >= days_per_year ){
			_cdn -= days_per_year;
			y++;
		}
		else
			break;
	}
	_cdn++;
	this->_time.year = y;
	this->_time.ydn = _cdn;
	calculate_dow();
	calculate_month_by_year_and_ydn();
	calculate_day_by_month_year_and_ydn();
	calculate_time2000();
}

/**
 * This fills the ram by time from 2000
 *
 *   Variable updates:
 *		time2000, cdn, ydn, year, month, day, hour, minute, second
 *
 * @param _time2000						- the time since 2000
 */
void DS1307::fill_by_time_2000(uint32_t _time2000){

	this->_time.time2000 = _time2000;
	this->_time.second = _time2000 % 60;
	_time2000 /= 60;
	this->_time.minute = _time2000 % 60;
	_time2000 /= 60;
	this->_time.hour = _time2000 % 24;
	_time2000 /= 24;
	fill_by_CDN(_time2000);
}

/**
 * This fills the ram based on HMS time.
 *
 * @param h								- hours
 * @param m								- minutes
 * @param s								- seconds
 */
void DS1307::fill_by_HMS(uint8_t h, uint8_t m, uint8_t s){

	//! Assign variables
	this->_time.hour = h;
	this->_time.minute = m;
	this->_time.second = s;

	//! Recalculate seconds since 2000-01-01
	calculate_time2000();
}

/**
 * This fills the rma based on the YMD time
 *
 * @param y								- year
 * @param m								- month
 * @param d								- day
 */
void DS1307::fill_by_YMD(uint16_t y, uint8_t m, uint8_t d){

	//! Assign variables
	this->_time.year = y;
	this->_time.month = m;
	this->_time.day = d;

	//! Recalculate depending values
	calculate_ydn();
	calculate_cdn();
	calculate_dow();
	calculate_time2000();
}

/**
 * This checks if it is day light savings or not
 *
 * @return bool							- yes or no
 */
uint8_t DS1307::is_MEZ_Summer_Time(void){

	uint32_t current_time, summer_start, winter_start;
	current_time = this->_time.time2000;
  
	// calculate start of summer time
	fill_by_YMD(this->_time.year, 3, 31);		// bugfix: Change from 30 to 31 because March has 31 days
	fill_by_HMS(2,0,0);
	fill_by_CDN(this->_time.cdn - this->_time.dow);	// sunday before
	summer_start = this->_time.time2000;

	// calculate start of winter
	fill_by_YMD(this->_time.year, 10, 31);
	fill_by_HMS(3,0,0);
	fill_by_CDN(this->_time.cdn - this->_time.dow);	// sunday before
	winter_start = this->_time.time2000;
  
	// restore time
	fill_by_time_2000(current_time);
  
	// return result
	if ( summer_start <= current_time && current_time < winter_start )
		return 1;
	return 0;
}

/**
 * This gets the internal time struct.
 *
 * @return time_t 						- the pointer to the internal time struct
 */
time_t* DS1307::return_time(){
	return &this->_time;
}

/**
 * This gets the private variable ctl.
 *
 * @return uint8_t						- the control flag
 */
uint8_t DS1307::return_ctl(){
	return this->_ctrl;
}

// Private Context

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
uint8_t DS1307::is_leap_year(uint16_t y){

   if ( 
          ((y % 4 == 0) && (y % 100 != 0)) || 
          (y % 400 == 0) 
      )
      return 1;
   return 0;
}

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
void DS1307::calculate_ydn(void){
	//! Variable Declaration
	uint8_t tmp1;
	uint16_t tmp2;

	//! Calculate
	tmp1 = 0;
	if (this->_time.month >= 3)
		tmp1++;
	tmp2 = this->_time.month;
	tmp2 +=2;
	tmp2 *=611;
	tmp2 /= 20;
	tmp2 += this->_time.day;
	tmp2 -= 91;
	tmp1 <<=1;
	tmp2 -= tmp1;
	if (tmp1 != 0)
		tmp2 += is_leap_year(this->_time.year);
	this->_time.ydn = tmp2;
}

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
void DS1307::calculate_cdn(void){

	//! Variable Decleration
	uint16_t y = this->_time.year;

	//! Calculate
	this->_time.cdn = this->_time.ydn;
	this->_time.cdn--;
	while(y > 2000){
		y--;
		this->_time.cdn += 365;
		this->_time.cdn += is_leap_year(y);
	}
}

/**
 * This calculates dow from ydn
 *
 *   calculate day of week (dow)
 * 	  0 = sunday .. 6 = saturday
 * 	  Arguments:
 *   	this->cdn	days since 2000-01-01 (2000-01-01 has the cdn 0 and is a saturday)
 */
void DS1307::calculate_dow(void){

	//! Variable Decleration
	uint16_t tmp;

	//! Calculate
	tmp = this->_time.cdn;
	tmp += 6;
	tmp %= 7;
	this->_time.dow = tmp;
}

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
void DS1307::calculate_time2000(void){

	//! Variable declaration
	uint32_t t;

	//! Calculate
	t = this->_time.cdn;
	t *= 24;
	t += this->_time.hour;
	t *= 60;
	t += this->_time.minute;
	t *= 60;
	t += this->_time.second;
	this->_time.time2000 = t;
}

/**
 * This calculates the corrected year by day number.
 *
 * @return corrected_year				- the year corrected
 */
uint16_t DS1307::_corrected_year_day_number(void){

	//! Variable Declaration
	uint8_t a;
	uint16_t corrected_ydn = this->_time.ydn;

	//! Calculate
	a = is_leap_year(this->_time.year);
	if (corrected_ydn > (uint8_t)(((uint8_t)59) + a)){

		corrected_ydn += 2;
		corrected_ydn -= a;
	}
	corrected_ydn += 91;
	return corrected_ydn;
}

/**
 * This calculates the month by year and ydn
 */
void DS1307::calculate_month_by_year_and_ydn(void){

	//! Delaration of variables
	uint8_t a;
	uint16_t c_ydn;

	//! Calculate
	c_ydn = _corrected_year_day_number();
	c_ydn *= 20;
	c_ydn /= 611;
	a = c_ydn;
 	a -= 2;
 	this->_time.month = a;
}

/**
 * This calculates the day by month year and ydn
 */
void DS1307::calculate_day_by_month_year_and_ydn(void){

	//! Declaration of variables
	uint8_t m;
	uint16_t tmp, c_ydn;

	//! Calculate
	m = this->_time.month;
	m += 2;
	c_ydn = _corrected_year_day_number();
	tmp = 611;
	tmp *= m;
	tmp /= 20;
	c_ydn -= tmp;
	this->_time.day = c_ydn;
}
