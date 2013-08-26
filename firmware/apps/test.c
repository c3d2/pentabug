#include <stdlib.h>

#include <avr/io.h>
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>

#include <pentabug/app.h>
#include <pentabug/lifecycle.h>

static void tester(void) {
	PORTD ^= 1 << 4;
	_delay_ms(100);
}

REG(tester);
