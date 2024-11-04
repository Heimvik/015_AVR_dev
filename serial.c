#include "serial.h"


void initUart(uint8_t ie){
    //Set normal speed mode (UCSRA: U2X = 0)
    UCSR0A &= ~(1<<U2X0);
    
    //Enable both receiver and transmitter (UCSRB:RXEN, UCSRB:TXEN = 1)
    UCSR0B |= (1<<RXEN0)|(1<<TXEN0);
    //Select asynchronus mode of operation (UCSRC:UMSEL = 0)
    UCSR0C &= ~(1<<UMSEL0);
    //8N1 mode: 8 data bits, no parity, 1 stop bit
    //8 data bits
    UCSR0C |= (1<<URSEL0)|(1<<UCSZ01)|(1<<UCSZ00);
    UCSR0B &= ~(1<<UCSZ02);
    //No parity
    UCSR0C &= ~(0b11<<UPM00);
    //1 stop bit
    UCSR0C &= ~(1<<USBS0);

    //Baudrate to 9600 bps (UBRR = 0d31)
    uint32_t ubrr_value = F_CLK/16/BAUDRATE_USART0-1;
    UBRR0H = (uint8_t)(ubrr_value >> 8);
    UBRR0L = (uint8_t)ubrr_value;

    //Set interrupt driven receive
    if(ie){
        UCSR0B |= (1<<RXCIE0);
    }else{
        UCSR0B &= ~(1<<RXCIE0);
    }
}

void putCharUart(unsigned char character){
    //Read/write to UDR only if the register is empty
    while(!(UCSR0A & (1<<UDRE0))){}
    UDR0 = character;
}

//Called upon getting an interrupt in main loop
char getCharUart(){
    return 0;
}