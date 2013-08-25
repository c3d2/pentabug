#include <stdlib.h>

#include <avr/io.h>
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>

#include <pentabug/app.h>
#include <pentabug/lifecycle.h>

static void init(void) {
	PIND |= 1 << 4;
	DDRD |= 1 << 4;

	_delay_ms(2000);
}

static void blinker(void) {
	size_t i;

	for(i = 0; i < 5; ++i) {
		PORTD |= 1 << 4;
		_delay_ms(800);

		PORTD &= ~(1 << 4);
		_delay_ms(200);
	}

	stop_app();
}

REGISTER(blinker, init, NULL);
