#include <stdlib.h>

#include <pentabug/app.h>
#include <pentabug/hal.h>
#include <pentabug/photons.h>
#include <pentabug/pentatonic.h>
#include <pentabug/music.h>
#include <pentabug/helper.h>

inline uint16_t biased_random(uint8_t value) {
	return value / 2 * (rand() & 15);
}

static void init(void) {
	pentatonic_direction(ALL_OUT);
	photons_init();
}

static void run(void) {
	if(ir_recv()) {
		// receive a ir signal, react

		motor_on();

		led_on(RIGHT);

		set_note(NOTE_B, 5);
		wait_ms(200);
		set_note(NOTE_F, 5);
		wait_ms(100);

		led_off(RIGHT);
		led_on(LEFT);

		set_note(NOTE_G, 5);
		wait_ms(100);
		set_note(NOTE_Ab, 5);
		wait_ms(100);
		set_note(NOTE_A, 5);
		wait_ms(100);

		led_off(LEFT);

		motor_off();
	} else if(button_clicked(RIGHT)) {
		// button clicked, send ir signal and do some stuff

		led_on(RIGHT);

		set_note(NOTE_A, 5);
		wait_ms(100);
		set_note(NOTE_Ab, 5);
		wait_ms(100);
		set_note(NOTE_G, 5);
		wait_ms(100);

		led_off(RIGHT);
		led_on(LEFT);

		set_note(NOTE_F, 5);
		wait_ms(100);
		set_note(NOTE_B, 5);
		wait_ms(200);
		stop_note();

		led_off(LEFT);

		ir_on();

		wait_ms(400);

		ir_off();

		wait_ms(10);
	} else {
		// regular bug behaviour

		uint8_t light = photons_measure();

		pentatonic_all_led_set(light >> 3);

		motor_set(biased_random(light) > 0x70);

		led_set(RIGHT, biased_random(light) > 0x30);
		led_set(LEFT, biased_random(light) > 0x30);

		if(biased_random(light) > 0x60) {
			uint16_t tone = (biased_random(light) * 2) + 500;
			set_note(tone, 0);
		} else {
			stop_note();
		}

		wait_ms(200);
	}
}

REGISTER(run, init, NULL);
