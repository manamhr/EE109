/********************************************
 *
 *  Name: Mana Mehraein
 *  Section: Wed 2:00PM
 *  Assignment: Lab 4 - Up/Down counter on LCD display
 *
 ********************************************/

#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"

int main(void) {
    
    // Setup DDR and PORT bits for the 2 input buttons
    // as necessary
    DDRD |=0xf0;                   // Set the DDR register bits for ports B and D
    DDRB |=0x03;                   // Take care not to affect any unnecessary bits
    
    
    // Initialize the LCD
    
    lcd_init();
    PORTC |=0x14;  //activate pull-up resistors
    
    // Use a state machine approach to organize your code
    //   - Declare and initialize a variable to
    //     track what state you are in
    unsigned char cnt=0;
    unsigned char state=1; //up=1 down=2
    unsigned char x=0; //new counter to check bottons
    
    while (1) {               // Loop forever
        
        
        lcd_writecommand(1);
        _delay_ms(100);
        // Use an outer if statement to select what state you are in
        // Then inside each 'if' or 'else' block, perform the desired
        //  output operations and then sample the inputs to update
        //  the state for the next iteration
        
        
        
        if((PINC & (1<<PC2))==0) //up
        {
            state=1;
        }
        
        
        else if ((PINC & (1<<PC4))==0) //down
        {
            state=2;
        }
        
        x++; // check bottons 5 times and then go do the increments
        if (x == 5)
        {
            x=0; // reset the botton counter
            
            if (state==1)
            {
                
                lcd_writedata(cnt + 0x30);
                cnt++; // count up
                _delay_ms(100);
                
                //            _delay_ms(500);
                lcd_writecommand(1); // Clear screen and move to row 0, column 0
                if (cnt==10) //go back to 0 from 9
                {
                    cnt=0;
                }
            }
            
            
            else if (state==2)
            {
                lcd_writedata(cnt + 0x30); // hex
                if (cnt==0) // go back to 9 from 0
                {
                    cnt=10;
                }
                cnt--; // count down
                _delay_ms(100);
                //            _delay_ms(500);
                lcd_writecommand(1); // Clear screen and move to row 0, column 0
                
            }
        }
    }
    
    return 0;   /* never reached */
}

