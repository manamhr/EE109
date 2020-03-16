/********************************************
 *
 *  Name: Mana Mehraein
 *  Section: Wed 2:00PM
 *  Assignment: Lab 8
 *
 ********************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include<avr/interrupt.h>

#include "lcd.h"
#include "adc.h"

volatile char flag=0;
volatile unsigned char count=0;
volatile unsigned char sec=0;
volatile unsigned char sec2=0;
volatile unsigned char state;


void init_timer1(unsigned short m)
{
    //Set the mode for "clear Timer on compare" (CTC)
    //– WGM13 = 0, WGM12 = 1
    TCCR1B |=(1<<WGM12);
    
    //Enable Output compare A Match Interrupt
    TIMSK1 |=(1<<OCIE1A);
    
    // load the Max count
    //OCR1A=6250;
    OCR1A=m;
    // prescalar= 256 : 100
    TCCR1B |= (1<<CS12);
    
}

int main(void)
{
    //    unsigned char adc_result;
    // Setup DDR and PORT bits for the 2 input buttons
    // as necessary
    DDRD |=((1<<4)||(1<<5)|(1<<6)|(1<<7));                   // Set the DDR register bits for ports B and D
    
    // Initialize the LCD
    lcd_init();
    
    //initialize the state
    state = 0;
    // state 0: stopped
    // state 1: started
    // state 2: lapped
    
    
    flag=0;
    // Initialize the ADC
    adc_init(0);
    // init and start timer
    init_timer1(6250);
    
    //enable interrupts
    sei();
    // used to check inputs
    // and perform state updates &
    //  update display
    
    // UP: start_stop
    //Down: Lap_Resert
    // None=255
    //Select=206
    //Left=161
    //UP=66
    //Down=107
    //Right=3
    
    lcd_writecommand(1);
    char volt3[10];
    lcd_moveto(0,2);
    snprintf(volt3, 10, " %d%d.%d ", sec2,sec,count);
    lcd_stringout(volt3);
    
    while (1)
    {
        //       char volt[20];
        //                lcd_moveto(1,2);
        //        adc_result = adc_sample();
        //
        //        snprintf(volt, 17, "%d ", adc_result);
        //        lcd_stringout(volt);
        //        _delay_ms(1000);
        unsigned char x= adc_sample();
        //        snprintf(volt, 17, "%d ", x);
        //        lcd_stringout(volt);
        if (state ==0)
        {
            if (x<132 && x>82)
            {
                //Down: Lap_Resert
                //reset time to 0
                count =0;
                sec=0;
                sec2=0;
                flag=1;
                
                while(x<132 && x>82) {x= adc_sample();}
                
            }
            else if (x<77 && x>27)
            {
                // UP: start_stop
                //state:Started
                state=1;
                flag=1;
                while(x<77 && x>27) {x= adc_sample();}
            }
        }
        else if (state == 1)
        {
            
            if (x<132 && x>82)
            {
                //Down: Lap_Resert
                //state:Lapped
                state=2;
                //freezes the displayed time at the instant the "lap" button is pressed while still keeping the internal watch time incrementing.
                flag=0;
                while(x<132 && x>82) {x= adc_sample();}
                
            }
            else if (x<77 && x>27)
            {
                // UP: start_stop
                state =0;
                //stop counter
                flag=0;
                while(x<77 && x>27) {x= adc_sample();}
                
            }
        }
        
        else if (state == 2)
        {
            if ((x<132 && x>82)||(x<77 && x>27))
            {
                //Down: Lap_Resert or UP: start_stop
                state=1;
                //show the continued count
                //When "lap" is pressed again (or "start" is pressed again) the internal watch time (which has been running) should be re-displayed and then continue as normal.
                flag=1;
                while ((x<132 && x>82)||(x<77 && x>27)) {x= adc_sample();}
                
            }
        }
        
        if (flag==1)
        {
            
            char volt3[10];
            lcd_moveto(0,2);
            snprintf(volt3, 10, " %d%d.%d ", sec2,sec,count);
            lcd_stringout(volt3);
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
        flag=1;
        count*0.1; //change the count as 10th of a sec
        count++;
        
        // change the first part of the sec
        if (count == 10)
        {
            sec++;
            count =0;
            // change the second part of the sec
            if (sec == 10)
            {
                sec2++;
                sec=0;
                if (sec2==6) // roll back time from 59.9 to zero
                {
                    sec2=0;
                    sec=0;
                }
            }
            
        }
    }
    
    else if (state == 2)
    {
        
        count*0.1;  //change the count as 10th of a sec
        count++;
        
        // change the first part of the sec
        if (count == 10)
        {
            sec++;
            count =0;
            // change the second part of the sec
            if (sec == 10)
            {
                sec2++;
                sec=0;
                if (sec2==6) // roll back time from 59.9 to zero
                {
                    sec2=0;
                    sec=0;
                }
            }
            
        }
    }
    
}




