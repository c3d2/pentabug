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
		// bring buzzer to a state where inverting it creates sound
		buzzer_up();
		// start timer with doubled frequency of tone we want to play
		start_timer(PRESCALE_8, note >> octave, tune);
	} else {
		// do nothing when paused
		stop_note();
	}
}

void stop_note(void) {
	// turn buzzer off to save energy
	buzzer_off();
	// we do not need the timer anymore
	stop_timer();
}

void play_melody(const uint16_t notes[], size_t len, uint8_t octave, int speed) {
	int pause = speed / 20;
	uint8_t length = 4;

	size_t i;

	for(i = 0; i < len; ++i) {
		if(notes[i] == MLDY_PAUSE) {
			// user defined pause
			++i;
			wait_ms(notes[i]);
		} else if(notes[i] == MLDY_LENGTH) {
			// sets length for next tone
			++i;
			length = notes[i];
		} else {
			// play note
			set_note(notes[i], octave);
			test_stop_app();
			wait_ms(speed * 4 / length);

			// pause after note
			stop_note();
			test_stop_app();
			wait_ms(pause);

			// reset length for next note
			length = 4;
		}
	}
}
