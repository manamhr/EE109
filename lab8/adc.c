/********************************************
 *
 *  Name: Mana Mehraein
 *  Section: Wed 2:00PM
 *  Assignment: Lab 6
 *
 ********************************************/

#include <avr/io.h>
#include "adc.h"

void adc_init(unsigned char channel)
{
    // Initialize the ADC
    
    //    Set/clear the MUX[3:0] bits in ADMUX to select the input channel you are using
    
    ADMUX |= channel;
    
    
    //    Set/clear the REFS[1:0] bits in ADMUX to select the high voltage reference. Using the AVCC reference is appropriate for this lab.
    
    ADMUX &= ~(1<<REFS1);
    ADMUX |=(1<<REFS0);
    
    
    //    Set or clear the ADLAR bit in ADMUX such that we will use 8-bit conversion results (not 10-bit).
    ADMUX |=(1<<ADLAR);
    
    //    Set/clear the ADPS[2:0] bits in ADCSRA to select an appropriate prescalar value.
    
    ADCSRA |= ((1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2));
    
    //    Set the ADEN bit in ADCSRA to enable the ADC module.
    
    ADCSRA|=(1<<ADEN);
    //    The other bits should be left as zeros. The ADSC bit (shown below in red) will be set to a one later in your code, but in the initialization of the registers it should be a zero.
    
    ADCSRA &=~((1<<ADSC)|(1<<ADATE)|(1<<ADIF) |(1<<ADIE));
    ADMUX &=~(1<<4);
}

unsigned char adc_sample()
{
    // Convert an analog input and return the 8-bit result
    //    Set the ADSC bit in the ADCSRA register to a 1. This starts the conversion process.
    ADCSRA |=(1<<ADSC);
    
    //    Enter a loop that test the ADSC bit each time through the loop and exits the loop when ADSC is 0. This indicates that the conversion is now complete.
    while((ADCSRA & (1<<ADSC)))
    {}
    
    //    Copy the 8-bit conversion result from the ADCH register and returns it to the calling program.
    unsigned char result = ADCH;
    return result;
}
