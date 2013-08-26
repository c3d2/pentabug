#include <stdlib.h>

#include <avr/io.h>
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>

#include <pentabug/app.h>
#include <pentabug/lifecycle.h>
#include <pentabug/hal.h>

static void init(void) {
	led_on(RIGHT);
}

static void blinker(void) {
	led_inv(RIGHT);
	led_inv(LEFT);
	_delay_ms(500);
}

REGISTER(blinker, init, NULL);
