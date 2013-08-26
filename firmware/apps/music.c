#include <stdlib.h>

#include <avr/io.h>
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>

#include <pentabug/app.h>
#include <pentabug/lifecycle.h>
#include <pentabug/music.h>

#define NOTE_SHORT		0xfffe

static uint16_t notes[] = {
	NOTE_C, NOTE_D, NOTE_E, NOTE_F, NOTE_G, NOTE_G, NOTE_SHORT,
	NOTE_A, NOTE_A, NOTE_A, NOTE_A, NOTE_G, NOTE_SHORT,
	NOTE_A, NOTE_A, NOTE_A, NOTE_A, NOTE_G, NOTE_SHORT,
	NOTE_F, NOTE_F, NOTE_F, NOTE_F, NOTE_E, NOTE_E, NOTE_SHORT,
	NOTE_D, NOTE_D, NOTE_D, NOTE_D, NOTE_C, NOTE_SHORT,
	NOTE_PAUSE, NOTE_PAUSE, NOTE_PAUSE,
};

static void init(void) {
	PORTC ^= 1 << 2;
}

static void run(void) {
	size_t i;
	for(i = 0; i < sizeof(notes) / sizeof(*notes); ++i) {
		if(notes[i] != NOTE_SHORT) {
			set_note(notes[i], 4);
			_delay_ms(200);
			test_stop_app();

			stop_note();
			_delay_ms(10);
			test_stop_app();
		} else {
			_delay_ms(100);
		}

		PORTD ^= 1 << 4;
		PORTC ^= 1 << 2;
	}
}

REGISTER(run, init, NULL);
