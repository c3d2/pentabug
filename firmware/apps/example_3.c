#include <pentabug/hal.h>
#include <pentabug/app.h>
#include <pentabug/music.h>

static void example(void) {
	if(button_clicked(LEFT)) {
		set_note(NOTE_C, 4);
		wait_ms(500);
		set_note(NOTE_D, 4);
		wait_ms(500);
		stop_note();
	}


	if(button_clicked(RIGHT)) {
		set_note(NOTE_D, 4);
		wait_ms(500);
		set_note(NOTE_C, 4);
		wait_ms(500);
		stop_note();
	}
}

REG(example);
