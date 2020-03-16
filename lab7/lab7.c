/********************************************
 *
 *  Name: Mana Mehraein
 *  Section: Wed 2PM
 *  Assignment: Lab 7 - Rotary Encoder
 *
 ********************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "lcd.h"

void variable_delay_us(int);
void play_note(unsigned short);
volatile int count;
volatile char new_state, old_state;
volatile char a, b;
volatile int flag;
volatile unsigned char var;


// Frequencies for natural notes from middle C (C4)
// up one octave to C5.
unsigned short frequency[8] =
{ 262, 294, 330, 349, 392, 440, 494, 523 };

int main(void) {
    //    int count = 0;        // Count to display
    count =0;
    // Initialize DDR and PORT registers and LCD
    DDRB |=((1<<1)|(1<<0)|(1<<4));
    DDRD |=((1<<4)|(1<<5)|(1<<6)|(1<<7));
    // setting up the interrupt
    PCICR |= (1<<PCIE1);
    PCMSK1 |=((1<<PCINT9)|(1<<PCINT13));
    sei();
    
    
    // Write a spash screen to the LCD
    lcd_init();
    PORTC |=((1<<PC1)|(1<<PC5)); // set port c, 1 and 5
    lcd_writecommand(1); // clear out the screen
    /* Call lcd_stringout to print out your name */
    lcd_moveto(0,2);
    lcd_stringout("Mana Mehraein");
    
    /* Delay 1 seconds */
    _delay_ms(1000);
    //    lcd_writecommand(1); // clear out the screen
    
    // Read the A and B inputs to determine the intial state
    // Warning: Do NOT read A and B separately.  You should read BOTH inputs
    // at the same time, then determine the A and B values from that value.
    
    var=PINC;
    a=var &(1<<PC1);
    b=var &(1<<PC5);
    
    if (!b && !a)
        old_state = 0;
    else if (!b && a)
        old_state = 1;
    else if (b && !a)
        old_state = 2;
    else
        old_state = 3;
    
    new_state = old_state;
    char cnt [5];
    lcd_moveto(1,1); //move to start
    snprintf(cnt, 17, "%d", count);
    lcd_stringout(cnt); //print out the 0
    flag=0;
    while (1) {                 // Loop forever
        // Read the input bits and determine A and B
        //        lcd_writecommand(1);
        if (flag==1)
        {
            flag=0;
            lcd_moveto(1,1); //move to start
            snprintf(cnt, 17, "%d", count);
            lcd_stringout(cnt); // print the updated count
            
            //play note
            if ((count % 8) == 0) {
                // Determine which note (0-7) to play
                char note= (abs(count) % 64)/ 8;
                
                // Find the frequency of the note
                
                // Call play_note and pass it the frequency
                play_note(frequency[note]);
            }
        }
        
    }
}

ISR(PCINT1_vect)
{
    var=PINC;
    a=var &(1<<PC1);
    b=var &(1<<PC5);
    
    
    if (old_state == 0) {
        
        // Handle A and B inputs for state 0
        if (a)
        {
            //CW
            new_state=1;
            count++;
            
        }
        else if (b)
        {
            //CCW
            new_state=2;
            count--;
            
        }
        
    }
    else if (old_state == 1) {
        
        // Handle A and B inputs for state 1
        if (b)
        {
            //CW
            new_state=3;
            count++;
        }
        else if (a==0)
        {
            //CCW
            new_state=0;
            count--;
        }
    }
    else if (old_state == 2) {
        
        // Handle A and B inputs for state 2
        if (b==0)
        {
            //CW
            new_state=0;
            count++;
        }
        else if (a)
        {
            //CCW
            new_state=3;
            count--;
        }
    }
    else {   // old_state = 3
        
        // Handle A and B inputs for state 3
        if (a==0)
        {
            //CW
            new_state=2;
            count++;
        }
        else if (b==0)
        {
            //CCW
            new_state=1;
            count--;
        }
    }
    
    
    if (new_state != old_state) { // Did state change?
        
        // Output count to LCD
        flag=1;
        old_state = new_state;
    }
    
}

/*
 Play a tone at the frequency specified for one second
 */
void play_note(unsigned short freq)
{
    unsigned long period;
    
    period = 1000000 / freq;      // Period of note in microseconds
    
    while (freq--) {
        // Make PB4 high
        PORTB |=(1<<4);
        // Use variable_delay_us to delay for half the period
        variable_delay_us(period/2);
        // Make PB4 low
        PORTB &=~(1<<4);
        // Delay for half the period again
        variable_delay_us(period/2);
    }
}

/*
 variable_delay_us - Delay a variable number of microseconds
 */
void variable_delay_us(int delay)
{
    int i = (delay + 5) / 10;
    
    while (i--)
        _delay_us(10);
}

