#include <pentabug/music.h>

#include <avr/io.h>

#include <pentabug/timer.h>
#include <pentabug/lifecycle.h>
#include <pentabug/hal.h>

static void tune(void) {
	buzzer_inv();
}

void set_note(uint16_t note, uint8_t octave) {
	if(note != NOTE_PAUSE) {
		buzzer_up();
		start_timer(PRESCALE_8, note >> octave, tune);
	} else {
		stop_note();
	}
}

void stop_note(void) {
	buzzer_off();
	stop_timer();
}

void play_melody(uint16_t notes[], size_t len, uint8_t octave, int speed) {
	int pause = speed / 20;
	uint8_t length = 4;

	size_t i;

	// TODO: get away from the int to double conversion to free half of our flash ...
	for(i = 0; i < len; ++i) {
		if(notes[i] == MLDY_PAUSE) {
			++i;
			wait_ms(notes[i] * 4 / length);
		} else if(notes[i] == MLDY_LENGTH) {
			++i;
			length = notes[i];
		} else {
			set_note(notes[i], octave);
			test_stop_app();
			wait_ms(speed * 4 / length);

			stop_note();
			test_stop_app();
			wait_ms(pause);

			length = 4;
		}
	}
}
