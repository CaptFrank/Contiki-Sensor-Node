/*
 * i2cdaemon.h
 *
 *  Created on: 2014-03-24
 *      Author: francispapineau
 */

#ifndef I2CDAEMON_H_
#define I2CDAEMON_H_

#include <list.h>

#include "base-i2c-service.h"

/**
 * This is the final packet.
 */
struct net_packet_t{

	uint8_t buffer[255];
	uint8_t valid;
	uint8_t checksum;
};

/**
 * This is the daemon which runs the data acquisition daemon.
 * It gets the data and concatenates the data into a packet
 * that is then sent to the user.
 */
class i2c_daemon {

	//! Public context
	public:

		/**
		 * This sets the registered service list to the daemon.
		 *
		 * @param service_list							- the list of services.
		 */
		i2c_daemon(list_p service_list);

		/**
		 *	The default class deconstructor
		 */
		virtual ~i2c_daemon();

	//! Private context
	private:

		/**
		 * This the internal reference to the service list
		 */
		list_p _service_list;

		/**
		 * The iterator list
		 */
		list_iter_p _iterator;

		/**
		 * This runs the daemon
		 */
		void run_daemon();
};

#endif /* I2CDAEMON_H_ */
