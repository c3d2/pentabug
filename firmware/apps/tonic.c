#include <pentabug/app.h>
#include <pentabug/pentatonic.h>
#include <pentabug/hal.h>
#include <pentabug/music.h>
#include <pentabug/helper.h>

static void run(void) {
	uint8_t prev_buttons = 0;
	static uint8_t octave = 4;

	pentatonic_direction(ALL_IN);

	for ever {
		uint8_t buttons = pentatonic_buttons();

		if(button_clicked(RIGHT) && octave < 8) {
			++octave;
			prev_buttons = 0;
		}

		if(button_clicked(LEFT) && octave > 0) {
			--octave;
			prev_buttons = 0;
		}

		if (prev_buttons != buttons) {
			if (buttons & 16)	set_note(NOTE_Db, octave);
			else if	(buttons & 8)	set_note(NOTE_Eb, octave);
			else if	(buttons & 4)	set_note(NOTE_Gb, octave);
			else if	(buttons & 2)	set_note(NOTE_Ab, octave);
			else if	(buttons & 1)	set_note(NOTE_Bb, octave);
			else stop_note();
			prev_buttons = buttons;
		}

		wait_ms(1);
	}
}

REG(run);
