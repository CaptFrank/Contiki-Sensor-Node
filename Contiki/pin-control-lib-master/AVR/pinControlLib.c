/*
* Pin Control Library
*/

#include "pinControlLib.h"

//Board Library
#include "avr/io.h"

/*
Note from Datasheet:
When switching between tri-state ({DDxn, PORTxn} = 0b00) and output high ({DDxn, PORTxn}
= 0b11), an intermediate state with either pull-up enabled {DDxn, PORTxn} = 0b01) or output
low ({DDxn, PORTxn} = 0b10) must occur. Normally, the pull-up enabled state is fully acceptable,
as a high-impedance environment will not notice the difference between a strong high
driver and a pull-up. If this is not the case, the PUD bit in the MCUCR Register can be set to disable
all pull-ups in all ports.

Switching between input with pull-up and output low generates the same problem. The user
must use either the tri-state ({DDxn, PORTxn} = 0b00) or the output high state ({DDxn, PORTxn}
= 0b11) as an intermediate step.
*/

void pinModeSet(volatile void* port, uint8_t pin, pin_enum_t pin_mode)
{
	//Implement Safety checking for above.  Always return to Tri-State
	*((uint8_t *) port) &= (0xFFFF ^ (1 << pin));   //Will turn the PORTn value to 0...
	if(((uint8_t *)port) == &PORTD)
	{
		switch(pin_mode)
		{
			DDRD &= (0xFFFF ^ (1 << pin)); //And then return the pin to Tri-state to fix the above

			case outputMode:
				DDRD |= 1 << pin;
				break;
			case inputMode:
				DDRD &= (0xFFFF ^ (1 << pin)); //Will turn the bit to 0, and then the and will work as desired.
				PORTD &= (0xFFFF ^ (1 << pin)); //Set pin low just to make sure pull-up is disabled
				break;
			case inputModeWithPullup:
				DDRD &= (0xFFFF ^ (1 << pin)); //Set as input
				PORTD |= 1 << pin; //Setting high enables pullup resistor
				break;
			default:
				//for disabled, just make an input
				DDRD &= (0xFFFF ^ (1 << pin));
				PORTD &= (0xFFFF ^ (1 << pin)); //Set pin low just to make sure pull-up is disabled
		}
	}
	else if(((uint8_t *)port) == &PORTB)
	{
		switch(pin_mode)
		{
			DDRB &= (0xFFFF ^ (1 << pin)); //And then return the pin to Tri-state to fix the above
			
			case outputMode:
				DDRB |= 1 << pin;		
				break;
			case inputMode:
				DDRB &= (0xFFFF ^ (1 << pin)); //Will turn the bit to 0, and then the and will work as desired.
				PORTB &= (0xFFFF ^ (1 << pin)); //Set pin low just to make sure pull-up is disabled
				break;
			case inputModeWithPullup:
				DDRB &= (0xFFFF ^ (1 << pin)); //Set as input
				PORTB |= 1 << pin; //Setting high enables pullup resistor
				break;
			default:
				//for disabled, just make an input
				DDRB &= (0xFFFF ^ (1 << pin));
				PORTB &= (0xFFFF ^ (1 << pin)); //Set pin low just to make sure pull-up is disabled
		}
	}
}

void pinSet(volatile void* port, uint8_t pin, bool value)
{
	if(value == HIGH)
	{
		*((uint8_t*) port) |= 1 << pin;
	}
	else
	{
		*((uint8_t*) port) &= (0xFFFF ^ (1 << pin)); //will turn the bit to 0, and then the AND will work as intendend
	}
}

void pinMaskSet(volatile void* port, uint16_t mask, bool value)
{
	if(value == HIGH)
	{
		*((uint8_t*) port) |= mask;
	}
	else
	{
		*((uint8_t*) port) &= (0xFFFF ^ mask);
	}
}

uint16_t pinMaskRead(volatile void* port, uint16_t mask)
{
	uint8_t masked_data;

	if(((uint8_t*) port) == &PORTD)
	{
		masked_data = PIND & mask;
	}
	else if(((uint8_t*) port) == &PORTB)
	{
		masked_data = PINB & mask;
	}
	else
	{
		masked_data = 0;
	}
	
	return masked_data;
}

bool pinRead(volatile void* port, uint8_t pin)
{
	bool pin_val;

	if(((uint8_t *)port) == &PORTD)
	{
		pin_val = (PIND >> pin) & 1;
	}
	else if(((uint8_t *)port) == &PORTB)
	{
		pin_val = (PINB >> pin) & 1;
	}
	else
	{
		pin_val = 0;
	}
	
	return pin_val;	
}
