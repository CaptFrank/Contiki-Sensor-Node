/*
 * i2cdaemon.cpp
 *
 *  Created on: 2014-03-24
 *      Author: francispapineau
 */

#include <avr/delay.h>
#include "i2c-daemon.h"

/**
 * This sets the registered service list to the daemon.
 *
 * @param service_list							- the list of services.
 */
i2c_daemon::i2c_daemon(list_p service_list){

	//! Init the service list pointer
	this->_service_list = service_list;

	//! Create a list iterator
	this->_iterator = list_iterator(service_list, FRONT);
}

/**
 * This runs the daemon
 */
void i2c_daemon::run_daemon(){

	uint8_t length = this->_service_list->length;
	//! Get the service node
	while(length --){

		//! Run the service
		(*((service_ptr_t*)(list_next(this->_iterator)))->service_ptr)();

		//! Wait for 100ms
		_delay_ms(100);
	}
}
