#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>
#include <stdlib.h>

#define ever		(;;)

#define follow(from_port, from_pin, to_port, to_pin)	{ if(from_port & (1 << from_pin)) to_port &= ~(1 << to_pin); else to_port |= 1 << to_pin; }
#define not_follow(from_port, from_pin, to_port, to_pin)	{ if(from_port & (1 << from_pin)) to_port |= 1 << to_pin; else to_port &= ~(1 << to_pin); }

ISR(TIMER0_COMPA_vect) {
	TCNT0 = 0;
	PORTD ^= 1 << 2;
}

int main(void) {
	uint8_t vib_delay = 0;

	// init and stuff

	PORTB |= (1 << 6) | (1 << 7);
	PORTC |= (1 << 0) | (1 << 2) | (1 << 3);

	DDRB |= (1 << 6) | (1 << 7);
	DDRC |= (1 << 0) | (1 << 2) | (1 << 3);
	DDRD |= (1 << 2) | (1 << 4);

	// pullups

	PORTB |= (1 << 0) | (1 << 1) | (1 << 2);
	PORTC |= (1 << 4) | (1 << 5);
	PORTD |= (1 << 5) | (1 << 6) | (1 << 7);

	/*PORTD |= 1 << 2;*/

	// we need to get real fast ...

	CLKPR = 0b10000000 ; 
	CLKPR = 0b00000000 ; //4MHz Clock 

	// initialize timer

	TIMSK0 |= (1 << OCIE0A);
	// this should be a 105 or something ... measure multiple bugs with a real measurement device
	OCR0A = 85;

	// no prescaler

	sei();

	// looping

	for ever {
		// LED by button

		not_follow(PIND, 5, PORTD, 4);

		// LED by IR

		not_follow(PIND, 3, PORTC, 2);

		// vibration

		follow(PINC, 4, PORTB, 6);

		// higher buzzer

		if(!(PINC & (1 << 5))) {
			PORTB ^= 1 << 7;
		} else {
			PORTB &= ~(1 << 7);
		}

		// lower buzzer

		vib_delay ^= 1;

		if(!(PIND & (1 << 7))) {
			if(vib_delay) {
				PORTC ^= 1 << 0;
			}
		} else {
			PORTC &= ~(1 << 0);
		}

		// send IR

		if(!(PIND & (1 << 6))) {
			TCCR0B = (1 << CS00);
		} else {
			TCCR0B = 0;
			PORTD &= ~(1 << 2);
		}

		// wait

		_delay_ms(1);
	}

	/* never  return 0; */
	return 0;
}
