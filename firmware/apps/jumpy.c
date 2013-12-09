#include <pentabug/app.h>
#include <pentabug/hal.h>
#include <pentabug/pentatonic.h>
#include <pentabug/listen.h>
#include <pentabug/helper.h>
#include <pentabug/music.h>

#define SENSITIVITY_FACT		4

static uint16_t max_sound;
static uint8_t first;

static void init(void) {
	pentatonic_direction(ALL_OUT);
	listen_init();

	max_sound = 5;
	first = 0;
}

static void run(void) {
	uint16_t sound = listen_measure();

	pentatonic_all_led_set(max_sound >> 5);

	if(!first && sound > max_sound / SENSITIVITY_FACT) {
		int8_t intensity = SENSITIVITY_FACT + 1 - max_sound / sound;

		intensity = MIN(intensity, SENSITIVITY_FACT + 1);

		motor_on();

		set_note(NOTE_C, (rand() & 3) + 3);

		for(size_t i = 0; i < intensity; ++i) {
			led_set(RIGHT, rand() & 1);
			led_set(LEFT, rand() & 1);

			wait_ms(100);
		}

		// turn everything off

		stop_note();

		motor_off();

		led_off(RIGHT);
		led_off(LEFT);

		wait_ms(100);
	} else {
		first = 0;
	}

	if(sound > max_sound) {
		max_sound = sound;
	}
}

REGISTER(run, init, NULL);
