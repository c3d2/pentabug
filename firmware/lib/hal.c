#include <pentabug/hal.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>

#include <pentabug/lifecycle.h>

#include <pentabug/timer.h>

static uint8_t ir_active = 1;
static int int_skip = 0;

static int button_count[2];

// major interrupt for button handling, every 5ms
inline static void major_interrupt(void) {
	uint8_t i = 0;

	for(i = 0; i < 2; ++i) {
		if(PINB & (1 << i)) {
			button_count[i] = 0;
		} else {
			++button_count[i];
		}

		if(button_count[i] == 400) {
			next_app(i ? 1 : -1);
			PORTC ^= 1 << 2;
		}
	}
}

ISR(TIMER0_COMPA_vect) {
	if(ir_active) {
		PORTD ^= 1 << 2;
	}

	++int_skip;

	if(int_skip >= 38 * 5) {
		int_skip = 0;
		major_interrupt();
	}
}

void init_hw(void) {
	// we need to get real fast (8MHz) to handle 38kHz IR frequency ...

	CLKPR = 0b10000000;
	CLKPR = 0b00000000;

	// initialize timer

	TIMSK0 |= (1 << OCIE0A);

	// calculated and works, but frequency is a little bit off?
	OCR0A = 105;

	TCCR0A = (1 << WGM01);
	TCCR0B = (1 << CS00);

	sei();
}

void reset_hw(void) {
	stop_timer();

	// 0: S1
	// 1: S2
	// 7: BUZZR
	PORTB = (1 << 0) | (1 << 1) | (1 << 7);
	DDRB = (1 << 7);

	// 0: BUZGND
	// 2: LED2
	// 3: LED2 (+)
	PORTC = (1 << 2) | (1 << 3);
	DDRC = (1 << 0) | (1 << 2) | (1 << 3);

	// 2: IR
	// 4: LED
	PORTD = (1 << 4);
	DDRD = (1 << 2) | (1 << 4);
}

