/********************************************
 *
 *  Name: Mana Mehraein
 *  Section: Wed 2:00PM
 *  Assignment: Lab 9 - Serial Communications
 *
 ********************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "lcd.h"
#include "adc.h"

// Serial communications functions and variables
void serial_init(unsigned short);
void serial_stringout(char *);
void serial_txchar(char);
volatile char flag;
char sentence [17];
volatile int i=0;

#define FOSC 16000000           // Clock frequency
#define BAUD 9600               // Baud rate used
#define MYUBRR FOSC/16/BAUD-1   // Value for UBRR0 register

// ADC functions and variables
void adc_init(unsigned char);

char *messages[] = {
    "Hello           ",
    "How are you?    ",
    "Have u done lab?",
    "Can I go home?:D",
    "Okay bye! :D  :D",
    "It's       cold!",
    "When lab's due? ",
    "Thank you!!     ",
    "I am hungry!    ",
    "Leaviiiinnggg..."
    // Add more messgages
};



int main(void) {
    
    // Initialize the modules and LCD
    lcd_init();
    adc_init(0);
    serial_init(MYUBRR);
    


    unsigned char pointer = 0;
    flag=0;
    
    // Enable interrupts
    UCSR0B |= (1 << RXCIE0);    // Enable receiver interrupts
    sei();                      // Enable interrupts
    
    // Show the splash screen
    lcd_writecommand(1); // clear out the screen
    /* Call lcd_stringout to print out your name */
    lcd_moveto(0,0);
    lcd_stringout("Choose a Message");
    /* Delay 1 seconds */
    _delay_ms(1000);
//    lcd_writecommand(1); // clear out the screen

    
    while (1) {                 // Loop forever
        
        // Get an ADC sample
       unsigned char x=adc_sample();
        
        // Up button pressed?
        if (x<77 && x>27)
        {
            _delay_ms(500);
            //            go Up
            pointer++;
            if (pointer==10) // adjust the pointer to go back to zero again
            {
                pointer=0;
            }
            //            print
//                lcd_writecommand(1); // clear out the screen

            lcd_moveto(0,0);
            lcd_stringout(messages[pointer]);
//            while(x<77 && x>27) {x= adc_sample();}

        }
        // Down button pressed?
        else if (x<132 && x>82)
        {
            _delay_ms(500);
            //            go down
            if (pointer ==0) // adjust the pointer
            {
                pointer =10;
            }
            pointer--;
            if (pointer ==10) // adjust the pointer
            {
                pointer =0;
            }
//            lcd_writecommand(1); // clear out the screen

            //            print
            lcd_moveto(0,0);
            lcd_stringout(messages[pointer]);
//            while(x<132 && x>82) {x= adc_sample();}
            

        }
        // Select button pressed?
//        else if (x<20 && x>0)
            else if (x<231 && x>188) // if select is pushed
        {
            // checking what value is x
//            char volt[20];
//            lcd_writecommand(1);
//            lcd_moveto(1,2);
//            snprintf(volt, 17, "%d ", x);
//            lcd_stringout(volt);
//            _delay_ms(1000);
//            lcd_writecommand(1); // clear out the screen

            serial_stringout(messages[pointer]); // print
            _delay_ms(500);
        }
        // Message received from remote device?
        if (flag==1)
        {
            flag=0; // set flag back to zero
            lcd_moveto(1,0);
            lcd_stringout(sentence); // print the received message
        }
        
    }
}

/* ----------------------------------------------------------------------- */

void serial_init(unsigned short baud)
{
    
    // Set up USART0 registers
    UBRR0 = MYUBRR;             // Set baud rate
    UCSR0C = (3 << UCSZ00);               // Async., no parity,
    // 1 stop bit, 8 data bits
    UCSR0B |= (1 << TXEN0 | 1 << RXEN0);  // Enable RX and TX
    
    // Enable tri-state
    DDRD |=(1<<PD3);
    PORTD &= ~(1<<PD3);
}



void serial_txchar(char ch)
{
    while ((UCSR0A & (1<<UDRE0)) == 0);
    UDR0 = ch;
}

void serial_stringout(char *s)
{
    int j=0;
    // Call serial_txchar in loop to send a string
    for (j=0; j<16; j++)
    {
        serial_txchar(s[j]);
    }
}


ISR(USART_RX_vect)
{
    
    // Handle recevied character
    char ch;
    
    ch = UDR0;                  // Get the received charater
    
    // Store in buffer
    sentence[i]=ch;
    i++;
    
    // If message complete, set flag
    // When the last character (the 16th) has been received
    if (i==16)
    {
        sentence[16]="/0"; // set the last character to null
        flag=1;
        i=0; // reset i to be able to receive new messages.
        
    }
}
