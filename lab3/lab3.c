/********************************************
 *
 *  Name: Mana Mehraein
 *  Lab section: Wed 2PM
 *  Assignment: Lab 3
 *
 ********************************************/

#include <avr/io.h>
#include <util/delay.h>

#define DOT_LENGTH  250         /* Define the length of a "dot" time in msec */

void dot(void);
void dash(void);
void makeOutput(char);
char checkInput(char);

int main(void)
{
    
    // Initialize appropriate DDR registers
    DDRB |= 0x01;
    
    // Initialize the LED output to 0
    PORTD &= 0xfe;
    
    // Enable the pull-up resistors for the
    // 3 button inputs
    PORTD |= 0x1C;
    
    
    // Loop forever
    while (1) {
        
        // Use if statements and the checkInput()
        //  function to determine if a button
        //  is being pressed and then output
        //  the correct dot/dash sequence by
        //  calling the dot(), dash(), and
        //  using appropriate delay functions
        
        
        
        if ( checkInput(4) == 0)
        {
            //U
            dot();
            dot();
            dash();
        }
        
        else if (  checkInput(3) == 0)
        {
            
            
            //S
            dot();
            dot();
            dot();
        }
        else if (  checkInput(2) == 0)
        {
            //C
            dash();
            dot();
            dash();
            dot();
            
        }
        
        
        
    }
    
    return 0;   /* never reached */
}

/*
 dot() - Makes the output LED blink a "dot".
 Write code to generate a dot by turning the
 output ON and OFF with appropriate delays.
 Be sure you don't forget to also delay an
 appropriate time after you turn the output off
 */
void dot()
{
    // Use makeOutput
    
    // this is dot function therefore less delay
    makeOutput(1);
    _delay_ms(250);
    makeOutput(0);
    _delay_ms(250);
    
}

/*
 dash() - Makes the output LED blink a "dash".
 Write code to generate a dash by turning the
 output ON and OFF with appropriate delays.
 Be sure you don't forget to also delay
 appropriate time after you turn the output off
 */
void dash()
{
    
    // 3 times delay for dash and last delay would be same as dot since it is before dot
    makeOutput(1);
    _delay_ms(750);
    makeOutput(0);
    _delay_ms(250);
}

/*
 makeOutput() - Changes the output bit to either a zero
 or one, based on the input argument "value".
 
 If the argument is zero, turn the output OFF,
 otherwise turn the output ON.
 
 Do not use any delays here.  Just use bit-wise operations
 to make the appropriate PORT bit turn on or off.
 */
void makeOutput(char value)
{
    if ( value == 0)
    {
        PORTB &= 0xfe;
    }
    else if ( value == 1)
    {
        PORTB |= 0x01;
    }
    
}

/*
 checkInput(bit) - Checks the state of the input bit specified by the
 "bit" argument (0-7), and returns either 0 or 1 depending on its state.
 
 COMPLETE -- DO NOT CHANGE
 */
char checkInput(char bit)
{
    if ((PIND & (1 << bit)) != 0)
        return(1);
    else
        return(0);
}
