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
    PORTC |=(1<<3);  //activate pull-up resistors
    
    lcd_writecommand(1); // clear out the screen
    
    // Initialize the ADC
    adc_init(3);
    
    
    // Use the ADC to find a seed for rand();
    adc_result = adc_sample();
    srand(adc_result << 8 | adc_result);  // Make a 16-bit number for srand()
    
    // Write splash screen
    
    lcd_writecommand(1); // clear out the screen
    /* Call lcd_stringout to print out your name */
    lcd_moveto(0,2);
    lcd_stringout("Mana Mehraein");
    
    /* Use snprintf to create a string with your birthdate */
    char date[17];
    unsigned char month, day, year;
    month = 12;
    day = 24;
    year = 80;
    
    snprintf(date, 17, "Bday %d/%d/%d ", month, day, year);
    
    /* Use lcd_moveto to start at an appropriate column
     in the bottom row to appear centered */
    //                  lcd_moveto(unsigned char row, unsigned char col)
    lcd_moveto(2,2);
    /* Use lcd_stringout to print the birthdate string */
    lcd_stringout(date);
    
    /* Delay 1 seconds */
    _delay_ms(1000);
    lcd_writecommand(1); // clear out the screen
    
    // Find a random initial position of the 'X'
    unsigned char X;
    X = rand() % 16;
    lcd_moveto(0,X);
    // Display the 'X' on the screen
    lcd_stringout("X");
    
    unsigned int time = 0;
    while (1) {                 // Loop forever
        
        unsigned char car;
        unsigned char prev=0;
        
        // Do a conversion
        //        Convert the 8-bit value into a column number between 0 and 15.
        
        car = (adc_sample() / 16);
        // Move '^' to new position
        //        If the column number for the '^' has changed, erase the '^' at the old position and draw a new one at the new position.
       
        if (prev!=car)
        {
            lcd_moveto(1,car);
            lcd_writecommand(1);
            lcd_moveto(0,X);
            lcd_stringout("X");
            lcd_moveto(1,prev);
            prev=car; //making new car pos equal to prev
            lcd_stringout("^");
        }
        
        // Check if '^' is aligned with 'X'
        if (car == X)
        {
            
            //        If so, increment a count of how many 10ms intervals it has been there. If the count indicates that it has been there for 2 seconds, print a message that the player has won the game. The program can then enter an infinite loop to stop the game play.
            if (time == 200)
            {
                lcd_writecommand(1); // clear out the screen
                lcd_stringout("You Won!");
                
                while (1)
                {}
                
            }
            //            If not, reset the count to zero. This handles the case where the '^' was under the 'X' briefly but then moved on to another column.
            _delay_ms(10);
            time ++;
        }
        else
        {
            time=0;
        }
        
    }
    
    return 0;   /* never reached */
}

