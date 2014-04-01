/*
* Pin Control Library
*
* This library has been modified for 8-bit architectures.
* The difference between headers is only in port type
*/

#ifndef PIN_CONTROL_LIB_H
#define PIN_CONTROL_LIB_H

#include <stdint.h>
#include <stdbool.h>

#define HIGH 1
#define LOW 0

typedef enum pin_types
{
	inputMode = 0,
	inputModeWithPullup,
	outputMode,
	disabledMode
} pin_enum_t;

void pinModeSet(volatile void* port, uint8_t pin, pin_enum_t pin_mode);
void pinSet(volatile void* port, uint8_t pin, bool value);
void pinMaskSet(volatile void* port, uint16_t mask, bool value);
uint16_t pinMaskRead(volatile void* port, uint16_t mask);
bool pinRead(volatile void *port, uint8_t pin);

#endif
