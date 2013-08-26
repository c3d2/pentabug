#include <pentabug/music.h>

#include <pentabug/timer.h>

#include <avr/io.h>

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
