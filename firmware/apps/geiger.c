#include <pentabug/app.h>
#include <pentabug/music.h>
#include <pentabug/photons.h>
#include <pentabug/hal.h>

inline uint16_t biased_random(uint8_t value) {
	return value / 4 * (rand() & 7);
}

enum modes {
	GEIGER,
	TWITCH,
	MAX_MODE,
};

static void init(void) {
	photons_init();
}

static enum modes mode = 0;

static void run(void) {
	uint8_t light = photons_measure();

	if(button_clicked(RIGHT)) {
		++mode;

		if(mode == MAX_MODE) {
			mode = 0;
		}
	}

	if(button_clicked(LEFT)) {
		if(mode == 0) {
			mode = MAX_MODE;
		}

		--mode;
	}

	switch(mode) {
		case GEIGER:
			if(light / 16 * (rand() & 15) > 0x40) {
				set_note(NOTE_C, 3);
			} else {
				stop_note();
			}

			wait_ms(20);
			break;
		case TWITCH:
			set_note(500 + light, 0);
			wait_ms(10);
			break;
		case MAX_MODE:
			break;
	}
}

REGISTER(run, init, NULL);
