#include "tc.h"

//TODO:
/*
- Set up interrupts after x mills
- Set up PWM
*/

uint8_t pickPrescalerBits(uint16_t prescaler) {
    if (prescaler == 1) {
        return 0b001;  // clkT2S / 1 (No prescaling)
    } else if (prescaler == 8) {
        return 0b010;  // clkT2S / 8
    } else if (prescaler == 64) {
        return 0b011;  // clkT2S / 32
    } else if (prescaler == 256) {
        return 0b100;  // clkT2S / 64
    } else if (prescaler == 1024) {
        return 0b101;  // clkT2S / 128
    } else {
        return 0b001;  // The fastest one
    }
}

/*
Initialises either Timer 0 or Timer 2 for PWM generation.
Frequency can be found by F_PWM = F_CLK / (PRESCALER * 256).
*/
void initPWM2(uint16_t prescaler){
    //Set OC2 (PB2) pin as output
    DDRB |= (pickPrescalerBits(prescaler)<<PORTB1);

    //Selects F_CLK/1024 as base clock
    TCCR2 |= (3<<CS20);
    //Set fast PWM mode
    TCCR2 |= (3<<WGM20);
    //Set the counter to clear on match
    TCCR2 |= (2<<COM20);
    //Start at 50% dc
    OCR2 = 0x0F;
}
void initPWM(PWM_COUNTER pwmCounter, uint8_t dc_percent, uint16_t prescaler) {
    // Set OC0 (PB3) or OC2 (PB1) pin as output depending on the timer
    if (pwmCounter == 0) {
        DDRB |= (pickPrescalerBits(prescaler) << PORTB0);  // Set PB3 (OC0) as output for Timer 0
        // Select F_CLK/1024 as base clock for Timer 0
        TCCR0 |= (3<<CS00);
        // Set fast PWM mode for Timer 0
        TCCR0 |= (3<<WGM00);
        // Set the counter to clear on match (non-inverting mode)
        TCCR0 |= (2<<COM00);
        // Set initial duty cycle
    }
    else if (pwmCounter == 2) {
        DDRB |= (pickPrescalerBits(prescaler) << PORTB1);  // Set PB1 (OC2) as output for Timer 2
        // Select F_CLK/1024 as base clock for Timer 2
        TCCR2 |= (3<<CS20);
        // Set fast PWM mode for Timer 2
        TCCR2 |= (3<<WGM20);
        // Set the counter to clear on match (non-inverting mode)
        TCCR2 |= (2<<COM20);
        // Set initial duty cycle
    }
    setPWMDutyCycle(pwmCounter,dc_percent);
}

void setPWMDutyCycle(PWM_COUNTER pwmCounter,uint8_t dc_percent){
    if(pwmCounter == 0){
        OCR0 = (uint8_t)((dc_percent*0xFF)/100);
    }
    else if(pwmCounter == 2){
        OCR2 = (uint8_t)((dc_percent*0xFF)/100);
    }
}

/*
Initializes either TC1 or TC3 for interrupt generation.
NB! Watch for overflow in OCR, period_ms is only valid when prescaler = 1
Becomes approximately one second if prescaler = 1024
*/
void initTC1Interrupt(TC_CHANNEL channel, uint16_t period_ms,uint16_t prescaler){
    //Disable OCnA output
    TCCR1A &= ~(0b11<<COM1A0);
    //Enable CTC mode
    TCCR1A &= ~(0b11<<WGM10);
    TCCR1B |= (1<<WGM12);
    //Select clock source
    TCCR1B |= (pickPrescalerBits(prescaler)<<CS10);
    uint16_t compareValue = (uint16_t)(TC1_COUNTER_VALUE)*period_ms;
    cli();
    //Set the compare value, disable interrupts for atomic access to 16-bit registers
    if(channel == CHANNEL_A){
        OCR1AH =(uint8_t)((compareValue)>>8);
        OCR1AL =(uint8_t)(compareValue);
        //Enable TC1A interrupt
        TIMSK |= (1<<OCIE1A);
    } else if (channel = CHANNEL_B){
        OCR1BH =(uint8_t)(compareValue>>8);
        OCR1BL =(uint8_t)compareValue;
        //Enable TC1A interrupt
        TIMSK |= (1<<OCIE1B);
    }
    sei();
}