#include <pentabug/app.h>
#include <pentabug/pentatonic.h>
#include <pentabug/hal.h>
#include <pentabug/music.h>

static void init(void) {
	pentatonic_direction(ALL_IN);
}

static void run(void) {
	pentatonic_direction(ALL_IN);

	uint8_t prev_buttons = 0;
	for(;;) {
		uint8_t buttons = pentatonic_buttons();
		if (prev_buttons != buttons) {
			if		(buttons & 16)	set_note(NOTE_Db, 4);
			else if	(buttons & 8)	set_note(NOTE_Eb, 4);
			else if	(buttons & 4)	set_note(NOTE_Gb, 4);
			else if	(buttons & 2)	set_note(NOTE_Ab, 4);
			else if	(buttons & 1)	set_note(NOTE_Bb, 4);
			else stop_note();
			prev_buttons = buttons;
		}
		wait_ms(1);
	}
}

REGISTER(run, init, NULL);
