#include <stdlib.h>

#include <pentabug/app.h>
#include <pentabug/hal.h>
#include <pentabug/photons.h>
#include <pentabug/pentatonic.h>

static void init(void) {
	pentatonic_direction(ALL_OUT);
	photons_init();
}

static void run(void) {
	uint16_t light = photons_measure();

	pentatonic_all_led_set(light >> 7);

	wait_ms(500);
}

REGISTER(run, init, NULL);
