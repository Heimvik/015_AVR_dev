#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "serial.h"
#include "tc.h"
#include "defines.h"


//Next:
//-Use 8 bit TC to generate interrupt every 10 seconds
//-Set the led on
//-Use 8 bit TC to generate a new interrupt after 20 ms
//-Turn the led off


void initHeartBeat(){
    DDRB |= (1<<PORTB2);
}

ISR(TIMER1_COMPA_vect){
    PORTB ^= (1<<PORTB2);
}


int main(){
    initHeartBeat();
    initUart(0);
    //initPWM(PWM_0,10,PWM0_PRESCALER);
    initTC1Interrupt(CHANNEL_A,1,1024);
    //sei();

    
    while(1){

        putCharUart('X');
        _delay_ms(1000);
        putCharUart('Y');
        _delay_ms(1000);
    }
    return 0;
}