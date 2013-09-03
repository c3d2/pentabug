#include <stdlib.h>

#include <pentabug/app.h>
#include <pentabug/hal.h>
#include <pentabug/photons.h>
#include <pentabug/pentatonic.h>

inline uint16_t biased_random(uint8_t value) {
	return value / 4 * (rand() & 7);
}

static void init(void) {
	pentatonic_direction(ALL_OUT);
	photons_init();

	led_on(RIGHT);
}

static void run(void) {
	uint16_t light = photons_measure();

	pentatonic_all_led_set(light >> 3);

	motor_set(biased_random(light) > 0x50 ? ON : OFF);

	led_set(RIGHT, biased_random(light) > 0x20);
	led_set(LEFT, biased_random(light) > 0x20);

	if(biased_random(light) > 0x70) {
		uint16_t tone = (biased_random(light) * 2) + 500;
		set_note(tone, 0);
	} else {
		stop_note();
	}

	wait_ms(200);
}

REGISTER(run, init, NULL);
