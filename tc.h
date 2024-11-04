#pragma once
#include <avr/io.h>
#include "defines.h"
#include <avr/interrupt.h>

void initPWM(PWM_COUNTER pwmCounter, uint8_t dc_percent, uint16_t prescaler);
void setPWMDutyCycle(PWM_COUNTER pwmCounter,uint8_t dc_percent);

void initTC1Interrupt(TC_CHANNEL channel, uint16_t period_ms, uint16_t prescaler);