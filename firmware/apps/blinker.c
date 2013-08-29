#include <stdlib.h>

#include <pentabug/app.h>
#include <pentabug/hal.h>

static void init(void) {
	led_on(RIGHT);
}

static void blinker(void) {
	led_inv(RIGHT);
	led_inv(LEFT);
	wait_ms(500);
}

REGISTER(blinker, init, NULL);
