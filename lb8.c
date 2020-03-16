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
    PORTB |=(1<<0);  //activate pull-up resistors
    
    lcd_writecommand(1); // clear out the screen
    
    // Initialize the ADC
    adc_init(0);
    
    
    
    // Write splash screen
    
    lcd_writecommand(1); // clear out the screen
    /* Call lcd_stringout to print out your name */
    lcd_moveto(0,2);
    lcd_stringout("Mana Mehraein");
    
    /* Use snprintf to create a string with your birthdate */
    char date[17];
    unsigned char volt;
    
    snprintf(date, 17, "%d ", volt);
    
    /* Use lcd_moveto to start at an appropriate column
     in the bottom row to appear centered */
    //                  lcd_moveto(unsigned char row, unsigned char col)
    lcd_moveto(2,2);
    /* Use lcd_stringout to print the birthdate string */
    lcd_stringout(date);
  
    
    return 0;   /* never reached */
}


