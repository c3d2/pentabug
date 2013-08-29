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

static volatile uint32_t button_state = 0;
static volatile uint8_t next_mode = 0;
static volatile uint8_t ir_active = 0;

enum test_modes {
	PHOTONS,
	MOTOR,
	AUDIO,
	SHIELD,
	// the end ... wrap
	MODE_MAX,
};

ISR(TIMER0_COMPA_vect) {
	if(ir_active) {
		PORTD ^= 1 << 2;
	} else {
		PORTD &= ~(1 << 2);
	}

	if(PINB & (1 << 1)) {
		button_state = 0;
	} else {
		++button_state;
	}

	if(button_state == (38l * 1000 / 2)) {
		next_mode = 1;
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
	uint8_t vib_delay = 0;
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

	enum test_modes mode = PHOTONS;

	for ever {
		// next mode?

		if(next_mode) {
			// cleanup
			switch(mode) {
				case PHOTONS:
					ir_active = 0;
					break;
				case MOTOR:
				case AUDIO:
				case SHIELD:
					break;
				case MODE_MAX: break;
			}

			reset_hw();

			++mode;

			if(mode == MODE_MAX) {
				mode = 0;
			}

			uint32_t i;
			for(i = 0; i < 50; ++i) {
				PORTB ^= 1 << 7;
				_delay_ms(1);
			}

			next_mode = 0;
		}

		uint8_t button = !(PINB & (1 << 0));

		not_follow(PIND & (1 << 3), PORTC, 2);

		switch(mode) {
			case PHOTONS:
				{
					ir_active = button;
					if(count % 0x2000 == 0) {
						PORTD ^= 1 << 4;
					}
					++count;
					break;
				}
			case MOTOR:
				not_follow(button, PORTB, 6);
				break;
			case AUDIO:
				{
					if(button) {
						PORTC ^= 1 << 0;
						_delay_ms(2);
					}

					break;
				}
			case SHIELD:
				if(button) {
					FLASH(PORTB, 2);
					FLASH(PORTD, 5);
					FLASH(PORTD, 6);
					FLASH(PORTD, 7);
					FLASH(PORTC, 5);
					FLASH(PORTC, 4);
				}
				break;
			case MODE_MAX: break;
		}
	}

	/* never  return 0; */
	return 0;
}
