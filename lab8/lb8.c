/********************************************
 *
 *  Name: Mana Mehraein
 *  Section: Wed 2:00PM
 *  Assignment: Lab 6
 *
 ********************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>

#include "lcd.h"
#include "adc.h"



int main(void)
{
    unsigned char adc_result;
    // Setup DDR and PORT bits for the 2 input buttons
    // as necessary
    DDRD |=((1<<4)||(1<<5)|(1<<6)|(1<<7));                   // Set the DDR register bits for ports B and D
    DDRB |=((1<<0)|(1<<1));
    
    // Initialize the LCD
    lcd_init();
    PORTC |=(1<<0);  //activate pull-up resistors
    
    lcd_writecommand(1); // clear out the screen
    
    // Initialize the ADC
    adc_init(0);
    while (1)
    {
        char volt[20];
        lcd_writecommand(1);
        lcd_moveto(0,2);
        adc_result = adc_sample();
        
        snprintf(volt, 17, "%d ", adc_result);
        lcd_stringout(volt);
        _delay_ms(1000);
    }
    return 0;
}


