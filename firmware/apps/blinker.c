#include <stdlib.h>

#include <avr/io.h>
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>

#include <pentabug/app.h>
#include <pentabug/lifecycle.h>

static void init(void) {
	_delay_ms(1000);
}

static void blinker(void) {
	PORTD |= 1 << 4;
	_delay_ms(800);
	test_stop_app();

	PORTD &= ~(1 << 4);
	_delay_ms(200);
	test_stop_app();
}

REGISTER(blinker, init, NULL);
