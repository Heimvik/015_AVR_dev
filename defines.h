#pragma once
#include <avr/io.h>

#define F_CLK 4915200
#define F_IO 13333
#define TC1_COUNTER_VALUE 0x200
#define PWM0_PRESCALER 1024
#define PWM2_PRESCALER 1024
#define BAUDRATE_USART0 9600

#define CONTROLLER_PERIOD_MS 50
#define K_P (double)0.2
#define K_I (double)0.2
#define K_D (double)0.2
typedef enum PWM_COUNTER{
    PWM_0,
    PWM_2
} PWM_COUNTER;

typedef enum TC_CHANNEL{
    CHANNEL_A,
    CHANNEL_B
} TC_CHANNEL;
