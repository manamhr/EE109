/********************************************
 *
 *  Name: Mana Mehraein
 *  Section: Wed 2:00PM
 *  Assignment: Project
 *
 ********************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <avr/eeprom.h>

#include "lcd.h"
#include "serial.h"
#include "speedtrap.h"

void variable_delay_us(int);
void play_note(unsigned short);
void init_timer0 (unsigned short m);
volatile char flag; // count
volatile char flag2; // rotary counter
volatile char flag3; // speaker
volatile unsigned int count;
volatile unsigned int counter; //rotary
volatile unsigned char state;
volatile char new_state, old_state;
volatile char a, b;
volatile unsigned char pininput;
volatile unsigned int cm=0;
unsigned short frequency=262;
extern volatile char flag4; // @ is received
extern volatile char flag5; // $ is received
extern char sentence [5];
volatile char volt3[17];
volatile char volt2[5];
volatile char speaker;
volatile int spkCount;
volatile unsigned int volt5;


#define FOSC 16000000           // Clock frequency
#define BAUD 9600               // Baud rate used
#define MYUBRR FOSC/16/BAUD-1

void init_timer1(unsigned short m)
{
    //Set the mode for "clear Timer on compare" (CTC)
    //– WGM13 = 0, WGM12 = 1
    TCCR1B |=(1<<WGM12);
    
    //Enable Output compare A Match Interrupt
    TIMSK1 |=(1<<OCIE1A);
    
    // load the Max count
    //OCR1A=2000;
    OCR1A=m;
    // prescalar= 8 : 010
    TCCR1B |= (1<<CS11);
    
}
void init_timer0 (unsigned short n)
{
    //Set the mode for "clear Timer on compare" (CTC)
    //– WGM03 = 0, WGM02 = 1
    TCCR0B |=(1<<WGM02);
    //Enable Output compare A Match Interrupt
    TIMSK0 |=(1<< OCIE0A);
    
    // load the Max count
    //OCR1A=15625;
    OCR0A=n;
    // prescalar=  64 : 011
    //101
    //    TCCR0B |=((1<<CS01)|(1<<CS02));
    TCCR0B |=((1<<CS02));
}


int main(void)
{
    
    // Initialize the LCD
    lcd_init();
    DDRB |=  ((1<<PB3)|(1<<PB4)); // colored LED | Speaker
    PORTB &= ~(1<<PB3);
    DDRC &= ~((1<<PC3)|(1<<PC4)); // for photoreceptors
    PORTC &= ~((1<<PC3)|(1<<PC4));
    
    PORTC |=((1<<PC1)|(1<<PC5)); // set port c, 1 and 5 for rotary
    
    count=0;
    state=0;
    flag=0; // count
    flag2=0; // rotary
    flag3=0; // speaker
    flag4=0;
    
    //    counter=0; //change this after eeprom
    
    
    counter=eeprom_read_byte((void *) 100); //rotary
    
    if (counter <0 || counter >100)
    {
        counter=20;
    }
    
    
    
    //     init and start timer
    init_timer1(2000); //timer for count
    //    period = 2000000 / freq;
    init_timer0(250); //buzzer
    
    serial_init(MYUBRR);
    // setting up the interrupt for rotary
    PCICR |= (1<<PCIE1);
    PCMSK1 |=((1<<PCINT9)|(1<<PCINT13)|(1<<PCINT11)|(1<<PCINT12)); //A1, A5: rotary A3, A4: photoreceptors
    
    UCSR0B |= (1 << RXCIE0);    // Enable receiver interrupts
    
    // used to check inputs
    // and perform state updates &
    //  update display
    lcd_writecommand(1);
    
    pininput = PINC;
    a=pininput &(1<<PC1); //rotary
    b=pininput &(1<<PC5);
    if (!b && !a)
        old_state = 0;
    else if (!b && a)
        old_state = 1;
    else if (b && !a)
        old_state = 2;
    else
        old_state = 3;
    
    new_state = old_state;
    
    sei();
    
    while (1)
    {
        
        if ((pininput & (1<<PC4))==0)
        {
            //reset time to 0
            PORTB |=(1<<PB3); //turn on the LED
            
            //            test
            //                                    lcd_moveto(1,2);
            //                                    lcd_stringout("START");
            
        }
        else if ((pininput & (1<<PC3))==0)//
        {
            PORTB &=~(1<<PB3); //turn off the LED
            
            //            test
            //                                    lcd_moveto(1,2);
            //                                    lcd_stringout("STOP");
        }
        
        if (flag==1)
        {
            lcd_writecommand(1);
            cm= 30480/(count);
            lcd_moveto(0,0);
            snprintf(volt3, 17, "%3dms%3d.%d cm/s", count, cm/10, cm%10);
            lcd_stringout(volt3);
            snprintf(volt2, 7, " %4d ", cm);
            //            lcd_stringout(volt2);
            serial_stringout(volt2);
            //            lcd_moveto(1,0);
            //            lcd_stringout("                  ");
            
            char volt4[17];
            lcd_moveto(1,0);
            snprintf(volt4, 17, "MAX= %d", counter);
            lcd_stringout(volt4);
            
            //            char volt7[6];
            //            unsigned int volt5;
            //            sscanf(sentence, " %d", &volt5); // char
            //            lcd_moveto(1,9);
            //            snprintf(volt7, 17, "%3d.%d", volt5/10, volt5%10);
            //            lcd_stringout(volt7);
            
            if (cm >= counter)
            {
                //                play_note(frequency);
                speaker=1;
            }
            count=0;
            flag = 0;
        }
        
        if (flag2==1)
        {
            //            lcd_writecommand(1);
            flag2=0; //
            char volt4[17];
            lcd_moveto(1,0);
            snprintf(volt4, 17, "MAX= %d", counter);
            lcd_stringout(volt4);
        }
        
        if (flag4==1)
        {
            //            lcd_writecommand(1);
//            unsigned int volt5;
            flag4=0; // set flag back to zero
            
            lcd_moveto(1,10);
            sscanf(sentence, "%d", &volt5); // char
            
            char volt7[6];
            
            snprintf(volt7, 6, "%d.%d", volt5/10, volt5%10);
            lcd_stringout(volt7); // print the received message
        }
        
        
    }
    return 0;
}

ISR (TIMER1_COMPA_vect)
{
    // update time based on state
    // set flag to tell main to
    //  update display based on state
    
    if (state == 1)
    {
        
        count++;
    }
    
}


ISR(PCINT1_vect)
{
    pininput=PINC;
    a=pininput &(1<<PC1);
    b=pininput &(1<<PC5);
    
    if ((pininput & (1<<PC4))==0)
    {
        state=1;
    }
    else if ((pininput & (1<<PC3))==0)
    {
        state=0;
        flag=1;
    }
    if (old_state == 0) {
        
        // Handle A and B inputs for state 0
        if (a)
        {
            //CW
            new_state=1;
            counter++;
            if (counter==100)
            {
                counter=0;
            }
        }
        else if (b)
        {
            //CCW
            new_state=2;
            if (counter==0)
            {
                counter=99;
            }
            counter--;
        }
        
    }
    else if (old_state == 1) {
        
        // Handle A and B inputs for state 1
        if (b)
        {
            //CW
            new_state=3;
            counter++;
            if (counter==100)
            {
                counter=0;
            }
            
        }
        else if (a==0)
        {
            //CCW
            new_state=0;
            if (counter==0)
            {
                counter=99;
            }
            counter--;
        }
    }
    else if (old_state == 2) {
        
        // Handle A and B inputs for state 2
        if (b==0)
        {
            //CW
            new_state=0;
            counter++;
            if (counter==100)
            {
                counter=0;
            }
        }
        else if (a)
        {
            //CCW
            new_state=3;
            if (counter==0)
            {
                counter=99;
            }
            counter--;
        }
    }
    else {   // old_state = 3
        
        // Handle A and B inputs for state 3
        if (a==0)
        {
            //CW
            new_state=2;
            counter++;
            if (counter==100)
            {
                counter=0;
            }
        }
        else if (b==0)
        {
            //CCW
            new_state=1;
            if (counter==0)
            {
                counter=99;
            }
            counter--;
        }
    }
    
    
    if (new_state != old_state) { // Did state change?
        eeprom_update_byte((void*) 100, counter); //put where counter changes
        
        // Output count to LCD
        flag2=1;
        old_state = new_state;
    }
    
}


//void play_note(unsigned short freq)
//{
//    unsigned long period;
//
//    period = 1000000 / freq;      // Period of note in microseconds
//
//    while (freq--) {
//        // Make PB4 high
//        PORTB |=(1<<4);
//        // Use variable_delay_us to delay for half the period
//        variable_delay_us(period/2);
//        // Make PB4 low
//        PORTB &=~(1<<4);
//        // Delay for half the period again
//        variable_delay_us(period/2);
//
//    }
//}

ISR (TIMER0_COMPA_vect)
{
    if (speaker)
    {
        PORTB ^=(1<<PB4);
        spkCount++;
        if (spkCount > 500) //freq*2
        {
            speaker=0;
            spkCount=0;
        }
        
    }
}

/*
 variable_delay_us - Delay a variable number of microseconds
 */
//void variable_delay_us(int delay)
//{
//    int i = (delay + 5) / 10;
//
//    while (i--)
//        _delay_us(10);
//}
//



