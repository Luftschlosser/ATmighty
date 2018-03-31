/*
 * test.cpp
 *
 *  Created on: 31.03.2018
 *      Author: valentin
 */

#include <avr/io.h>
#include "test.h"

uint8_t test (uint8_t x){
	UDR0 = '.'; //send char to Usart0
	return ++x;
}

