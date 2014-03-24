/*
 * basei2cservice.h
 *
 *  Created on: 2014-03-24
 *      Author: francispapineau
 */

#ifndef BASEI2CSERVICE_H_
#define BASEI2CSERVICE_H_

#include <list.h>

#define MAX_SERVICE_QUEUE		10

/**
 * This is the pointer indicator for the first and
 * last service in the service tree.
 * 	- FIRST
 * 	- LAST
 */
typedef enum service_pointers_t {
	FIRST,
	LAST
};

/**
 * This is the service pointer structure containing the
 * main information pieces.
 */
typedef struct service_ptr_t {

	void* service_ptr;
	uint8_t process_type;
	uint8_t process_timeout;
	uint8_t size;
};

/**
 * This class serves as a template for an i2c service.
 * We use this in the scheduler, as we set pointers for
 * the services needed to act in the list above.
 */
class base_i2c_service {

	// Public context
	public:

		/**
		 * This is the default constructor for the class.
		 * We initialize the list in this method.
		 *
		 * @param max_queue_size				- the number of services that can be registered.
		 */
		base_i2c_service(uint8_t max_queue_size = MAX_SERVICE_QUEUE);

		/**
		 * This method registers a service to the dynamic service list
		 *
		 * @param service_ptr					- the service pointer structure
		 */
		void register_service(service_ptr_t* service_ptr);

		/**
		 * This method removes a registered service from the dynamic service list
		 */
		void unregister_service();

		/**
		 * Get the service list
		 *
		 * @return list							- the list of services
		 */
		list_p get_service_list();

		/**
		 * This is the virtual default constructor
		 */
		virtual ~base_i2c_service();

		/**
		 * This is the run service method
		 */
		virtual void run_service();

	// Private context
	private:

		/**
		 * This is the service pointer table.
		 */
		service_pointers_t _service_pointer;

		/**
		 * This is the list that holds the services
		 */
		list_p _service_list;

		/**
		 * This method registers a service to the dynamic service list
		 *
		 * @param service_ptr					- the service pointer structure
		 */
		void add_service(service_ptr_t* service_ptr);

		/**
		 * This method removes a registered service from the dynamic service list
		 *
		 * @param service_ptr					- the service pointer structure
		 */
		void remove_service();

};

#endif /* BASEI2CSERVICE_H_ */
