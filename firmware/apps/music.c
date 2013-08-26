#include <stdlib.h>

#include <avr/io.h>
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>

#include <pentabug/app.h>
#include <pentabug/lifecycle.h>
#include <pentabug/music.h>

#define NOTE_BREAK		0xfffe
#define NOTE_SHORT		0xfffd

static uint16_t notes[] = {
	NOTE_C, NOTE_D, NOTE_E, NOTE_F, NOTE_SHORT, NOTE_G, NOTE_G, NOTE_BREAK,
	NOTE_A, NOTE_A, NOTE_A, NOTE_A, NOTE_SHORT, NOTE_G, NOTE_BREAK,
	NOTE_A, NOTE_A, NOTE_A, NOTE_A, NOTE_SHORT, NOTE_G, NOTE_BREAK,
	NOTE_F, NOTE_F, NOTE_F, NOTE_F, NOTE_SHORT, NOTE_E, NOTE_E, NOTE_BREAK,
	NOTE_D, NOTE_D, NOTE_D, NOTE_D, NOTE_SHORT, NOTE_C, NOTE_BREAK,
	NOTE_PAUSE, NOTE_PAUSE, NOTE_PAUSE,
};

static void run(void) {
	size_t i;
	for(i = 0; i < sizeof(notes) / sizeof(*notes); ++i) {
		if(notes[i] == NOTE_BREAK) {
			_delay_ms(120);
		} else if(notes[i] == NOTE_SHORT) {
			_delay_ms(20);
		} else {
			set_note(notes[i], 4);
			_delay_ms(180);
			test_stop_app();

			stop_note();
			_delay_ms(10);
			test_stop_app();
		}
	}
}

REG(run);
