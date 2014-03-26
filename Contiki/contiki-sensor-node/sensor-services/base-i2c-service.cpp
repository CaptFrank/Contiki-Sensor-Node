/*
 * basei2cservice.cpp
 *
 *  Created on: 2014-03-24
 *      Author: francispapineau
 */

#include "base-i2c-service.h"

/**
 * This is the default constructor for the class.
 * We initialize the list in this method.
 *
 * @param max_queue_size				- the number of services that can be registered.
 */
base_i2c_service::base_i2c_service(uint8_t max_queue_size = MAX_SERVICE_QUEUE){

	//! Init pointer
	this->_service_pointer = 0;

	//! This is creating an empty list
	this->_service_list = create_list();

	//! This adds a header to the list
	list_add(this->_service_list, (void*)(_service_pointer = FIRST), 1);

	//! This adds a tail to the list
	list_add(this->_service_list, (void*)(_service_pointer = LAST), 1);
}

/**
 * This method registers a service to the dynamic service list
 *
 * @param service_ptr					- the service pointer structure
 */
void base_i2c_service::register_service(service_ptr_t* service_ptr){

	//! Wraps the private method
	this->add_service(service_ptr);
}

/**
 * This method removes a registered service from the dynamic service list
 *
 * @param service_ptr					- the service pointer structure
 */
void base_i2c_service::unregister_service(){

	//! Wraps the private access method
	this->remove_service();
}

/**
 * This method registers a service to the dynamic service list
 *
 * @param service_ptr					- the service pointer structure
 */
void base_i2c_service::add_service(service_ptr_t* service_ptr){

	//! We remove the last pointer - from the back
	list_remove(this->_service_list, BACK);

	//! We then add a service to the list
	list_add(this->_service_list, (void*) service_ptr, sizeof(service_ptr->size));

	//! We then add the tail flag
	list_add(this->_service_list, (void*)(_service_pointer = LAST), 1);

}

/**
 * This method removes a registered service from the dynamic service list
 *
 * @param service_ptr					- the service pointer structure
 */
void base_i2c_service::remove_service(){

	//! We remove the last pointer - from the back
	list_remove(this->_service_list, BACK);

	//! We remove the last node
	list_remove(this->_service_list, BACK);
}

/**
 * Get the service list
 *
 * @return list							- the list of services
 */
list_p base_i2c_service::get_service_list(){
	return this->_service_list;
}
