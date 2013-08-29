#include <stdlib.h>

#include <pentabug/app.h>
#include <pentabug/music.h>
#include <pentabug/hal.h>

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
			wait_ms(180);
		} else if(notes[i] == NOTE_SHORT) {
			wait_ms(30);
		} else {
			set_note(notes[i], 4);
			wait_ms(500);

			stop_note();
			wait_ms(10);
		}
	}
}

REG(run);
