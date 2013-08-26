#include <pentabug/music.h>

#include <avr/io.h>
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>

#include <pentabug/timer.h>
#include <pentabug/lifecycle.h>

static void tune(void) {
	PORTB ^= 1 << 7;
}

void set_note(uint16_t note, uint8_t octave) {
	if(note != NOTE_PAUSE) {
		start_timer(PRESCALE_8, note >> octave, tune);
	} else {
		stop_timer();
	}
}

void stop_note(void) {
	stop_timer();
}

void play_melody(uint16_t notes[], size_t len, uint8_t octave, int speed) {
	int pause = speed / 20;

	size_t i;

	// TODO: get away from the int to double conversion to free half of our flash ...
	for(i = 0; i < len; ++i) {
		if(notes[i] == MLDY_PAUSE) {
			++i;
			_delay_ms(notes[i]);
		} else {
			set_note(notes[i], octave);
			test_stop_app();
			_delay_ms(speed);

			stop_note();
			test_stop_app();
			_delay_ms(pause);
		}
	}
}
