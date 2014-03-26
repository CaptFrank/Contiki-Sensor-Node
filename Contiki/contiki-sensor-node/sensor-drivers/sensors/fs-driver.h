/*
 * fs-driver.h
 *
 *  Created on: 2014-03-26
 *      Author: francispapineau
 */

#ifndef FS_DRIVER_H_
#define FS_DRIVER_H_

/**
 * The file system integrity.
 */
typedef enum fs_intergity_t {
	READ_WRITE,
	READ_ONLY,
	CORRUPT,
	FULL,
	EMPTY
};

class fs_driver : public EEPROM {

private :
    /**
     * This method checks if the file system was in fact intact,
     * or if it falls under one of these circumstances:
     * 	- READ_WRITE 	-> integrity and space to write
     * 	- READ_ONLY		-> this file system is read only and must not be reformatted
     * 	- CORRUPT		-> file system is corrupt and needs to be reformatted
     * 	- FULL			-> file system is in fact full and needs to over write data
     * 	- EMPTY			-> this is a file system that is empty and needs to init
     *
     * @return fs_intergity_t				- the file system state
     */
    fs_intergity_t check_fs();

    void reformat_fs();

    void restart_fs_index();


};



#endif /* FS_DRIVER_H_ */
