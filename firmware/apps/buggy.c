#include <stdlib.h>

#include <pentabug/app.h>
#include <pentabug/hal.h>
#include <pentabug/photons.h>
#include <pentabug/pentatonic.h>

static void init(void) {
	pentatonic_direction(ALL_OUT);
	photons_init();

	led_on(RIGHT);
}

static void run(void) {
	uint8_t light = photons_measure();

	pentatonic_all_led_set(light >> 3);

	led_inv(LEFT);
	led_inv(RIGHT);

	wait_ms(100);
}

REGISTER(run, init, NULL);
