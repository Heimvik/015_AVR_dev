#pragma once
#include <avr/io.h>
#include <util/delay.h>
#include "defines.h"


void initUart(uint8_t ie);
void putCharUart(unsigned char character);
char getCharUart();