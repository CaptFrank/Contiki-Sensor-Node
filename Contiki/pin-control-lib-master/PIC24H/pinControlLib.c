/*
* Pin Control Library
*/

#include "pinControlLib.h"

//Board Library
#include "p24HJ64GP502.h"

/*
* Lookup Tables
*/
static const int8_t PORTA_CN_PIN_LOOKUP_TABLE[] = {2, 3, 30, 29, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
static const int8_t PORTB_CN_PIN_LOOKUP_TABLE[] = {4, 5, 6, 7, 1, 27, 24, 23, 22, 21, 16, 15, 14, 13, 12, 11};
/*
* Function Definitions *
*/

int8_t getCNPinMapping(volatile uint16_t *port, uint8_t pin);

void pinModeSet(volatile void* port, uint8_t pin, pin_enum_t pin_mode)
{
	if(((uint16_t *)port) == &PORTB)
	{
		switch(pin_mode)
		{
			case outputMode:
				TRISB &= (0xFFFF ^ (1 << pin)); //Will turn the bit to 0, and then the and will work as desired.
				break;
			case inputMode:
				TRISB |= 1 << pin; //Enable the pin as an input
				break;
			case inputModeWithPullup:
				TRISB |= 1 << pin;  //Enable the pin as an input
				
				//Enable the pullup on the pin
				int8_t cn_pin = getCNPinMapping(port, pin);
				if(cn_pin < 0)
					return; //Invalid port/pin for what we're trying to do!
				if(cn_pin < 16)
				{
					//use CNPU1
					CNPU1 |= 1 << cn_pin;
				}
				else if(cn_pin < 32)
				{
					//Use CNPU2
					CNPU2 |= 1 << (cn_pin - 16);
				}
				break;
			default:
				//for disabled, just make an input
				TRISB |= 1 << pin;
		}
	}
	else if(((uint16_t *)port) == &PORTA)
	{
		switch(pin_mode)
		{
			case outputMode:
				TRISA &= (0xFFFF ^ (1 << pin)); //Will turn the bit to 0, and then the and will work as desired.
				break;
			case inputMode:
				TRISA |= 1 << pin;
				break;
			default:
				//for disabled, just make an input
				TRISA |= 1 << pin;
		}
	}
}

void pinSet(volatile void* port, uint8_t pin, bool value)
{
	if(value == HIGH)
	{
		*((uint16_t*) port) |= 1 << pin;
	}
	else
	{
		*((uint16_t*) port) &= (0xFFFF ^ (1 << pin)); //will turn the bit to 0, and then the AND will work as intendend
	}
}

uint16_t pinMaskRead(volatile void* port, uint16_t mask)
{
	//Allows you to read multiple bits at once.
	return (*((uint16_t*) port)) & mask;
}

bool pinRead(volatile void *port, uint8_t pin)
{
	uint16_t masked_port = (*((uint16_t*) port)) & (1 << pin);
	return (masked_port >> pin);
}

void pinMaskSet(volatile void* port, uint16_t mask, bool value)
{
	if(value == HIGH)
	{
		*((uint16_t*) port) |= mask;
	}
	else
	{
		*((uint16_t*) port) &= (0xFFFF ^ mask);
	}
}

int8_t getCNPinMapping(volatile uint16_t *port, uint8_t pin)
{
		if(port == &PORTB)
		{
			return	PORTB_CN_PIN_LOOKUP_TABLE[pin];
		}
		else if(port == &PORTA)
		{
			return PORTA_CN_PIN_LOOKUP_TABLE[pin];
		}
	return -1; //If we haven't returned yet, we had invalid input	
}
