#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>
#include <stdlib.h>

#define ever		(;;)

#define follow(active, to_port, to_pin)	{ if(active) to_port &= ~(1 << to_pin); else to_port |= 1 << to_pin; }
#define not_follow(active, to_port, to_pin)	{ if(active) to_port |= 1 << to_pin; else to_port &= ~(1 << to_pin); }

#define FLASH(port, pin)		{ port &= ~(1 << pin); _delay_ms(50); port |= 1 << pin;}

static volatile uint8_t ir_active = 0;

ISR(TIMER0_COMPA_vect) {
	if(ir_active) {
		PORTD ^= 1 << 2;
	} else {
		PORTD &= ~(1 << 2);
	}
}

static void reset_hw(void) {
	// 0: S1
	// 1: S2
	// 2: SHIELD
	// 7: BUZZR
	PORTB = (1 << 0) | (1 << 1) | (1 << 2) | (1 << 7);
	DDRB = (1 << 2) | (1 << 6) | (1 << 7);

	// 0: BUZGND
	// 2: LED2
	// 3: LED2 (+)
	// 4: SHIELD
	// 5: SHIELD
	PORTC = (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5);
	DDRC = (1 << 0) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5);

	// 2: IR
	// 4: LED
	// 5: SHIELD
	// 6: SHIELD
	// 7: SHIELD
	PORTD = (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7);
	DDRD = (1 << 2) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7);

}

int main(void) {
	uint16_t count = 0;

	// we need to get real fast (8MHz) to handle 38kHz IR frequency ...

	CLKPR = 0b10000000;
	CLKPR = 0b00000000;

	// initialize timer

	TIMSK0 |= (1 << OCIE0A);

	// calculated and works, but frequency is a little bit off?
	OCR0A = 105;

	TCCR0A = (1 << WGM01);
	TCCR0B = (1 << CS00);

	reset_hw();

	// no prescaler

	sei();

	// looping

	for ever {
		// blinking left led

		if(count % 0x4000 == 0) {
			PORTD ^= 1 << 4;
		}

		++count;

		// right led on ir

		not_follow(PIND & (1 << 3), PORTC, 2);

		// left button

		const uint8_t button_r = !(PINB & (1 << 0));
		const uint8_t button_l = !(PINB & (1 << 1));

		// ir

		ir_active = button_r;

		// motor

		not_follow(button_l, PORTB, 6);

		// sound stuff

		if(button_l) {
			PORTC ^= 1 << 0;
			_delay_ms(2);
		}

		if(button_r) {
			PORTC ^= 1 << 0;
			_delay_ms(4);
		}

		// cycle pins on extension board

		const uint8_t step = (count >> 13) & 7;

		follow(step == 0, PORTB, 2);
		follow(step == 1, PORTD, 5);
		follow(step == 2, PORTD, 6);
		follow(step == 3, PORTD, 7);
		follow(step == 4, PORTC, 5);
		follow(step == 5, PORTC, 4);
	}

	/* never  return 0; */
	return 0;
}
