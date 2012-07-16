#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

#include "main.h"

int main (void)
{

	//enable LED channels as output
	DDRC |= (1<<PORTC0)|(1<<PORTC2)|(1<<PORTC3)|(1<<PORTC1);
	PORTC = 1; //one led is on...

	//BUZZER IS OUT:
	DDRD |= (1<<PORTD6);

	//vibration motor 
	DDRB |= (1<<PORTB1);

	//reduce power
	PRR |= (1<<PRTWI)|(1<<PRTIM2)|(1<<PRSPI)|(1<<PRUSART0);

	PORTB |= (1 << PORTB1);
	PORTD = 0;


	while(1)
	{
		_delay_ms(500);
		PORTC ^= (1<<PORTC0)|(1<<PORTC2);
		PORTB ^= (1<<PORTB1);
		PORTD ^= (1<< PORTD6);

	}
}

