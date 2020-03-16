/********************************************
 *
 *  Name: Mana Mehraein
 *  Section: Wed 2:00PM
 *  Assignment: Project
 *
 ********************************************/


#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "speedtrap.h"
#include "serial.h"

// Serial communications functions and variables
void serial_init(unsigned short);
void serial_stringout(char *);
void serial_txchar(char);
volatile char flag4; // @ is received
volatile char flag5; // $ is received
char sentence [5];
volatile int startReceived = 0;
volatile int i=0;
volatile unsigned int icount=0;

  // Value for UBRR0 register


void serial_init(unsigned short baud)
{
    
    // Set up USART0 registers
    UBRR0 = baud;             // Set baud rate
    UCSR0C = (3 << UCSZ00);               // Async., no parity,
    // 1 stop bit, 8 data bits
    //    UCSR0B |= (1 << RXCIE0); // set RXCIE0 bit to one
    UCSR0B |= (1 << TXEN0 | 1 << RXEN0);  // Enable RX and TX
    
    // Enable tri-state
    DDRD |=(1<<PD3);
    PORTD &= ~(1<<PD3);
}



void serial_txchar(char next_character)
{
    while ((UCSR0A & (1<<UDRE0)) == 0) { }
    UDR0 = next_character;
}

void serial_stringout(char *s)
{
    int j=0;
    // Call serial_txchar in loop to send a string
    serial_txchar('@');
    for (j=0; j<5; j++)
    {
        if ((s[j] -'0') >=0 && (s[j]-'0') < 10)
        {
        serial_txchar(s[j]);
        }
    }
    serial_txchar('$');
}


ISR(USART_RX_vect)
{
    // Handle recevied character
    char next_character;
    
    next_character = UDR0;                  // Get the received charater
    
    
    if (next_character=='@')
    {
        startReceived = 1;
    }
    
    if ((  (next_character - '0') >=0 && (next_character - '0') < 10) && startReceived == 1 )
    {
        
        sentence[icount] =next_character;
        icount++;
        
    }
    //check if you received $, then you want to reset icount to 0 and startReceived to 0 and set flag4 to 1
    if (next_character=='$')
    {
        sentence[icount]="/0";
        icount=0;
        startReceived=0;
        flag4=1;
    }
    
}

