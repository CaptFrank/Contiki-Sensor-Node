/*
 * Utility.h
 *
 *  Created on: 2014-03-27
 *      Author: francispapineau
 */

#ifndef UTILITY_H_
#define UTILITY_H_

    // Utility methods

    /**
     * This converts decimal numbers to binary coded decimal numbers
     *
     * @param num							- the number to convert
     * @return number						- the number converted
     */
    inline uint8_t dec2bcd(uint8_t num){
    	return ((num/10 * 16) + (num % 10));
    }

    /**
     * This converts binary coded decimal numbers to decimal numbers
     *
     * @param num							- the number to convert
     * @return number						- the number converted
     */
    inline uint8_t bcd2dec(uint8_t num){
    	return ((num/16 * 10) + (num % 16));
    }


#endif /* UTILITY_H_ */
