#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

#include "main.h"


void init_pwm(void){
	
	// TCCR1A xx01xx10
	TCCR1A &= ~0b00100001;
	TCCR1A |=  0b00000010;

	// TCCR1B xxx11001
	TCCR1B &= ~0b00000110;
	TCCR1B |=  0b00011001;
	
	//PB1 set to output:
	DDRB |= 0b10;


}


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

	/*PORTB |= (1 << PORTB1);*/
	PORTD = 0;

	init_pwm();


	/*while(1)*/
	/*{*/
		/*_delay_ms(500);*/
		/*PORTC ^= (1<<PORTC0)|(1<<PORTC2);*/
		/*PORTB ^= (1<<PORTB1);*/
		/*PORTD ^= (1<< PORTD6);*/

	/*}*/

	for(;;){
		OCR1BH = 0;
		OCR1BL = 0;
		_delay_ms(1);
		OCR1BH = 0xff;
		OCR1BL = 0xff;
		_delay_ms(1);
	}

}

