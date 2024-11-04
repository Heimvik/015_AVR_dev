#pragma once
#include <avr/io.h>

#define F_CLK 16000000
#define F_IO 13333
#define TC1_COUNTER_VALUE 0x200
#define PWM0_PRESCALER 1024
#define PWM2_PRESCALER 1024
#define BAUDRATE_USART0 9600

typedef enum PWM_COUNTER{
    PWM_0,
    PWM_2
} PWM_COUNTER;

typedef enum TC_CHANNEL{
    CHANNEL_A,
    CHANNEL_B
} TC_CHANNEL;
